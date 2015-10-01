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

#include <cstdlib>
#include <iomanip>
#include <regex>


namespace meteo {
namespace vm2 {


class PatternException : public std::runtime_error {
public:
    PatternException(int lineno, const std::string& msg) : std::runtime_error("line " + std::to_string(lineno) + ": " + msg) {}
};

static std::regex regexp("^([0-9]{12}([0-9][0-9])?),([0-9]+),([0-9]+),([+-]?[0-9.]*),([+-]?[0-9.]*),([^,\n\r]*),([^,\n\r]*[\r\n]*)$");

Parser::Parser(std::istream& in) : in(in), lineno(0) {}
Parser::~Parser() {}

bool Parser::next(Value& value) {
  std::string line;
  return next(value, line);
}

bool Parser::next(Value& value, std::string& line) {
  char c;
  std::smatch match;

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

  if (not regex_match(line, match, regexp))
    throw PatternException(lineno, "pattern mismatch");

  // match[0]: line
  // match[1]: datetime
  // match[2]: seconds (optional)
  // match[3]: station id
  // match[4]: variable id
  // match[5]: value1
  // match[6]: value2
  // match[7]: value3
  // match[8]: flags

  value.sec = 0;
  sscanf(match.str(1).c_str(), "%04d%02d%02d%02d%02d%02d",
         &value.year, &value.month, &value.mday,
         &value.hour, &value.min, &value.sec);
  value.station_id = strtoul(match.str(3).c_str(), NULL, 10);
  value.variable_id = strtoul(match.str(4).c_str(), NULL, 10);
  value.value1 = (match.str(5).empty() ? vm2::MISSING_DOUBLE : strtod(match.str(5).c_str(), NULL));
  value.value2 = (match.str(6).empty() ? vm2::MISSING_DOUBLE : strtod(match.str(6).c_str(), NULL));
  value.value3 = match.str(7);
  value.flags = match.str(8);

  return true;
}

void Parser::serialize(std::ostream& out, const Value& value) {
  out << std::setfill('0') << std::setw(4) << value.year
      << std::setfill('0') << std::setw(2) << value.month
      << std::setfill('0') << std::setw(2) << value.mday
      << std::setfill('0') << std::setw(2) << value.hour
      << std::setfill('0') << std::setw(2) << value.min
      << std::setfill('0') << std::setw(2) << value.sec
      << ","
      << value.station_id
      << ","
      << value.variable_id
      << ","
      << (value.value1 != vm2::MISSING_DOUBLE ? std::to_string(value.value1) : "")
      << ","
      << (value.value2 != vm2::MISSING_DOUBLE ? std::to_string(value.value2) : "")
      << ","
      << value.value3 << ","
      << value.flags << std::endl;
}

}
}
