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

#include <string>
#include <vector>
#include <istream>
#include <ostream>

#include <wibble/regexp.h>

#include <vm2/value.h>

namespace vm2 {

struct Parser {
  static std::string regexp_str;
  static wibble::Regexp regexp;

  std::istream& in;
  int lineno;

  Parser(std::istream& in);
  ~Parser();
  bool next(Value& value);

  // Convert VM2 reftime in YYYY-mm-ddTHH:MM:SSZ reftime
  static std::string decode_reftime(std::string s);
  // Convert YYYY-mm-ddTHH:MM:SSZ reftime in VM2 reftime
  static std::string encode_reftime(std::string reftime);
  // Serialize a value
  static void serialize(std::ostream& out, const Value& value);
};

}

#endif        /* VM2_PARSER_H */
