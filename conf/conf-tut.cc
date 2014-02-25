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

#include <meteo-vm2/source.h>
#include <wreport/conv.h>
#include <dballe/core/record.h>
#include <dballe/core/defs.h>

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
    std::vector<int> get_all_variables() {
        int idx;
        lua_newtable(L);
        idx = lua_gettop(L);
        std::vector<int> ids = source.lua_find_variables(idx);
        lua_pop(L, 1);
        return ids;
    }
    void get_item_value(const char* name, const char* key, dballe::Record& record) {
        lua_getfield(L, -1, name);
        switch (lua_type(L, -1)) {
            case LUA_TSTRING:
                record[key].setc(lua_tostring(L, -1));
                break;
            case LUA_TNUMBER:
                record[key].seti(lua_tointeger(L, -1));
                break;
            case LUA_TNIL:
                record[key].setc("");
                break;
        }
        lua_pop(L, 1);
    }
    void get_item_value(const char* name, std::string& val) {
        lua_getfield(L, -1, name);
        if (!lua_isnil(L, -1))
            val = lua_tostring(L, -1);
        lua_pop(L, 1);
    }
    void get_station_unique_keys(int id, dballe::Record& record) {
        source.lua_push_station(id);
        get_item_value("lon", "lon", record);
        get_item_value("lat", "lat", record);
        get_item_value("rep", "rep_memo", record);
        get_item_value("ident", "ident", record);
        lua_pop(L, 1);
    }
    void get_variable_unique_keys(int id, dballe::Record& record) {
        source.lua_push_variable(id);
        get_item_value("bcode", "var", record);
        get_item_value("tr", "pindicator", record);
        get_item_value("p1", "p1", record);
        get_item_value("p2", "p2", record);
        get_item_value("lt1", "leveltype1", record);
        get_item_value("l1", "l1", record);
        get_item_value("lt2", "leveltype2", record);
        get_item_value("l2", "l2", record);
        lua_pop(L, 1);
    }
};
TESTGRP(simc_conf);

// Check stations have unique keys
template<> template<>
void to::test<1>()
{
    std::vector<int> ids = get_all_stations();
    for (std::vector<int>::const_iterator i = ids.begin();
         i != ids.end(); ++i) {
        dballe::Record record;
        get_station_unique_keys(*i, record);
        ensure(record.key_peek(dballe::DBA_KEY_LON));
        ensure(record.key_peek(dballe::DBA_KEY_LAT));
        ensure(record.key_peek(dballe::DBA_KEY_REP_MEMO));
        // DBA_KEY_IDENT is an optional workaround
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
        dballe::Record record;
        get_station_unique_keys(*i, record);
        std::string key = wibble::str::fmtf("/%d,%d/%s/%s",
                                            record.key(dballe::DBA_KEY_LON).enqi(),
                                            record.key(dballe::DBA_KEY_LAT).enqi(),
                                            record.key(dballe::DBA_KEY_REP_MEMO).enqc(),
                                            record.key(dballe::DBA_KEY_IDENT).enqc());
        ensure(keys.find(key) == keys.end());
        keys[key] = *i;
    }
    ensure_equals(ids.size(), keys.size());
}
// Check variables have unique keys
template<> template<>
void to::test<3>()
{
    std::vector<int> ids = get_all_variables();
    for (std::vector<int>::const_iterator i = ids.begin();
         i != ids.end(); ++i) {
        dballe::Record record;
        get_variable_unique_keys(*i, record);
        ensure(record.key_peek(dballe::DBA_KEY_VAR));
        ensure(record.key_peek(dballe::DBA_KEY_PINDICATOR));
        ensure(record.key_peek(dballe::DBA_KEY_P1));
        ensure(record.key_peek(dballe::DBA_KEY_P2));
        ensure(record.key_peek(dballe::DBA_KEY_LEVELTYPE1));
    }
}
// Check variables are unique
template<> template<>
void to::test<4>()
{
    std::vector<int> ids = get_all_variables();
    std::map<std::string, int> keys;
    for (std::vector<int>::const_iterator i = ids.begin();
         i != ids.end(); ++i) {
        dballe::Record record;
        get_variable_unique_keys(*i, record);
        std::stringstream ss;
        ss  << record.get(dballe::DBA_KEY_VAR, "-") << ","
            << record.get(dballe::DBA_KEY_PINDICATOR, "-") << ","
            << record.get(dballe::DBA_KEY_P1, "-") << ","
            << record.get(dballe::DBA_KEY_P2, "-") << ","
            << record.get(dballe::DBA_KEY_LEVELTYPE1, "-") << ","
            << record.get(dballe::DBA_KEY_L1, "-") << ","
            << record.get(dballe::DBA_KEY_LEVELTYPE2, "-") << ","
            << record.get(dballe::DBA_KEY_L2, "-");
        std::string key = ss.str();
        ensure(keys.find(key) == keys.end());
        keys[key] = *i;
    }
    ensure_equals(ids.size(), keys.size());
}
// Check for unit conversion
template<> template<>
void to::test<5>()
{
    bool everything_ok = true;

    std::vector<int> ids = get_all_variables();
    for (std::vector<int>::const_iterator i = ids.begin();
         i != ids.end(); ++i) {
        std::string bcode, unit;
        source.lua_push_variable(*i);
        get_item_value("bcode", bcode);
        get_item_value("unit", unit);
        lua_pop(L, 1);
        wreport::Varcode varcode = wreport::descriptor_code(bcode.c_str());
        wreport::Varinfo varinfo = dballe::varinfo(varcode);
        try {
            wreport::Var var(varinfo);
            if (not unit.empty()) {
                var.set(varinfo->imin);
                double v = wreport::convert_units(varinfo->unit, unit.c_str(), var.enqd());
                wreport::convert_units(unit.c_str(), varinfo->unit, v);
            }
        } catch (const std::exception& e) {
            std::cerr << "While inspecting variable " << *i << ": " << e.what() << std::endl;
            everything_ok = false;
        }
    }
    ensure(everything_ok);
}

}
