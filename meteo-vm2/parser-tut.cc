/*
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

#include <wibble/tests.h>
#include <wibble/exception.h>
#include "parser.h"

#define ensure_throws(x) do { try { x; ensure(false); } catch (wibble::exception::Generic& e) { } } while (0)

namespace tut {

struct meteo_vm2_parser_shar {
};
TESTGRP(meteo_vm2_parser);

// Parse a well-formed VM2 message
template<> template<>
void to::test<1>()
{
  std::string line("201201020300,123,456,78.9,,,000000000\n");
  std::stringstream in(line);
  meteo::vm2::Parser parser(in);

  meteo::vm2::Value value;
  ensure(parser.next(value));
  ensure_equals(in.tellg(), line.size());
  ensure_equals(value.year, 2012);
  ensure_equals(value.month, 1);
  ensure_equals(value.mday, 2);
  ensure_equals(value.hour, 3);
  ensure_equals(value.min, 0);
  ensure_equals(value.sec, 0);
  ensure_equals(value.station_id, 123);
  ensure_equals(value.variable_id, 456);
  ensure_equals(value.value1, 78.9);
  ensure_equals(value.value2, meteo::vm2::MISSING_DOUBLE);
  ensure_equals(value.flags, "000000000");
}
// Parse a malformed VM2 message
template<> template<>
void to::test<2>()
{
  std::string line("201201020300,123,456,78.9,,");
  std::stringstream in(line);
  meteo::vm2::Parser parser(in);

  meteo::vm2::Value value;
  ensure_throws(parser.next(value));
}
// Serializer
template<> template<>
void to::test<3>()
{
  std::string line("20120102030000,123,456,78.9,-5.3,,\n");
  std::stringstream in(line);
  meteo::vm2::Parser parser(in);

  meteo::vm2::Value value;
  ensure(parser.next(value));

  std::stringstream out;
  meteo::vm2::Parser::serialize(out, value);
  ensure_equals(out.str().substr(0,out.str().size()-1),line.substr(0,line.size()-1));
}
// Parse a well-formed VM2 message (with seconds)
template<> template<>
void to::test<4>()
{
  std::string line("20120102030058,123,456,78.9,,,000000000\n");
  std::stringstream in(line);
  meteo::vm2::Parser parser(in);

  meteo::vm2::Value value;
  ensure(parser.next(value));
  ensure_equals(in.tellg(), line.size());
  ensure_equals(value.year, 2012);
  ensure_equals(value.month, 1);
  ensure_equals(value.mday, 2);
  ensure_equals(value.hour, 3);
  ensure_equals(value.min, 0);
  ensure_equals(value.sec, 58);
  ensure_equals(value.station_id, 123);
  ensure_equals(value.variable_id, 456);
  ensure_equals(value.value1, 78.9);
  ensure_equals(value.value2, meteo::vm2::MISSING_DOUBLE);
  ensure_equals(value.flags, "000000000");
}

}