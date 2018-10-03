/*
 * parser - Parse VM2 files
 *
 * Copyright (C) 2012,2013 Arpae-SIMC <simc-urp@arpae.it>
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
 * Author: Emanuele Di Giacomo <edigiacomo@arpae.it>
 */
#ifndef METEO_VM2_PARSER_H
#define METEO_VM2_PARSER_H
/**
 * @file
 * @brief Parse VM2 files
 * @see @ref VM2ValueFile
 */

#include <string>
#include <vector>
#include <istream>
#include <ostream>

#include <meteo-vm2/value.h>

namespace meteo {
namespace vm2 {


/**
 * Parser exception raised by VM2 parser
 */
class ParserException : public std::runtime_error {
public:
    ParserException(int lineno, const std::string& msg);
};


/**
 * Parser for the VM2 value files
 */
struct Parser {
  /// Pattern of a VM2 value
  static std::string regexp_str;
  /// Input stream
  std::istream& in;
  /// Number of the last line parsed
  int lineno;

  /// Create a stdin parser
  Parser(std::istream& in);
  virtual ~Parser();

  /// Read the next VM2 value
  bool next(Value& value);

  /// Read the next VM2 value and store the raw message (w/o trailing newline)
  bool next(Value& value, std::string& raw);

  /// Serialize a value
  static void serialize(std::ostream& out, const Value& value);
};

}
}

#endif        /* METEO_VM2_PARSER_H */
