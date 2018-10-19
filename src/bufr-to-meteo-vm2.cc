/*
 * bufr-to-meteo-vm2 - Convert BUFR to VM2
 *
 * Copyright (C) 2012-2014 Arpae-SIMC <simc-urp@arpae.it>
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
 * Author: Emanuele Di Giacomo <edigiacomo@arpae.it>
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <iterator>

#include <meteo-vm2/source.h>
#include <meteo-vm2/parser.h>

#include <wreport/varinfo.h>
#include <wreport/conv.h>
#include <dballe/var.h>
#include <dballe/message.h>
#include <dballe/types.h>


template<typename T>
static std::string join(std::vector<T> l)
{
    std::stringstream ss;
    std::string s;
    std::copy(l.begin(), l.end(), std::ostream_iterator<T>(ss, ","));
    s = ss.str();
    s.pop_back();
    return s;
}

static inline std::string convert_qc_back(int qc)
{
    int qcint = (int)rint((100.0 - qc) * 6.0 / 100.0 + 48);
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << qcint;
    return ss.str();
}

// Wrapper for dballe::format_code
static inline std::string format_code(wreport::Varcode code)
{
    char c[7];
    dballe::format_code(code, c);
    return c;
}

void print_usage()
{
    std::cout
        << "Usage: bufr-to-meteo-vm2 [options] [SOURCEFILE]" << std::endl
        << std::endl
        << "Convert BUFR from stdin to generic VM2 to stdout." << std::endl
        << "Options:" << std::endl
        << "-h, --help      show this help and exit" << std::endl
        << "--version   show version and exit" << std::endl;
}

int main(int argc, const char** argv)
{
    std::string sourcefile;

    if (argc > 1) {
        if (std::string(argv[1]) == "-h" or std::string(argv[1]) == "--help") {
            print_usage();
            return 0;
        }
        else if (std::string(argv[1]) == "--version") {
            std::cout << "meteo-vm2-to-bufr " PACKAGE_VERSION << std::endl;
            return 0;
        }
        else
            sourcefile = argv[1];
    }
    try {
        meteo::vm2::Source* source = NULL;

        if (sourcefile.empty())
            source = new meteo::vm2::Source(METEO_VM2_BUFR_SOURCE);
        else
            source = new meteo::vm2::Source(sourcefile);

        std::unique_ptr<dballe::File> infile = dballe::File::create(stdin, false, "stdin");

        infile->foreach([&source](const dballe::BinaryMessage& bmsg) {
            std::unique_ptr<dballe::Importer> importer = dballe::Importer::create(dballe::Encoding::BUFR);
            return importer->foreach_decoded(bmsg, [&source](std::unique_ptr<dballe::Message>&& msg) {
                lua_State* L = source->L;

                int idx;
                meteo::vm2::Value vm2value;
                // station
                lua_newtable(L);
                idx = lua_gettop(L);
                // Station lookup by lon, lat, rep_memo
                if (wreport::Var* v = msg.get("lon")) {
                    lua_pushnumber(L, v->enqi());
                    lua_setfield(L, idx, "lon");
                } else {
                    std::cerr << "Missing longitude" << std::endl;
                    return true;
                }
                if (wreport::Var* v = msg.get("lat")) {
                    lua_pushnumber(L, v->enqi());
                    lua_setfield(L, idx, "lat");
                } else {
                    std::cerr << "Missing latitude" << std::endl;
                    return true;
                }
                if (wreport::Var* v = msg.get("rep_memo")) {
                    lua_pushstring(L, v->enqc());
                    lua_setfield(L, idx, "rep");
                } else {
                    std::cerr << "Missing rep_memo" << std::endl;
                    return true;
                }
                if (wreport::Var* v = msg.get("ident")) {
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
                        << join(stations)
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

                msg.foreach_var([](const Level& level, const Trange& trange, const wreport::Var& var) {
                    if (level.is_missing() and trange.is_missing()) {
                        return true;
                    }
                    // variable
                    lua_newtable(L);
                    idx = lua_gettop(L);
                    lua_pushstring(L, format_code(var.code()).c_str());
                    lua_setfield(L, idx, "bcode");
                    lua_pushinteger(L, trange.pind);
                    lua_setfield(L, idx, "tr");
                    lua_pushinteger(L, trange.p1);
                    lua_setfield(L, idx, "p1");
                    lua_pushinteger(L, trange.p2);
                    lua_setfield(L, idx, "p2");
                    lua_pushinteger(L, level.ltype1);
                    lua_setfield(L, idx, "lt1");
                    if (level.l1 != dballe::MISSING_INT) {
                        lua_pushinteger(L, level.l1);
                        lua_setfield(L, idx, "l1");
                    }
                    if (level.ltype2 != dballe::MISSING_INT) {
                        lua_pushinteger(L, level.ltype2);
                        lua_setfield(L, idx, "lt2");
                    }
                    if (level.l2 != dballe::MISSING_INT) {
                        lua_pushinteger(L, level.l2);
                        lua_setfield(L, idx, "l2");
                    }
                    std::vector<int> variables = source->lua_find_variables(idx);
                    lua_settop(L, idx);
                    if (variables.size() == 0) {
                        std::cerr << "cannot find variable with "
                            << "bcode=" << format_code(var.code()) << ", "
                            << "level=" << level << ", "
                            << "trange=" << trange << std::endl;
                        continue;
                    }
                    if (variables.size() > 1) {
                        std::cerr << variables.size()
                            << " variables with same attributes"
                            << " ("
                            << join(variables)
                            << ")"
                            << std::endl;
                        continue;
                    }
                    vm2value.variable_id = variables.at(0);

                    source->lua_push_variable(vm2value.variable_id);
                    lua_getfield(L, -1, "unit");
                    if (!lua_isnil(L, -1)) {
                        vm2value.value1 = wreport::convert_units(var.info()->unit, lua_tostring(L, -1), var.enqd());
                    } else {
                        vm2value.value1 = v.enqd();
                    }
                    lua_pop(L, 1);

                    vm2value.value2 = meteo::vm2::MISSING_DOUBLE;
                    vm2value.value3 = "";
                    // TODO: flags
                    const wreport::Var* vattr = var.next_attr();
                    if (vattr == NULL) {
                        vm2value.flags = "";
                    } else {
                        vm2value.flags = "000000000";
                        for (vattr = var.next_attr(); vattr != NULL;
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
                    return true;
                });
            });
        });
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
