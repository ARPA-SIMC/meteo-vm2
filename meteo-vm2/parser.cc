/*
 * parser - Parse VM2 files
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
#include <meteo-vm2/parser.h>

#include <stdlib.h>

#include <wibble/exception.h>
#include <wibble/string.h>
#include <wibble/regexp.h>

namespace meteo {
namespace vm2 {

std::string Parser::regexp_str = "^[0-9]{12}([0-9][0-9])?,[0-9]+,[0-9]+,[+-]?[0-9.]*,[+-]?[0-9.]*,[^,\n\r]*,[^,\n\r]*[\r\n]*$";

static wibble::Regexp regexp(Parser::regexp_str, 0, REG_EXTENDED);

Parser::Parser(std::istream& in) : in(in), lineno(0) {}
Parser::~Parser() {}

bool Parser::next(Value& value) {
  std::string line;
  return next(value, line);
}

bool Parser::next(Value& value, std::string& line) {
  char c;
  line.clear();
  while (true) {
    in.get(c);
    if (!in.good())
      if (line.size() == 0)
        return false;
      else
        break;
    if (c == '\r')
      continue;
    if (c == '\n')
      break;
    line.append(1, c);
  }
  ++lineno;

  if (!regexp.match(line))
    throw wibble::exception::Consistency(
        wibble::str::fmtf("reading line %d", lineno),
        "pattern mismatch");

  wibble::str::Split splitter(",", line);
  wibble::str::Split::const_iterator i = splitter.begin();
  if (i == splitter.end())
    throw wibble::exception::Consistency(
        wibble::str::fmtf("reading line %d", lineno),
        "line does not contain a date field");

  value.sec = 0;
  if (sscanf(i->c_str(),
             "%04d%02d%02d%02d%02d%02d",
             &value.year, &value.month, &value.mday,
             &value.hour, &value.min, &value.sec) < 5) {
    throw wibble::exception::Consistency(
        wibble::str::fmtf("reading line %d", lineno),
        "invalid date format");
  }

  ++i;
  if (i == splitter.end())
    throw wibble::exception::Consistency(
        wibble::str::fmtf("reading line %d", lineno),
        "line does not contain a station id field");
  value.station_id = strtoul(i->c_str(), NULL, 10);

  ++i;
  if (i == splitter.end())
    throw wibble::exception::Consistency(
        wibble::str::fmtf("reading line %d", lineno),
        "line does not contain a variable id field");
  value.variable_id = strtoul(i->c_str(), NULL, 10);
  
  ++i;
  if (i == splitter.end())
    throw wibble::exception::Consistency(
        wibble::str::fmtf("reading line %d", lineno),
        "line does not contain a value1 field");
  if (i->empty()) {
    value.value1 = vm2::MISSING_DOUBLE;
  } else {
    value.value1 = strtod(i->c_str(), NULL);
  }

  ++i;
  if (i == splitter.end())
    throw wibble::exception::Consistency(
        wibble::str::fmtf("reading line %d", lineno),
        "line does not contain a value2 field");
  if (i->empty()) {
    value.value2 = vm2::MISSING_DOUBLE;
  } else {
    value.value2 = strtod(i->c_str(), NULL);
  }

  ++i;
  if (i == splitter.end())
    throw wibble::exception::Consistency(
        wibble::str::fmtf("reading line %d", lineno),
        "line does not contain a value3 field");
  value.value3 = *i;

  ++i;
  if (i != splitter.end())
    value.flags = *i;

  return true;
}

void Parser::serialize(std::ostream& out, const Value& value) {
  out << wibble::str::fmtf("%04d%02d%02d%02d%02d%02d,",
                           value.year, value.month, value.mday,
                           value.hour, value.min, value.sec)
      << value.station_id << ","
      << value.variable_id << ",";
  if (value.value1 != vm2::MISSING_DOUBLE)
    out << value.value1;
  out << ",";
  if (value.value2 != vm2::MISSING_DOUBLE)
    out << value.value2;
  out << ","
      << value.value3 << ","
      << value.flags << std::endl;
}

}
}
