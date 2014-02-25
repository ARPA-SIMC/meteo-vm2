#include <meteo-vm2/source.h>
#include <meteo-vm2/parser.h>

#include <vector>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <fstream>

#define METEOVM2_ERROR_GENERIC 1
#define METEOVM2_ERROR_ID_NOT_FOUND 2
#define METEOVM2_ERROR_KEY_NOT_FOUND 3
#define METEOVM2_ERROR_TYPE_CONVERSION 4

extern "C" {
#include <f77.h>
}

class cnfstring : public std::string {
 private:
  char* _s;
 public:
  cnfstring(const char* s, size_t l) {
    _s = cnfCreim(s, l);
    assign(_s);
  }
  virtual ~cnfstring() {
    free(_s);
  }
};

class SourceHandler : public std::vector<meteo::vm2::Source*> {
 private:
  int insert_source(meteo::vm2::Source* source) {
    for (int i = 1; i < size(); ++i) {
      if (at(i) == NULL) {
        (*this)[i] = source;
        return i;
      }
    }
    push_back(source);
    return size() - 1;
  }

 public:
  int insert_source_filename(const std::string& filename) {
    meteo::vm2::Source* source = new meteo::vm2::Source(filename);
    return insert_source(source);
  }
  int insert_source_default() {
    meteo::vm2::Source::get();
    return 0;
  }

  meteo::vm2::Source* get_source(int i) {
    if (i == 0) return meteo::vm2::Source::get();
    if (i < 0 || i > size()) {
      throw std::runtime_error("Source not found");
    }
    meteo::vm2::Source* source = at(i);
    if (source == NULL)
      throw std::runtime_error("Source already closed");
    return source;
  }

  void close_source(int i) {
    if (i == 0) return;
    if (i < 0 || i > size())
      throw std::runtime_error("Source not found");
    delete at(i);
    (*this)[i] = NULL;
  }
} sourcehandler;

