/*
 * value - value class
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
#ifndef METEO_VM2_VALUE_H
#define METEO_VM2_VALUE_H

/**
 * @file
 * @brief Value class
 * @see @ref VM2ValueFile
 */
#include <string>
#include <limits>

namespace meteo {
namespace vm2 {

static double MISSING_DOUBLE = std::numeric_limits<double>::max();

/**
 * VM2 value. 
 *
 * For VM2 value parsing, see @ref VM2ValueFile.
 * For VM2 value attributes, see @ref VM2AttributesFile.
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

  std::string value1;
  std::string value2;
  std::string value3;
  std::string flags;
};

}
}
#endif        /* METEO_VM2_VALUE_H */
