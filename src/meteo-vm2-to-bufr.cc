/*
 * meteo-vm2-to-bufr - Convert VM2 to generic BUFR
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

#include <meteo-vm2/source.h>
#include <meteo-vm2/parser.h>

#include <wreport/varinfo.h>
#include <wreport/var.h>
#include <wreport/conv.h>
#include <dballe/message.h>


static inline int convert_qc(const std::string& str)
{
  int qc = atoi(str.c_str());
  return (int)rint(100.0 - ((qc-48.0) * 100.0 / 6.0));
}

static inline void set_station(meteo::vm2::Source* source, const meteo::vm2::Value& value, dballe::Message& msg) {
    lua_State* L = source->L;
    source->lua_push_station(value.station_id);
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        throw std::runtime_error("Station " + std::to_string(value.station_id) + " not found");
    }
    lua_pushnil(L);
    while(lua_next(L, -2)) {
        std::string bcode = lua_tostring(L, -2);
        wreport::Varcode varcode;
        // Handle "lon", "lat" and "rep" aliases
        if (bcode == "lon")
            varcode = WR_VAR(0, 6, 1);
        else if (bcode == "lat")
            varcode = WR_VAR(0, 5, 1);
        else if (bcode == "rep")
            varcode = WR_VAR(0, 1,194);
        else if (bcode == "ident")
            varcode = WR_VAR(0, 1, 11);
        else
            varcode = wreport::varcode_parse(bcode.c_str());
        if (lua_type(L, -1) == LUA_TNUMBER) {
            msg.set(dballe::var(varcode, (int) lua_tointeger(L, -1)), varcode,
                    dballe::Level(257), dballe::Trange());
        } else {
            msg.set(dballe::Level(257), dballe::Trange(), dballe::var(varcode, lua_tostring(L, -1)));
        }
        lua_pop(L,1);
    }
    lua_pop(L, 1);
}

static inline void set_variable(meteo::vm2::Source* source, const meteo::vm2::Value& value, dballe::Message& msg) {
    lua_State* L = source->L;
    source->lua_push_variable(value.variable_id);
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        throw std::runtime_error("Variable " + std::to_string(value.variable_id) + " not found");
    }
    lua_getfield(L, -1, "bcode");
    wreport::Varcode varcode = wreport::varcode_parse(lua_tostring(L, -1));
    dballe::Trange trange;
    lua_pop(L, 1);
    lua_getfield(L, -1, "tr");
    trange.pind = lua_tointeger(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, -1, "p1");
    trange.p1 = lua_tointeger(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, -1, "p2");
    trange.p2 = lua_tointeger(L, -1);
    lua_pop(L, 1);
    dballe::Level level;
    lua_getfield(L, -1, "lt1");
    level.ltype1 = lua_tointeger(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, -1, "l1");
    if (!lua_isnil(L, -1))
        level.l1 = lua_tointeger(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, -1, "lt2");
    if (!lua_isnil(L, -1))
        level.ltype2 = lua_tointeger(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, -1, "l2");
    if (!lua_isnil(L, -1))
        level.l2 = lua_tointeger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, -1, "unit");
    std::string unit = dballe::varinfo(varcode)->unit;
    if (!lua_isnil(L, -1))
        unit = lua_tostring(L, -1);
    lua_pop(L, 1);

    if (value.value1 == meteo::vm2::MISSING_DOUBLE)
        throw std::runtime_error("Cannot convert missing value to BUFR");

    double val = wreport::convert_units(unit.c_str(), 
            dballe::varinfo(varcode)->unit, 
            value.value1);

    wreport::Var var = dballe::var(varcode, val);
    if (value.flags.size() == 9) {
        if (value.flags[0] == '1')
            var.seta(dballe::var(WR_VAR(0, 33, 196), 1));
        if (value.flags[0] == '2')
            var.seta(dballe::var(WR_VAR(0, 33, 197), 1));
        if (value.flags.substr(1,2) != "00")
            var.seta(dballe::var(WR_VAR(0, 33, 192), convert_qc(value.flags.substr(1,2))));
        if (value.flags.substr(3,2) != "00")
            var.seta(dballe::var(WR_VAR(0, 33, 193), convert_qc(value.flags.substr(3,2))));
        if (value.flags.substr(5,2) != "00")
            var.seta(dballe::var(WR_VAR(0, 33, 194), convert_qc(value.flags.substr(5,2))));
    }
    msg.set(level, trange, var);
}

void print_usage()
{
    std::cout
        << "Usage: meteo-vm2-to-bufr [options] [SOURCEFILE]" << std::endl
        << std::endl
        << "Convert VM2 from stdin to generic BUFR to stdout." << std::endl
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

    meteo::vm2::Parser parser(std::cin);

    meteo::vm2::Value value;

    std::string line;
    lua_State* L = source->L;
    while (parser.next(value, line)) {
      int top = lua_gettop(L);

      try {
        dballe::Messages msgs;
        std::uniqe_ptr<dballe::Message> msg = dballe::Message::create(dballe::MessageType::GENERIC);
        // date
        msg.set("year", value.year);
        msg.set("year", value.month);
        msg.set("year", value.mday);
        msg.set("year", value.hour);
        msg.set("year", value.min);
        msg.set("year", value.sec);
        // station
        set_station(source, value, msg);
        // variable
        set_variable(source, value, msg);

        msgs.append(msg);

        dballe::msg::BufrExporter exporter;
        std::cout << exporter.to_binary(msgs);

      } catch (std::exception& e) {
        std::cerr << parser.lineno << ":[" << line << "] - " << e.what() << std::endl;
      }

      lua_settop(L, top);
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
