/*
 * meteo-vm2-manual-validation - Set B33196 flag
 *
 * Copyright (C) 2018 Arpae-SIMC <simc-urp@arpae.it>
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

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <regex>

#include <meteo-vm2/parser.h>


struct ValidationRecord {
    static std::regex regexp;

    unsigned int station_id;
    unsigned int variable_id;
    bool is_invalid;
    std::string from_time;
    std::string until_time;

    ValidationRecord() {}

    static ValidationRecord parse_line(const std::string& line) {
        ValidationRecord record;
        std::smatch match;

        if (not regex_match(line, match, regexp)) {
            throw std::runtime_error("pattern mismatch");
        }
        record.is_invalid = strtoul(match.str(1).c_str(), NULL, 10);
        record.station_id = strtoul(match.str(2).c_str(), NULL, 10);
        record.variable_id = strtoul(match.str(3).c_str(), NULL, 10);
        record.from_time = match.str(4);
        record.until_time = match.str(5);

        return record;
    }
};

std::regex ValidationRecord::regexp("^([01]),([0-9]+),([0-9]+),([0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}),([0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2})?$");

struct ValidationTable {
    std::map<int, std::vector<ValidationRecord>> table;

    static ValidationTable parse_file(const std::string& filename) {
        ValidationTable table;
        std::ifstream file(filename);
        file.exceptions(std::ifstream::badbit);
        std::string line;
        while (std::getline(file, line)) {
            ValidationRecord record = ValidationRecord::parse_line(line);
            table.table[record.station_id].push_back(record);
        }
        return table;
    }

    void modify_value(meteo::vm2::Value& value) {
        auto records = table.find(value.station_id);
        if (records != table.end()) {
            for (auto iter = (*records).second.rbegin();
                 iter != (*records).second.rend(); iter++) {
                const ValidationRecord& record = *iter;
                char reftime[17] = {'\0'};
                sprintf(reftime, "%04d-%02d-%02d %02d:%02d",
                        value.year, value.month, value.mday, value.hour, value.min);
                std::string reftime_str(reftime);

                if ((record.variable_id == 999 ||
                     record.variable_id == value.variable_id) &&
                    (record.from_time <= reftime_str && (record.until_time >= reftime_str || record.until_time == ""))) {
                    std::cerr << "Found" << std::endl;
                    return;
                }
            }
        }
        std::cerr << "Not found" << std::endl;
    }
};


void print_usage()
{
    std::cout
        << "Usage: meteo-vm2-manual-validation [options] VALIDATION_TABLE" << std::endl
        << std::endl
        << "Read VM2 from stdin and print VM2 to stdout with flags modified according "
        << "to VALIDATION TABLE." << std::endl
        << "Options:" << std::endl
        << "  -h, --help  show this help and exit" << std::endl
        << "  --version   show version and exit" << std::endl;
}


int main(int argc, const char** argv)
{
    std::string table_filename;
    ValidationTable table;

    if (argc > 1) {
        if (std::string(argv[1]) == "-h" or std::string(argv[1]) == "--help") {
            print_usage();
            return 0;
        }
        else if (std::string(argv[1]) == "--version") {
            std::cout << "meteo-vm2-manual-validation " PACKAGE_VERSION << std::endl;
            return 0;
        }
        else
            table_filename = argv[1];
    } else {
        print_usage();
        return 1;
    }
    try {
        table = ValidationTable::parse_file(table_filename);
        meteo::vm2::Parser parser(std::cin);
        meteo::vm2::Value value;
        std::string line;
        while (parser.next(value, line)) {
            std::cerr << "Inspecting " << line << std::endl;
            table.modify_value(value);
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
