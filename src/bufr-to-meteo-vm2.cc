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
#include <dballe/core/file.h>
#include <dballe/msg/msgs.h>
#include <dballe/msg/msg.h>
#include <dballe/msg/context.h>
#include <dballe/msg/wr_codec.h>

#include <meteo-vm2/source.h>

static void show_usage(std::ostream& out)
{
  out << "Usage: bufr-to-meteo-vm2 [SOURCEFILE]" << std::endl
      << std::endl
      << "Read a generic BUFR file from stdin and print" << std::endl
      << "a VM2 file to stdout." << std::endl;
}

static inline std::string convert_qc_back(int qc)
{
  int qcint = (int)rint((100.0 - qc) * 6.0 / 100.0 + 48);
  return wibble::str::fmtf("%02d", qcint);
}

int main(int argc, const char** argv)
{
  try {
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

    lua_State* L = source->L;
    dballe::msg::BufrImporter importer;

    std::auto_ptr<dballe::File> infile = dballe::File::create(dballe::BUFR, 
                                                              "(stdin)", "r");

    dballe::Rawmsg raw;

    while (infile->read(raw)) {
      dballe::Msgs msgs;
      importer.from_rawmsg(raw, msgs);

      for (dballe::Msgs::const_iterator mi = msgs.begin();
           mi != msgs.end(); ++mi) {
        int idx;
        meteo::vm2::Value vm2value;
        const dballe::Msg& msg = **mi;
        const dballe::msg::Context* sta_ctx = msg.find_station_context();
        if (!sta_ctx) {
          std::cerr << "Cannot find station context" << std::endl;
          continue;
        }
        // station
        lua_newtable(L);
        idx = lua_gettop(L);
        // Station lookup by lon, lat, rep_memo
        if (const wreport::Var *v = sta_ctx->find(WR_VAR(0, 6, 1))) {
            lua_pushnumber(L, v->enqi());
            lua_setfield(L, idx, "lon");
        } else {
            std::cerr << "Missing longitude" << std::endl;
            continue;
        }
        if (const wreport::Var *v = sta_ctx->find(WR_VAR(0, 5, 1))) {
            lua_pushnumber(L, v->enqi());
            lua_setfield(L, idx, "lat");
        } else {
            std::cerr << "Missing latitude" << std::endl;
            continue;
        }
        if (const wreport::Var *v = sta_ctx->find(WR_VAR(0, 1,194))) {
            lua_pushstring(L, v->enqc());
            lua_setfield(L, idx, "rep");
        } else {
            std::cerr << "Missing rep_memo" << std::endl;
            continue;
        }
        if (const wreport::Var *v = sta_ctx->find(WR_VAR(0, 1, 11))) {
            lua_pushstring(L, v->enqc());
            lua_setfield(L, idx, "ident");
        } else {
            std::cerr << "Missing ident" << std::endl;
            continue;
        }
#if 0
        for (std::vector<wreport::Var*>::const_iterator vi = msg.find_station_context()->data.begin();
             vi != msg.find_station_context()->data.end(); ++vi) {
          if (*vi == msg.get_year_var()
              || *vi == msg.get_month_var()
              || *vi == msg.get_day_var()
              || *vi == msg.get_hour_var()
              || *vi == msg.get_minute_var()
              || *vi == msg.get_second_var()) continue;
          wreport::Var& v = **vi;
          if (v.info()->is_string()) {
            lua_pushstring(L, v.enqc());
          } else {
            lua_pushnumber(L, v.enqd());
          }
          lua_setfield(L, idx, dballe::format_code(v.code()).c_str());

        }
#endif
        std::vector<int> stations = source->lua_find_stations(idx);
        lua_settop(L, idx);
        if (stations.size() == 0) {
          std::cerr << "cannot find station" << std::endl;
          continue;
        }
        if (stations.size() > 1) {
          std::cerr << stations.size() 
              << " stations with same attributes"
              << " ("
              << wibble::str::join(stations.begin(), stations.end())
              << ")"
              << std::endl;
          continue;
        }
        vm2value.station_id = stations.at(0);
        vm2value.year = msg.get_year_var()->enqi();
        vm2value.month = msg.get_month_var()->enqi();
        vm2value.mday = msg.get_day_var()->enqi();
        vm2value.hour = msg.get_hour_var()->enqi();
        vm2value.min = msg.get_minute_var()->enqi();
        if (msg.get_second_var()) {
          vm2value.sec = msg.get_second_var()->enqi();
        } else {
          vm2value.sec = 0;
        }

        for (std::vector<dballe::msg::Context*>::const_iterator ci = msg.data.begin();
             ci != msg.data.end(); ++ci) {
          if (msg.find_station_context() == *ci) continue;
          dballe::msg::Context& c = **ci;
          for (std::vector<wreport::Var*>::const_iterator vi = c.data.begin();
               vi != c.data.end(); ++vi) {
            wreport::Var& v = **vi;
            // variable
            lua_newtable(L);
            idx = lua_gettop(L);
            lua_pushstring(L, dballe::format_code(v.code()).c_str());
            lua_setfield(L, idx, "bcode");
            lua_pushinteger(L, c.trange.pind);
            lua_setfield(L, idx, "tr");
            lua_pushinteger(L, c.trange.p1);
            lua_setfield(L, idx, "p1");
            lua_pushinteger(L, c.trange.p2);
            lua_setfield(L, idx, "p2");
            lua_pushinteger(L, c.level.ltype1);
            lua_setfield(L, idx, "lt1");
            if (c.level.l1 != dballe::MISSING_INT) {
              lua_pushinteger(L, c.level.l1);
              lua_setfield(L, idx, "l1");
            }
            if (c.level.ltype2 != dballe::MISSING_INT) {
              lua_pushinteger(L, c.level.ltype2);
              lua_setfield(L, idx, "lt2");
            }
            if (c.level.l2 != dballe::MISSING_INT) {
              lua_pushinteger(L, c.level.l2);
              lua_setfield(L, idx, "l2");
            }
            std::vector<int> variables = source->lua_find_variables(idx);
            lua_settop(L, idx);
            if (variables.size() == 0) {
              std::cerr << "cannot find variable " << dballe::format_code(v.code()) << std::endl;
              continue;
            }
            if (variables.size() > 1) {
                std::cerr << variables.size()
                    << " variables with same attributes"
                    << " ("
                    << wibble::str::join(variables.begin(), variables.end())
                    << ")"
                    << std::endl;
              continue;
            }
            vm2value.variable_id = variables.at(0);

            source->lua_push_variable(vm2value.variable_id);
            lua_getfield(L, -1, "unit");
            if (!lua_isnil(L, -1)) {
              vm2value.value1 = wreport::convert_units(v.info()->unit,
                                                       lua_tostring(L, -1),
                                                       v.enqd());
            } else {
              vm2value.value1 = v.enqd();
            }
            lua_pop(L, 1);

            vm2value.value2 = meteo::vm2::MISSING_DOUBLE;
            vm2value.value3 = "";
            // TODO: flags
            const wreport::Var* vattr = v.next_attr();
            if (vattr == NULL) {
              vm2value.flags = "";
            } else {
              vm2value.flags = "000000000";
              for (vattr = v.next_attr(); vattr != NULL; 
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
          }
        }
      }
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}
