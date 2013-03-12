/*
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

#include <wibble/tests.h>

#include "source.h"
#include <stdlib.h>

#define ensure_throws(x) do { try { x; ensure(false); } catch (wibble::exception::Generic& e) { } } while (0)

namespace tut {

struct meteo_vm2_source_shar {
  lua_State* L;

  meteo_vm2_source_shar() {
    L = lua_open();
    luaL_openlibs(L);
  }
  ~meteo_vm2_source_shar() {
    lua_close(L);
  }
};
TESTGRP(meteo_vm2_source);

// Load files
template<> template<>
void to::test<1>()
{
  meteo::vm2::Source source1(TOP_SRCDIR"/test/data/source-1.lua");
  meteo::vm2::Source source2(TOP_SRCDIR"/test/data/source-2.luac");
}


// Lua table from station id
template<> template<>
void to::test<2>()
{
  meteo::vm2::Source source(TOP_SRCDIR"/test/data/source-1.lua");
  lua_State *L = source.L;

  int start_idx = lua_gettop(L);

  source.lua_push_station(1);
  ensure(lua_istable(L, -1));
  
  lua_getfield(L, -1, "rep");
  ensure_equals("locali", std::string(lua_tostring(L, -1)));
  lua_pop(L,1);
  
  lua_getfield(L, -1, "lon");
  ensure_equals(1207738, lua_tonumber(L, -1));
  lua_pop(L,1);

  lua_getfield(L, -1, "lat");
  ensure_equals(4460016, lua_tonumber(L, -1));
  lua_pop(L,1);

  lua_pop(L,1);
  ensure_equals(start_idx, lua_gettop(L));
}
// Lua table from variable id
template<> template<>
void to::test<3>()
{
  meteo::vm2::Source source(TOP_SRCDIR"/test/data/source-1.lua");
  lua_State *L = source.L;
  int start_idx = lua_gettop(L);

  source.lua_push_variable(1);
  ensure(lua_istable(L, -1));
  
  lua_getfield(L, -1, "bcode");
  ensure_equals("B20013", std::string(lua_tostring(L, -1)));
  lua_pop(L,1);
  
  lua_getfield(L, -1, "lt1");
  ensure_equals(256, lua_tonumber(L, -1));
  lua_pop(L,1);

  lua_getfield(L, -1, "l1");
  ensure_equals(0, lua_tonumber(L, -1));
  lua_pop(L,1);

  lua_getfield(L, -1, "lt2");
  ensure_equals(258, lua_tonumber(L, -1));
  lua_pop(L,1);

  lua_getfield(L, -1, "l2");
  ensure(lua_isnil(L, -1));
  lua_pop(L,1);

  lua_getfield(L, -1, "tr");
  ensure_equals(254, lua_tonumber(L, -1));
  lua_pop(L,1);

  lua_getfield(L, -1, "p1");
  ensure_equals(0, lua_tonumber(L, -1));
  lua_pop(L,1);

  lua_getfield(L, -1, "p2");
  ensure_equals(0, lua_tonumber(L, -1));
  lua_pop(L,1);

  lua_pop(L,1);
  ensure_equals(start_idx, lua_gettop(L));
}
// Stations from Lua table
template<> template<>
void to::test<4>()
{
  meteo::vm2::Source source(TOP_SRCDIR"/test/data/source-1.lua");
  lua_State *L = source.L;
  int qidx;

  lua_newtable(L);
  qidx = lua_gettop(L);
  lua_pushstring(L, "locali");
  lua_setfield(L, qidx, "rep");

  std::vector<int> res = source.lua_find_stations(qidx);
  ensure(res.size() > 0);
  for (std::vector<int>::const_iterator i = res.begin();
       i != res.end(); ++i) {
    source.lua_push_station(*i);
    ensure(lua_istable(L, -1));
  
    lua_getfield(L, -1, "rep");
    ensure_equals("locali", std::string(lua_tostring(L, -1)));
    lua_pop(L,1);

    lua_pop(L,1);
  }
}
// Variables from Lua table
template<> template<>
void to::test<5>()
{
  meteo::vm2::Source source(TOP_SRCDIR"/test/data/source-1.lua");
  lua_State *L = source.L;
  int qidx;

  lua_newtable(L);
  qidx = lua_gettop(L);
  lua_pushstring(L, "B20013");
  lua_setfield(L, qidx, "bcode");

  std::vector<int> res = source.lua_find_variables(qidx);
  ensure(res.size() > 0);
  for (std::vector<int>::const_iterator i = res.begin();
       i != res.end(); ++i) {
    source.lua_push_variable(*i);
    ensure(lua_istable(L, -1));
  
    lua_getfield(L, -1, "bcode");
    ensure_equals("B20013", std::string(lua_tostring(L, -1)));
    lua_pop(L,1);

    lua_pop(L,1);
  }
}
// Open a CoreSource
template<> template<>
void to::test<6>()
{
  meteo::vm2::CoreSource source1(TOP_SRCDIR"/test/data/source-1.lua", L);
  meteo::vm2::CoreSource source2(TOP_SRCDIR"/test/data/source-2.luac", L);
}
// Test default source
template<> template<>
void to::test<7>()
{
  ::setenv("METEO_VM2_SOURCE", TOP_SRCDIR"/test/data/source-2.luac", 1);
  lua_State* L = meteo::vm2::Source::get()->L;
  meteo::vm2::Source::get()->lua_push_station(1);
  ensure(lua_istable(L, -1));
  lua_pop(L, 1);
}



}
