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

/**
 * @file
 * @page VM2AttributesFile VM2 attributes file
 *
 * The attributes file is a Lua script that return a table with 
 * the stations and variables attributes:
 *
 * @code
 *
 *  return {
 *    stations = {
 *      [145] = { lon=1285000, lat=4412645, rep='synop' },
 *      [218] = { mykey=4 
 *    },
 *    variables = {
 *      [245] = {  ... }
 *    }
 *  }
 * @endcode
 *
 * You can create a vm2::Source instance to query the attributes file.
 *
 * E.g., if you want the attributes associated to the station with id = 12:
 *
 * @code
 *  // Open the attributes file "v2m.lua"
 *  vm2::Source source("vm2.lua");
 *  // Push the station 12 on top of the stack
 *  source.lua_push_station(12);
 *  // If stations 12 exists there's a table else a nil value
 *  if (lua_istable(source.L,-1)) {
 *    ...
 *  }
 *  // Pop the table or the nil value
 *  lua_pop(L,1);
 *
 * @endcode
 *
 * Or, if you want the list of station id matching the attributes
 * `lon=1285000` and `lat=4412645`:
 *
 * @code
 *
 *  // Open the attributes file "v2m.lua"
 *  vm2::Source source("vm2.lua");
 *  // Create the query table
 *  lua_newtable(source.L);
 *  // Save the index of the query table
 *  int idx = lua_gettop(source.L);
 *  // Populate the query table
 *  lua_pushnumber(source.L, 1285000);
 *  lua_setfield(source.L, idx, "lon");
 *  lua_pushnumber(source.L, 4412645);
 *  lua_setfield(source.L, idx, "lat");
 *  // Get the list of station id matching the query table
 *  std::vector<int> stations = source.lua_find_stations(idx);
 *
 * @endcode
 */
#include <stdio.h>
#include <string>
#include <vector>

#include <wibble/exception.h>

#include <lua.hpp>

namespace vm2 {

struct Source {
  lua_State* L;
  
  int stations_ref;
  int variables_ref;
  int filter_ref;

  Source(const std::string& path);
  ~Source();

  /// Push on top of the stack the station attributes (or nil if not found)
  void lua_push_station(int id);
  /// Push on top of the stack the variable attributes (or nil if not found)
  void lua_push_variable(int id);
  /// List of station id matching the table at the given index
  std::vector<int> lua_find_stations(int idx);
  /// List of station id matching the table at the given index
  std::vector<int> lua_find_variables(int idx);
};

}

#endif        /* VM2_SOURCE_H */