extern "C" {
 F77_SUBROUTINE(meteovm2_source_open_default)(
    INTEGER(sourcehandle),
    INTEGER(errorcode))
 {
   GENPTR_INTEGER(sourcehandle);
   *sourcehandle = -1;
   *errorcode = 0;

   try {
     *sourcehandle = sourcehandler.insert_source_default();
   } catch (...) {
     *errorcode = 1;
   }
 } 
 F77_SUBROUTINE(meteovm2_source_open_filename)(
     INTEGER(sourcehandle),
     CHARACTER(filename),
     INTEGER(errorcode)
     TRAIL(filename))
 {
   GENPTR_INTEGER(sourcehandle);
   GENPTR_CHARACTER(filename);
   GENPTR_INTEGER(errorcode);
   *sourcehandle = -1;
   *errorcode = 0;

   cnfstring filename_str(filename, filename_length);

   try {
     *sourcehandle = sourcehandler.insert_source_filename(filename_str);
   } catch (...) {
     *errorcode = 1;
   }
 }
 F77_SUBROUTINE(meteovm2_source_close_handle)(
     INTEGER(sourcehandle),
     INTEGER(errorcode))
 {
   GENPTR_INTEGER(sourcehandle);
   GENPTR_INTEGER(errorcode);
   *errorcode = 0;

   try {
     sourcehandler.close_source(*sourcehandle);
   } catch (...) {
     *errorcode = 1;
   }
 }
 F77_SUBROUTINE(meteovm2_get_station_attr_i)(
     INTEGER(sourcehandle),
     INTEGER(id),
     CHARACTER(key),
     INTEGER(value),
     INTEGER(errorcode)
     TRAIL(key))
 {
   GENPTR_INTEGER(sourcehandle);
   GENPTR_INTEGER(id)
   GENPTR_CHARACTER(key);
   GENPTR_INTEGER(value);
   GENPTR_INTEGER(errorcode);
   *errorcode = 0;

   try {
     meteo::vm2::Source *source = sourcehandler.get_source(*sourcehandle);
     lua_State* L = source->L;
     source->lua_push_station(*id);
     if (lua_isnil(L, -1)) {
       lua_pop(L, 1);
       //throw std::runtime_error("Station not found");
       *errorcode = METEOVM2_ERROR_ID_NOT_FOUND;
       return;
     }
     cnfstring key_str(key, key_length);
     lua_getfield(L, -1, key_str.c_str());
     if (lua_isnil(L, -1)) {
       lua_pop(L, 2);
       //throw std::runtime_error("Key not found");
       *errorcode = METEOVM2_ERROR_KEY_NOT_FOUND;
       return;
     }
     if (lua_type(L, -1) != LUA_TNUMBER) {
       lua_pop(L, 2);
       //throw std::runtime_error("Value is not a number");
       *errorcode = METEOVM2_ERROR_TYPE_CONVERSION;
       return;
     }
     *value = lua_tointeger(L, -1);
     lua_pop(L, 2);
   } catch (...) {
     *errorcode = 1;
   }
 }
 F77_SUBROUTINE(meteovm2_get_station_attr_s)(
     INTEGER(sourcehandle),
     INTEGER(id),
     CHARACTER(key),
     CHARACTER(value),
     INTEGER(errorcode)
     TRAIL(key)
     TRAIL(value))
 {
   GENPTR_INTEGER(sourcehandle);
   GENPTR_INTEGER(id)
   GENPTR_CHARACTER(key);
   GENPTR_CHARACTER(value);
   GENPTR_INTEGER(errorcode);
   *errorcode = 0;

   try {
     meteo::vm2::Source *source = sourcehandler.get_source(*sourcehandle);
     lua_State* L = source->L;
     source->lua_push_station(*id);
     if (lua_isnil(L, -1)) {
       lua_pop(L, 1);
       //throw std::runtime_error("Station not found");
       *errorcode = METEOVM2_ERROR_ID_NOT_FOUND;
       return;
     } 
     cnfstring key_str(key, key_length);
     lua_getfield(L, -1, key_str.c_str());
     if (lua_isnil(L, -1)) {
       lua_pop(L, 2);
       //throw std::runtime_error("Key not found");
       *errorcode = METEOVM2_ERROR_KEY_NOT_FOUND;
       return;
     }
     if (lua_type(L, -1) != LUA_TSTRING) {
       lua_pop(L, 2);
       //throw std::runtime_error("Value is not a string");
       *errorcode = METEOVM2_ERROR_TYPE_CONVERSION;
       return;
     }
     cnfExprt(lua_tostring(L, -1), value, value_length);
     lua_pop(L, 2);
   } catch (...) {
     *errorcode = 1;
   }
 }
 F77_SUBROUTINE(meteovm2_get_variable_attr_i)(
     INTEGER(sourcehandle),
     INTEGER(id),
     CHARACTER(key),
     INTEGER(value),
     INTEGER(errorcode)
     TRAIL(key))
 {
   GENPTR_INTEGER(sourcehandle);
   GENPTR_INTEGER(id)
   GENPTR_CHARACTER(key);
   GENPTR_INTEGER(value);
   GENPTR_INTEGER(errorcode);
   *errorcode = 0;

   try {
     meteo::vm2::Source *source = sourcehandler.get_source(*sourcehandle);
     lua_State* L = source->L;
     source->lua_push_variable(*id);
     if (lua_isnil(L, -1)) {
       lua_pop(L, 1);
       //throw std::runtime_error("Variable not found");
       *errorcode = METEOVM2_ERROR_ID_NOT_FOUND;
       return;
     } 
     cnfstring key_str(key, key_length);
     lua_getfield(L, -1, key_str.c_str());
     if (lua_isnil(L, -1)) {
       lua_pop(L, 2);
       //throw std::runtime_error("Key not found");
       *errorcode = METEOVM2_ERROR_KEY_NOT_FOUND;
       return;
     }
     if (lua_type(L, -1) != LUA_TNUMBER) {
       lua_pop(L, 2);
       //throw std::runtime_error("Value is not a number");
       *errorcode = METEOVM2_ERROR_TYPE_CONVERSION;
       return;
     }
     *value = lua_tointeger(L, -1);
     lua_pop(L, 2);
   } catch (...) {
     *errorcode = 1;
   }
 }
 F77_SUBROUTINE(meteovm2_get_variable_attr_s)(
     INTEGER(sourcehandle),
     INTEGER(id),
     CHARACTER(key),
     CHARACTER(value),
     INTEGER(errorcode)
     TRAIL(key)
     TRAIL(value))
 {
   GENPTR_INTEGER(sourcehandle);
   GENPTR_INTEGER(id)
   GENPTR_CHARACTER(key);
   GENPTR_CHARACTER(value);
   GENPTR_INTEGER(errorcode);
   *errorcode = 0;

   try {
     meteo::vm2::Source *source = sourcehandler.get_source(*sourcehandle);
     lua_State* L = source->L;
     source->lua_push_variable(*id);
     if (lua_isnil(L, -1)) {
       lua_pop(L, 1);
       //throw std::runtime_error("Variable not found");
       *errorcode = METEOVM2_ERROR_ID_NOT_FOUND;
       return;
     } 
     cnfstring key_str(key, key_length);
     lua_getfield(L, -1, key_str.c_str());
     if (lua_isnil(L, -1)) {
       lua_pop(L, 2);
       //throw std::runtime_error("Key not found");
       *errorcode = METEOVM2_ERROR_KEY_NOT_FOUND;
       return;
     }
     if (lua_type(L, -1) != LUA_TSTRING) {
       lua_pop(L, 2);
       //throw std::runtime_error("Value is not a string");
       *errorcode = METEOVM2_ERROR_TYPE_CONVERSION;
       return;
     }
     cnfExprt(lua_tostring(L, -1), value, value_length);
     lua_pop(L, 2);
   } catch (...) {
     *errorcode = 1;
   }
 }
}

