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
#include <wibble/exception.h>

#include <set>
#include <meteo-vm2/source.h>

namespace tut {

struct simc_conf_shar {
    meteo::vm2::Source& source;
    lua_State* L;

    simc_conf_shar() : source(*meteo::vm2::Source::get()) {
        L = source.L;
    }
    std::vector<int> get_all_stations() {
        int idx;
        lua_newtable(L);
        idx = lua_gettop(L);
        std::vector<int> ids = source.lua_find_stations(idx);
        lua_pop(L, 1);
        return ids;
    }
    void get_station_unique_keys(int id, int& lon, int& lat, std::string& rep) {
        lon = -1;
        lat = -1;
        rep = "";
        source.lua_push_station(id);
        lua_getfield(L, -1, "lon");
        if (lua_isnumber(L, -1))
            lon = lua_tointeger(L, -1);
        lua_pop(L, 1);
        lua_getfield(L, -1, "lat");
        if (lua_isnumber(L, -1))
            lat = lua_tointeger(L, -1);
        lua_pop(L, 1);
        lua_getfield(L, -1, "rep");
        if (lua_isstring(L, -1))
            rep = lua_tostring(L, -1);
        lua_pop(L, 1);
        lua_pop(L, 1);
    }
};
TESTGRP(simc_conf);

// Check stations have lon, lat and rep keys
template<> template<>
void to::test<1>()
{
    std::vector<int> ids = get_all_stations();
    for (std::vector<int>::const_iterator i = ids.begin();
         i != ids.end(); ++i) {
        int lon, lat;
        std::string rep;
        get_station_unique_keys(*i, lon, lat, rep);
        ensure(lon != -1);
        ensure(lat != -1);
        ensure(rep != "");
    }
}
// Check stations are unique
template<> template<>
void to::test<2>()
{
    std::vector<int> ids = get_all_stations();
    std::map<std::string, int> keys;
    for (std::vector<int>::const_iterator i = ids.begin();
         i != ids.end(); ++i) {
        int lon, lat;
        std::string rep;
        get_station_unique_keys(*i, lon, lat, rep);
        std::string key = wibble::str::fmtf("%d,%d,%s", lon, lat, rep.c_str());
        ensure(keys.find(key) == keys.end());
        keys[key] = *i;
    }
    ensure_equals(ids.size(), keys.size());
}

}
