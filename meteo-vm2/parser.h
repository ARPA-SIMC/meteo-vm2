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
 * Parser for the VM2 value files
 */
struct Parser {
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
