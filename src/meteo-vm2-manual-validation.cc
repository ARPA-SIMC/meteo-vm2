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

// VALIDATION_FLAG,STATION_ID,VARIABLE_ID,START_DATE,END_DATE
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

    void modify_validation_flag(meteo::vm2::Value& value) {
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
                    set_validation_flag(value, record);
                }
            }
        }
    }

    void set_validation_flag(meteo::vm2::Value& value, const ValidationRecord& record) {
        if (value.flags.empty())
            value.flags = "000000000";
        char flag = value.flags.at(0);
        // If the right flag is already set, do nothing
        if ((record.is_invalid and flag == '1') or
            (not record.is_invalid and flag == '0')) {
            return;
        }
        // Revalidation can be applied to invalid data only
        else if (not record.is_invalid and flag != '1') {
            return;
        }
        // Revalidation of this kind of value means that the flag must be set to 2
        else if (not record.is_invalid and flag == '1' and value.value2 !=  meteo::vm2::MISSING_DOUBLE and
                 (value.value3.empty() or value.value3 == "S" or value.value3 == "R")) {
            value.flags[0] = '2';
        }
        // In this case, value3 is the level of the measure: the flag can be updated
        else if (not record.is_invalid and not value.value3.empty() and
                 value.variable_id >= 522 and value.variable_id <= 535) {
            value.flags[0] = (record.is_invalid? '1': '0');
        }
        // The last case is the precipitation over more than one period: the flag is not updated
        else if (not record.is_invalid and flag == '1' and not value.value3.empty()) {
            return;
        }
        // Finally
        else {
            value.flags[0] = (record.is_invalid? '1': '0');
        }
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
            table.modify_validation_flag(value);
            meteo::vm2::Parser::serialize(std::cout, value);
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
