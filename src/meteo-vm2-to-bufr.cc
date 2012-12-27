#include <math.h>

#include <iostream>
#include <fstream>
#include <cstring>

#include <wibble/string.h>
#include <wreport/varinfo.h>
#include <wreport/conv.h>
#include <meteo-vm2/source.h>
#include <meteo-vm2/parser.h>
#include <dballe/core/defs.h>
#include <dballe/core/var.h>
#include <dballe/msg/msgs.h>
#include <dballe/msg/msg.h>
#include <dballe/msg/wr_codec.h>

static void show_usage(std::ostream& out)
{
  out << "Usage: meteo-vm2-to-bufr [SOURCEFILE]" << std::endl
      << std::endl
      << "Read a VM2 file from stdin and print" << std::endl
      << "a generic BUFR file to stdout." << std::endl;
}

static inline int convert_qc(const std::string& str)
{
  int qc = atoi(str.c_str());
  return (int)rint(100.0 - ((qc-48.0) * 100.0 / 6.0));
}

int main(int argc, const char** argv)
{
  meteo::vm2::Source* source = NULL;

  if (argc > 1 
      && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
    show_usage(std::cerr);
    return 0;
  }
  if (argc == 1)
    source = meteo::vm2::Source::get();
  else
    source = new meteo::vm2::Source(argv[1]);

  meteo::vm2::Parser parser(std::cin);

  lua_State* L = source->L;
  meteo::vm2::Value value;
  dballe::msg::BufrExporter exporter;

  while (parser.next(value)) {
    int top = lua_gettop(L);

    try {
      dballe::Msgs msgs;
      dballe::Msg msg;
      // date
      msg.set_year(value.year);
      msg.set_month(value.month);
      msg.set_day(value.mday);
      msg.set_hour(value.hour);
      msg.set_minute(value.min);
      msg.set_second(value.sec);
      // station
      source->lua_push_station(value.station_id);
      if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        throw std::runtime_error(
            wibble::str::fmtf("line %d: station %d not found",
                              parser.lineno, value.station_id));
      }
      lua_pushnil(L);
      while(lua_next(L, -2)) {
        std::string bcode = lua_tostring(L, -2);
        wreport::Varcode varcode = wreport::descriptor_code(bcode.c_str());
        if (lua_isnumber(L, -1)) {
          msg.set(dballe::var(varcode, lua_tonumber(L, -1)), varcode, 
                              dballe::Level(257), dballe::Trange());
        } else {
          msg.set(dballe::var(varcode, lua_tostring(L, -1)), varcode, 
                              dballe::Level(257), dballe::Trange());
        }
        lua_pop(L,1);
      }
      lua_pop(L, 1);
      // variable
      source->lua_push_variable(value.variable_id);
      if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        throw std::runtime_error(
            wibble::str::fmtf("line %d: variable %d not found",
                              parser.lineno, value.variable_id));
      }
      lua_getfield(L, -1, "bcode");
      wreport::Varcode varcode = wreport::descriptor_code(lua_tostring(L, -1));
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
      std::string unit = lua_tostring(L, -1);
      lua_pop(L, 1);

      double val = wreport::convert_units(unit.c_str(), 
                                          dballe::varinfo(varcode)->unit, 
                                          value.value1);
      wreport::Var var = dballe::var(varcode, val);
      if (value.flags.size() == 9) {
        if (value.flags[0] == '1') {
          var.seta(dballe::newvar(WR_VAR(0, 33, 196), 1));
        }
        if (value.flags[0] == '2') {
          var.seta(dballe::newvar(WR_VAR(0, 33, 197), 1));
        }
        if (value.flags.substr(1,2) != "00") {
          var.seta(dballe::newvar(WR_VAR(0, 33, 192), convert_qc(value.flags.substr(1,2))));
        }
        if (value.flags.substr(3,2) != "00") {
          var.seta(dballe::newvar(WR_VAR(0, 33, 193), convert_qc(value.flags.substr(3,2))));
        }
        if (value.flags.substr(5,2) != "00") {
          var.seta(dballe::newvar(WR_VAR(0, 33, 194), convert_qc(value.flags.substr(5,2))));
        }
      }

      msg.set(var, varcode, level, trange);

      msgs.acquire(msg);

      dballe::Rawmsg raw;
      exporter.to_rawmsg(msgs, raw);

      std::cout << raw;
    } catch (std::exception& e) {
      std::cerr << "line " << parser.lineno << ": " << e.what() << std::endl;
    }

    lua_settop(L, top);
  }

  return 0;
}
