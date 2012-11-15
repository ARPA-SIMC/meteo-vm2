/*
 * value - value class
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
#ifndef VM2_VALUE_H
#define VM2_VALUE_H

/**
 * @file
 */
#include <string>
#include <limits>

namespace vm2 {

static double MISSING_DOUBLE = std::numeric_limits<double>::max();

/**
 * VM2 value. 
 *
 * For VM2 value parsing, see @ref FormatOfVM2ValueFile.
 * For VM2 value attributes, see @ref FormatOfVM2AttributesFile.
 */
struct Value {
  int year;
  int month;
  int mday;
  int hour;
  int min;
  int sec;

  unsigned int station_id;
  unsigned int variable_id;

  double value1;
  double value2;
  std::string value3;
  std::string flags;
};

}

#endif        /* VM2_VALUE_H */
