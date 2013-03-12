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
#ifndef METEO_VM2_SOURCE_H
#define METEO_VM2_SOURCE_H
/**
 * @file
 * @brief Reader for VM2 attributes file
 * @see @ref VM2AttributesFile
 */

#include <stdio.h>
#include <string>
#include <vector>

#include <lua.hpp>

namespace meteo {
namespace vm2 {
namespace source {

/**
 * @brief Default source path.
 * The path is the value of the environment variable `METEO_VM2_SOURCE` or, if
 * unset, `$localstatedir/lib/meteo-vm2/source/default.luac`.
 */
std::string path();

}

/**
 * Attributes file reader.
 *
 * This class loads the attributes file (see \ref VM2AttributesFile)
 * in a previously opened Lua interpreter.
 *
 * The stations and variables attributes are stored in Lua references.
 */
struct CoreSource {
  std::string path;
  lua_State *L;

  int stations_ref;
  int variables_ref;
  int filter_ref;

  /// Load the attributes file path in Lua VM L
  CoreSource(const std::string& path, lua_State* L);
  /// Unload the attributes
  ~CoreSource();

  /// Push on top of the stack the station attributes (or nil if not found)
  void lua_push_station(int id);
  /// Push on top of the stack the variable attributes (or nil if not found)
  void lua_push_variable(int id);
  /// List of station id matching the table at the given index
  std::vector<int> lua_find_stations(int idx);
  /// List of station id matching the table at the given index
  std::vector<int> lua_find_variables(int idx);
};

/**
 * Helper class for CoreSource, creates the CoreSource and the Lua interpreter.
 */
class Source {
 private:
  static Source* instance;

  CoreSource* coresource;

 public:
  /// Lua VM
  lua_State* L;

  /**
   * Get the default source (singleton) using the default path
   * (`METEO_VM2_SOURCE` environment variable or, if unset,
   * `$sharedstatedir/meteo-vm2/source/default.luac`).
   */
  static Source* get();

  Source(const std::string& path);
  ~Source();

  /// @see CoreSource::lua_push_station
  void lua_push_station(int id);
  /// @see CoreSource::lua_push_variable
  void lua_push_variable(int id);
  /// @see CoreSource::lua_find_stations
  std::vector<int> lua_find_stations(int idx);
  /// @see CoreSource::lua_find_variables
  std::vector<int> lua_find_variables(int idx);
};

}
}

#endif        /* METEO_VM2_SOURCE_H */
