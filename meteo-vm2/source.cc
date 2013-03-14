/*
 * source - Reader for VM2 attributes file
 *
 * Copyright (C) 2012,2013 ARPA-SIM <urpsim@smr.arpa.emr.it>
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

#include <meteo-vm2/source.h>

#include <iostream>

#include <wibble/exception.h>

#define METEO_VM2_SOURCE_FILTER \
                            "return function(q, l) " \
                            "  res = {} " \
                            "  for idx,item in pairs(l) do " \
                            "    found = true " \
                            "    for k, v in pairs(q) do " \
                            "      if (item[k] ~= v) then " \
                            "        found = false " \
                            "        break " \
                            "      end " \
                            "    end " \
                            "    if (found) then " \
                            "      table.insert(res,idx) " \
                            "    end " \
                            "  end " \
                            "  return res " \
                            "end"

namespace meteo {
namespace vm2 {
namespace source {

std::string path() {
	const char* s = ::getenv("METEO_VM2_SOURCE");
	if (!s)
		return METEO_VM2_DEFAULT_SOURCE;
	return s;
}

}

CoreSource::CoreSource(const std::string& path, lua_State* L) : path(path), L(L), stations_ref(LUA_NOREF), variables_ref(LUA_NOREF), filter_ref(LUA_NOREF) {
  if (luaL_dofile(L, path.c_str()) != 0) {
    std::string msg = lua_tostring(L, -1);
    lua_pop(L, 1);
    throw wibble::exception::Consistency(
        "while loading " + path,
        msg);
  }

  lua_pushstring(L, "stations");
  lua_gettable(L, -2);
  stations_ref = luaL_ref(L, LUA_REGISTRYINDEX);
  lua_pushstring(L, "variables");
  lua_gettable(L, -2);
  variables_ref = luaL_ref(L, LUA_REGISTRYINDEX);

  if (luaL_dostring(L, METEO_VM2_SOURCE_FILTER) != 0) {
    std::string msg = lua_tostring(L, -1);
    lua_pop(L, 1);
    throw wibble::exception::Consistency(
        "while compiling filter",
        msg);
  }
  filter_ref = luaL_ref(L, LUA_REGISTRYINDEX);
  lua_pop(L, 1);
}

CoreSource::~CoreSource() {
    luaL_unref(L, LUA_REGISTRYINDEX, stations_ref);
    luaL_unref(L, LUA_REGISTRYINDEX, variables_ref);
    luaL_unref(L, LUA_REGISTRYINDEX, filter_ref);
}

void CoreSource::lua_push_station(int id) {
  lua_rawgeti(L, LUA_REGISTRYINDEX, stations_ref);
  lua_pushinteger(L, id);
  lua_gettable(L, -2);
  lua_remove(L, -2);
}
void CoreSource::lua_push_variable(int id) {
  lua_rawgeti(L, LUA_REGISTRYINDEX, variables_ref);
  lua_pushinteger(L, id);
  lua_gettable(L, -2);
  lua_remove(L, -2);
}

std::vector<int> CoreSource::lua_find_stations(int idx) {
  std::vector<int> res;

  lua_rawgeti(L, LUA_REGISTRYINDEX, filter_ref);
  lua_pushvalue(L, idx);
  lua_rawgeti(L, LUA_REGISTRYINDEX, stations_ref);
  if (lua_pcall(L, 2, 1, 0) != 0) {
    std::string msg = lua_tostring(L, -1);
    lua_pop(L,1);
    throw wibble::exception::Consistency(
        "while filtering stations",
        msg);
  }
  lua_pushnil(L);
  while (lua_next(L, -2)) {
    res.push_back(lua_tointeger(L, -1));
    lua_pop(L,1);
  }
  lua_pop(L,1);
  return res;
}
std::vector<int> CoreSource::lua_find_variables(int idx) {
  std::vector<int> res;

  lua_rawgeti(L, LUA_REGISTRYINDEX, filter_ref);
  lua_pushvalue(L, idx);
  lua_rawgeti(L, LUA_REGISTRYINDEX, variables_ref);
  if (lua_pcall(L, 2, 1, 0) != 0) {
    std::string msg = lua_tostring(L, -1);
    lua_pop(L,1);
    throw wibble::exception::Consistency(
        "while filtering variables",
        msg);
  }
  lua_pushnil(L);
  while (lua_next(L, -2)) {
    res.push_back(lua_tointeger(L, -1));
    lua_pop(L,1);
  }
  lua_pop(L,1);
  return res;
}

Source* Source::instance = NULL;

Source* Source::get() {
  if (!instance) {
    instance = new Source(source::path());
  }
  return instance;
}

Source::Source(const std::string& path) {
  L = lua_open();
  luaL_openlibs(L);
  coresource = new CoreSource(path, L);
}
Source::~Source() {
  delete coresource;
  lua_close(L);
}

void Source::lua_push_station(int id) {
  return coresource->lua_push_station(id);
}
void Source::lua_push_variable(int id) {
  return coresource->lua_push_variable(id);
}
std::vector<int> Source::lua_find_stations(int idx) {
  return coresource->lua_find_stations(idx);
}
std::vector<int> Source::lua_find_variables(int idx) {
  return coresource->lua_find_variables(idx);
}

}
}
