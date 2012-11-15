/*
 * parser - Parse VM2 files
 *
 * Copyright (C) 2012  ARPA-SIM <urpsim@smr.arpa.emr.it>
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
#ifndef VM2_PARSER_H
#define VM2_PARSER_H

/**
 * @file
 * @page VM2ValueFile VM2 value file
 *
 * A VM2 file contains a list of values in CSV format with the following 
 * columns:
 *  - REFTIME: reference time (YYYYmmddHHMM) if the value (in UTC)
 *  - STATION ID: integer
 *  - VARIABLE ID id: integer
 *  - VALUE 1: double (empty if missing)
 *  - VALUE 2: double (empty if missing)
 *  - VALUE 3: string (without comma)
 *  - FLAGS: string (without comma)
 *
 * E.g.
 *
 * @code
 * 201201010000,1,2,4.56,7.8,X,000000000
 * 201201010030,1,2,12.0,9.4,X,000000000
 * @endcode
 *
 * Read VM2 values from stdin
 *
 * @code
 *
 * vm2::Parser parser(std::cin);
 * vm2::Value value;
 * while (parser.next(value)) {
 *  // ...
 * }
 *
 * @endcode
 *
 * Serialize a VM2 file
 *
 * @code
 *
 * vm2::Value value;
 * // Populate the value
 * value.year = 2012;
 * value.month = 1;
 * // ...
 * value.station_id = 123;
 * // ...
 * // Serialize
 * vm2::Parser::serialize(std::cout, value);
 *
 * @endcode
 */

#include <string>
#include <vector>
#include <istream>
#include <ostream>

#include <vm2/value.h>

namespace vm2 {

/**
 * Parser for the VM2 value files
 */
struct Parser {
  /// Pattern of a VM2 value
  static std::string regexp_str;

  std::istream& in;
  /// Number of the last line parsed
  int lineno;

  Parser(std::istream& in);
  ~Parser();

  /// Store the next VM2 message in value
  bool next(Value& value);

  /**
   * Store the next VM2 message in value, and the raw message in \a raw
   */
  bool next(Value& value, std::string& raw);

  /// Serialize a value
  static void serialize(std::ostream& out, const Value& value);
};

}

#endif        /* VM2_PARSER_H */
