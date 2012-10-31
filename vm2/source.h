/*
 * source - Reader for VM2 attributes file
 *
 * Copyright (C) 2012  ARPA-SIM <urpsim@smr.arpa.emr.it>
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
#ifndef VM2_SOURCE_H
#define VM2_SOURCE_H

#include <stdio.h>
#include <string>
#include <vector>

#include <wibble/exception.h>

#include <lua.hpp>

namespace vm2 {

/**
 * The lua file has the following schema:
 *  
 * \code{.lua}
 *
 *  return {
 *    stations = {
 *      [id] = { key=value, ... },
 *      ...
 *    },
 *    variables = {
 *      [id] = { key=value, ... },
 *    }
 *  }
 * \encode
 *
 * Get the attributes associated to the station with id = 12
 *
 * \code{.cpp}
 * 
 *  vm2::Source source("vm2.lua");
 *  source.lua_push_station(12);
 *  if (lua_istable(source.L,-1)) {
 *    ...
 *  }
 *  lua_pop(L,1);
 *
 * \encode
 *
 * Get the list of station id containing the attributes `{lon=12,lat=44}`
 *
 * \code{.cpp}
 *  vm2::Source source("vm2.lua");
 *  lua_newtable(source.L);
 *  int idx = lua_gettop(source.L);
 *  lua_pushnumber(source.L, 12);
 *  lua_setfield(source.L, idx, "lon");
 *  lua_pushnumber(source.L, 44);
 *  lua_setfield(source.L, idx, "lat");
 *  std::vector<int> stations = source.lua_find_stations(idx);
 *
 * \encode
 */
struct Source {
  lua_State* L;
  
  int stations_ref;
  int variables_ref;
  int filter_ref;

  Source(const std::string& path);
  ~Source();

  // Push on top of the stack the station attributes (or nil if not found)
  void lua_push_station(int id);
  // Push on top of the stack the variable attributes (or nil if not found)
  void lua_push_variable(int id);
  // List of station id matching the table at the given index
  std::vector<int> lua_find_stations(int idx);
  // List of station id matching the table at the given index
  std::vector<int> lua_find_variables(int idx);
};

}

#endif        /* VM2_SOURCE_H */
