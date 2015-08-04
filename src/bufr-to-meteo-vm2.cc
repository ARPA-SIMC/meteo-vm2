/*
 * bufr-to-meteo-vm2 - Convert BUFR to VM2
 *
 * Copyright (C) 2012-2014 ARPA-SIM <urpsim@smr.arpa.emr.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Author: Emanuele Di Giacomo <edigiacomo@arpa.emr.it>
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>

#include <iostream>
#include <fstream>
#include <cstring>

#include <wibble/string.h>
#include <wibble/commandline/parser.h>
#include <wreport/varinfo.h>
#include <wreport/conv.h>
#include <meteo-vm2/source.h>
#include <meteo-vm2/parser.h>
#include <dballe/core/defs.h>
#include <dballe/core/var.h>
#include <dballe/core/file.h>
#include <dballe/msg/msg.h>
#include <dballe/msg/context.h>
#include <dballe/msg/wr_codec.h>

#include <meteo-vm2/source.h>

using wibble::commandline::StandardParserWithManpage;

struct Options : public StandardParserWithManpage {
    Options() : StandardParserWithManpage("bufr-to-meteo-vm2", PACKAGE_VERSION,
            1, PACKAGE_BUGREPORT) {
        usage = "[SOURCEFILE]";
        description = "Convert BUFR to VM2.";
        longDescription = "This program convert BUFR to VM2.\n"
            "If SOURCEFILE is given, use this file as conversion table.";
    }
};

static inline std::string convert_qc_back(int qc)
{
    int qcint = (int)rint((100.0 - qc) * 6.0 / 100.0 + 48);
    return wibble::str::fmtf("%02d", qcint);
}

// Wrapper for dballe::format_code
static inline std::string format_code(wreport::Varcode code)
{
    char c[7];
    dballe::format_code(code, c);
    return c;
}

int main(int argc, const char** argv)
{
    try {
        meteo::vm2::Source* source = NULL;

        Options opts;

        if (opts.parse(argc, argv))
            return 0;

        if (opts.hasNext())
            source = new meteo::vm2::Source(opts.next());
        else
            source = new meteo::vm2::Source(METEO_VM2_BUFR_SOURCE);


        std::auto_ptr<dballe::File> infile(dballe::File::create(dballe::File::BUFR, "(stdin)", "r").release());

        infile->foreach([&source](const dballe::BinaryMessage& bmsg) {
            dballe::msg::BufrImporter importer;
            return importer.foreach_decoded(bmsg, [&source](std::unique_ptr<dballe::Message>&& msgptr) {
                const dballe::Msg& msg = dballe::Msg::downcast(*msgptr);
                lua_State* L = source->L;

                int idx;
                meteo::vm2::Value vm2value;
                const dballe::msg::Context* sta_ctx = msg.find_station_context();
                if (!sta_ctx) {
                        std::cerr << "Cannot find station context" << std::endl;
                        return true;
                    }
                // station
                lua_newtable(L);
                idx = lua_gettop(L);
                // Station lookup by lon, lat, rep_memo
                if (const wreport::Var *v = sta_ctx->find(WR_VAR(0, 6, 1))) {
                    lua_pushnumber(L, v->enqi());
                    lua_setfield(L, idx, "lon");
                } else {
                    std::cerr << "Missing longitude" << std::endl;
                    return true;
                }
                if (const wreport::Var *v = sta_ctx->find(WR_VAR(0, 5, 1))) {
                    lua_pushnumber(L, v->enqi());
                    lua_setfield(L, idx, "lat");
                } else {
                    std::cerr << "Missing latitude" << std::endl;
                    return true;
                }
                if (const wreport::Var *v = sta_ctx->find(WR_VAR(0, 1,194))) {
                    lua_pushstring(L, v->enqc());
                    lua_setfield(L, idx, "rep");
                } else {
                    std::cerr << "Missing rep_memo" << std::endl;
                    return true;
                }
                if (const wreport::Var *v = sta_ctx->find(WR_VAR(0, 1, 11))) {
                    lua_pushstring(L, v->enqc());
                    lua_setfield(L, idx, "ident");
                }
                std::vector<int> stations = source->lua_find_stations(idx);
                lua_settop(L, idx);
                if (stations.size() == 0) {
                    std::cerr << "cannot find station" << std::endl;
                    return true;
                }
                if (stations.size() > 1) {
                    std::cerr << stations.size()
                        << " stations with same attributes"
                        << " ("
                        << wibble::str::join(stations.begin(), stations.end())
                        << ")"
                        << std::endl;
                        return true;
                }
                vm2value.station_id = stations.at(0);
                vm2value.year = msg.get_datetime().date().year;
                vm2value.month = msg.get_datetime().date().month;
                vm2value.mday = msg.get_datetime().date().day;
                vm2value.hour = msg.get_datetime().time().hour;
                vm2value.min = msg.get_datetime().time().minute;
                vm2value.sec = msg.get_datetime().time().second;

                for (std::vector<dballe::msg::Context*>::const_iterator ci = msg.data.begin(); ci != msg.data.end(); ++ci) {
                    if (msg.find_station_context() == *ci) continue;
                    dballe::msg::Context& c = **ci;
                    for (std::vector<wreport::Var*>::const_iterator vi = c.data.begin(); vi != c.data.end(); ++vi) {
                        wreport::Var& v = **vi;
                        // variable
                        lua_newtable(L);
                        idx = lua_gettop(L);
                        lua_pushstring(L, format_code(v.code()).c_str());
                        lua_setfield(L, idx, "bcode");
                        lua_pushinteger(L, c.trange.pind);
                        lua_setfield(L, idx, "tr");
                        lua_pushinteger(L, c.trange.p1);
                        lua_setfield(L, idx, "p1");
                        lua_pushinteger(L, c.trange.p2);
                        lua_setfield(L, idx, "p2");
                        lua_pushinteger(L, c.level.ltype1);
                        lua_setfield(L, idx, "lt1");
                        if (c.level.l1 != dballe::MISSING_INT) {
                            lua_pushinteger(L, c.level.l1);
                            lua_setfield(L, idx, "l1");
                        }
                        if (c.level.ltype2 != dballe::MISSING_INT) {
                            lua_pushinteger(L, c.level.ltype2);
                            lua_setfield(L, idx, "lt2");
                        }
                        if (c.level.l2 != dballe::MISSING_INT) {
                            lua_pushinteger(L, c.level.l2);
                            lua_setfield(L, idx, "l2");
                        }
                        std::vector<int> variables = source->lua_find_variables(idx);
                        lua_settop(L, idx);
                        if (variables.size() == 0) {
                            std::cerr << "cannot find variable with "
                                << "bcode=" << format_code(v.code()) << ", "
                                << "level=" << c.level << ", "
                                << "trange=" << c.trange << std::endl;
                            continue;
                        }
                        if (variables.size() > 1) {
                            std::cerr << variables.size()
                                << " variables with same attributes"
                                << " ("
                                << wibble::str::join(variables.begin(), variables.end())
                                << ")"
                                << std::endl;
                            continue;
                        }
                        vm2value.variable_id = variables.at(0);

                        source->lua_push_variable(vm2value.variable_id);
                        lua_getfield(L, -1, "unit");
                        if (!lua_isnil(L, -1)) {
                            vm2value.value1 = wreport::convert_units(v.info()->unit, lua_tostring(L, -1), v.enqd());
                        } else {
                            vm2value.value1 = v.enqd();
                        }
                        lua_pop(L, 1);

                        vm2value.value2 = meteo::vm2::MISSING_DOUBLE;
                        vm2value.value3 = "";
                        // TODO: flags
                        const wreport::Var* vattr = v.next_attr();
                        if (vattr == NULL) {
                            vm2value.flags = "";
                        } else {
                            vm2value.flags = "000000000";
                            for (vattr = v.next_attr(); vattr != NULL; 
                                vattr = vattr->next_attr()) {
                                if (vattr->code() == WR_VAR(0,33,196) && vattr->enqi() == 1)
                                    vm2value.flags[0] = '1';
                                if (vattr->code() == WR_VAR(0,33,197) && vattr->enqi() == 1)
                                    vm2value.flags[0] = '2';
                                if (vattr->code() == WR_VAR(0,33,192)) {
                                    std::string qc = convert_qc_back(vattr->enqi());
                                    vm2value.flags[1] = qc.at(0);
                                    vm2value.flags[2] = qc.at(1);
                                }
                                if (vattr->code() == WR_VAR(0,33,193)) {
                                    std::string qc = convert_qc_back(vattr->enqi());
                                    vm2value.flags[3] = qc.at(0);
                                    vm2value.flags[4] = qc.at(1);
                                }
                                if (vattr->code() == WR_VAR(0,33,194)) {
                                    std::string qc = convert_qc_back(vattr->enqi());
                                    vm2value.flags[5] = qc.at(0);
                                    vm2value.flags[6] = qc.at(1);
                                }
                            }
                        }
                        meteo::vm2::Parser::serialize(std::cout, vm2value);
                    }
                }
                return true;
            });
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
        return 0;
    }
