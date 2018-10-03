/*
 * Copyright (C) 2012-2018 Arpae-SIMC <simc-urp@arpae.it>
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
#include "meteo-vm2/utils/tests.h"
#include "meteo-vm2/parser.h"
#include "parser.h"

#define ensure_throws(x) do { try { x; ensure(false); } catch (wibble::exception::Generic& e) { } } while (0)
#define wassert_equals(x, y) do { wassert_true(x == y); } while (0)
using namespace meteo::vm2::utils::tests;

namespace {
class Tests : public TestCase
{
    using TestCase::TestCase;
    // Parse a well-formed VM2 message
    void register_tests() override
    {
        add_method("parse-well-formed-VM2", []() {
            std::string line("201201020300,123,456,78.9,,,000000000\n");
            std::stringstream in(line);
            meteo::vm2::Parser parser(in);

            meteo::vm2::Value value;
            std::string l;
            wassert(parser.next(value, l));
            wassert_equals(in.tellg(), line.size());
            wassert_equals(l.size(), line.substr(0, 37).size());
            wassert_equals(value.year, 2012);
            wassert_equals(value.month, 1);
            wassert_equals(value.mday, 2);
            wassert_equals(value.hour, 3);
            wassert_equals(value.min, 0);
            wassert_equals(value.sec, 0);
            wassert_equals(value.station_id, 123);
            wassert_equals(value.variable_id, 456);
            wassert_equals(value.value1, "78.9");
            wassert_equals(value.value2, "");
            wassert_equals(value.flags, "000000000");
        });
        // Parse a malformed VM2 message
        add_method("parse-malformed-VM2", []() {
            std::string line("201201020300,123,456,78.9,,");
            std::stringstream in(line);
            meteo::vm2::Parser parser(in);

            meteo::vm2::Value value;
            wassert_throws(meteo::vm2::ParserException, parser.next(value));
        });
        // Parse a well-formed VM2 message (with seconds)
        add_method("parse-well-formed-VM2-with-secs", []() {
            std::string line("20120102030058,123,456,78.9,,,000000000\n");
            std::stringstream in(line);
            meteo::vm2::Parser parser(in);

            meteo::vm2::Value value;
            wassert(parser.next(value));
            wassert_equals(in.tellg(), line.size());
            wassert_equals(value.year, 2012);
            wassert_equals(value.month, 1);
            wassert_equals(value.mday, 2);
            wassert_equals(value.hour, 3);
            wassert_equals(value.min, 0);
            wassert_equals(value.sec, 58);
            wassert_equals(value.station_id, 123);
            wassert_equals(value.variable_id, 456);
            wassert_equals(value.value1, "78.9");
            wassert_equals(value.value2, "");
            wassert_equals(value.flags, "000000000");
        });
        // Parse two well-formed VM2 message (emulates a line parser)
        add_method("parse-two-well-formed-VM2", []() {
            std::string line1 = "20120102030058,123,456,78.9,,,100000000\n";
            std::string line2 = "20120102030058,123,456,78.9,,,\n";
            std::string line = line1 + line2;

            std::stringstream in(line);
            meteo::vm2::Parser parser(in);

            meteo::vm2::Value value;
            wassert(parser.next(value));
            wassert_equals(in.tellg(), line1.size());
            wassert_equals(value.year, 2012);
            wassert_equals(value.month, 1);
            wassert_equals(value.mday, 2);
            wassert_equals(value.hour, 3);
            wassert_equals(value.min, 0);
            wassert_equals(value.sec, 58);
            wassert_equals(value.station_id, 123);
            wassert_equals(value.variable_id, 456);
            wassert_equals(value.value1, "78.9");
            wassert_equals(value.value2, "");
            wassert_equals(value.flags, "100000000");

            wassert(parser.next(value));
            wassert_equals(in.tellg(), line1.size() + line2.size());
            wassert_equals(value.year, 2012);
            wassert_equals(value.month, 1);
            wassert_equals(value.mday, 2);
            wassert_equals(value.hour, 3);
            wassert_equals(value.min, 0);
            wassert_equals(value.sec, 58);
            wassert_equals(value.station_id, 123);
            wassert_equals(value.variable_id, 456);
            wassert_equals(value.value1, "78.9");
            wassert_equals(value.value2, "");
            wassert_equals(value.flags, "");
        });
    }
} tests("parser");

}
