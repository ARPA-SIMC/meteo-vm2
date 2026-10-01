/*
 * meteo-vm2-to-bufr - Convert VM2 to generic BUFR
 *
 * Copyright (C) 2012-2014 Arpae-SIMC <simc-urp@arpae.it>
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>

#include <meteo-vm2/parser.h>


static std::string PROGRAM_NAME = "meteo-vm2-to-csv";


void print_usage()
{
    std::cout
        << "Usage: "
        << PROGRAM_NAME
        << " [options] [SOURCEFILE]" << std::endl
        << std::endl
        << "Convert VM2 to CSV." << std::endl
        << "Options:" << std::endl
        << "  --help     show this help and exit" << std::endl
        << "  --version  show version and exit" << std::endl;
}

int main(int argc, const char** argv)
{
    if (argc > 1) {
        if (std::string(argv[1]) == "--help") {
            print_usage();
            return 0;
        }
        if (std::string(argv[1]) == "--version") {
            std::cout << PROGRAM_NAME << " " << PACKAGE_VERSION << std::endl;
            return 0;
        }
    }

    try {
        meteo::vm2::Parser parser(std::cin);
        meteo::vm2::Value value;
        std::string line;
        while (parser.next(value, line)) {
            std::cout
                << std::setfill('0')
                << std::setw(4) << value.year
                << std::setw(2) << value.month
                << std::setw(2) << value.mday
                << std::setw(2) << value.hour
                << std::setw(2) << value.min;
            std::cout << ",";
            std::cout << value.station_id;
            std::cout << ",";
            std::cout << value.variable_id;
            std::cout << ",";
            bool is_valid = true;
            if (value.flags.size() == 9) {
                if (value.flags[0] != '0')
                    is_valid = false;
                if (value.flags.substr(1,2) != "00")
                    is_valid = false;
            }
            if (is_valid)
                std::cout << value.value1;
            std::cout << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