struct FileParser : public meteo::vm2::Parser {
  std::ifstream infile;

  FileParser(const std::string& filename) : infile(filename.c_str()), meteo::vm2::Parser(infile) {
    if (!infile.good()) throw std::runtime_error("Error while opening input file");
  }

  virtual ~FileParser() { 
    infile.close();
  }
};

class ParserHandler : public std::vector<meteo::vm2::Parser*> {
 private:
  int insert_parser(meteo::vm2::Parser* parser) {
    for (int i = 0; i < size(); ++i) {
      if (at(i) == NULL) {
        (*this)[i] = parser;
        return i;
      }
    }
    push_back(parser);
    return size() - 1;
  }
 public:
  int insert_parser_filename(const std::string& filename) {
    return insert_parser(new FileParser(filename));
  }

  meteo::vm2::Parser* get_parser(int i) {
    if (i < 0 || i > size()) {
      throw std::runtime_error("Parser not found");
    }
    meteo::vm2::Parser* parser = at(i);
    if (parser == NULL)
      throw std::runtime_error("Parser already closed");
    return parser;
  }
  void close_parser(int i) {
    if (i < 0 || i >= size())
      throw std::runtime_error("Parser not found");
    delete at(i);
    (*this)[i] = NULL;
  }
} parserhandler;

extern "C" {
 F77_SUBROUTINE(meteovm2_parser_open_filename)(
     INTEGER(parserhandle),
     CHARACTER(filename),
     INTEGER(errorcode)
     TRAIL(filename))
 {
   GENPTR_INTEGER(parserhandle);
   GENPTR_CHARACTER(filename);
   GENPTR_INTEGER(errorcode);
   *errorcode = 0;
   *parserhandle = -1;

   cnfstring filename_str(filename, filename_length);

   try {
     *parserhandle = parserhandler.insert_parser_filename(filename_str);
   } catch (...) {
     *errorcode = 1;
   }
 }
 F77_SUBROUTINE(meteovm2_parser_close_handle)(
     INTEGER(parserhandle),
     INTEGER(errorcode))
 {
   GENPTR_INTEGER(parserhandle);
   GENPTR_INTEGER(errorcode);
   *errorcode = 0;
   try {
     parserhandler.close_parser(*parserhandle);
   } catch (...) {
     *errorcode = 1;
   }
 }
 F77_SUBROUTINE(meteovm2_parser_next_1)(
     INTEGER(parserhandle),
     LOGICAL(havenext),
     INTEGER(year),
     INTEGER(month),
     INTEGER(day),
     INTEGER(hour),
     INTEGER(min),
     INTEGER(sec),
     INTEGER(station),
     INTEGER(variable),
     REAL(value1),
     REAL(value2),
     CHARACTER(value3),
     CHARACTER(flags),
     INTEGER(errorcode)
     TRAIL(value3)
     TRAIL(flags))
 {
     GENPTR_INTEGER(parserhandle);
     GENPTR_LOGICAL(havenext);
     GENPTR_INTEGER(year);
     GENPTR_INTEGER(month);
     GENPTR_INTEGER(day);
     GENPTR_INTEGER(hour);
     GENPTR_INTEGER(min);
     GENPTR_INTEGER(sec);
     GENPTR_INTEGER(station);
     GENPTR_INTEGER(variable);
     GENPTR_REAL(value1);
     GENPTR_REAL(value2);
     GENPTR_CHARACTER(value3);
     GENPTR_CHARACTER(flags);
     GENPTR_INTEGER(errorcode);
     *errorcode = 0;
     
     try {
       meteo::vm2::Parser* parser = parserhandler.get_parser(*parserhandle);
       meteo::vm2::Value value;
       bool result = parser->next(value);
       if (result) {
         *havenext = F77_TRUE;
         *year = value.year;
         *month = value.month;
         *day = value.mday;
         *hour = value.hour;
         *min = value.min;
         *sec = value.sec;
         *station = value.station_id;
         *variable = value.variable_id;
         *value1 = value.value1;
         *value2 = value.value2;
         cnfExprt(value.value3.c_str(), value3, value3_length);
         cnfExprt(value.flags.c_str(), flags, flags_length);
       } else {
         *havenext = F77_FALSE;
       }
     } catch (...) {
       *errorcode = 1;
     }
 }
}
