/*
 * value - value class
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
#include "config.h"

#include <stdexcept>
#include <iostream>

#include <meteo-vm2/value.h>
#include <yajl/yajl_tree.h>

struct YajlValRAII {
    yajl_val node = nullptr;
    ~YajlValRAII() {
        if (node)
            yajl_tree_free(node);
    }
};

namespace meteo{
namespace vm2 {
std::map<std::string, std::string> Value::jsonflags() const {
#ifdef HAVE_YAJL
    std::map<std::string, std::string> f;

    YajlValRAII raii;
    yajl_val node = raii.node;
    
    node = yajl_tree_parse(flags.c_str(), NULL, 0);
    if (node == nullptr || not YAJL_IS_OBJECT(node))
        throw std::runtime_error("Invalid JSON");
    for (size_t i = 0; i < YAJL_GET_OBJECT(node)->len; ++i) {
        const char* k = YAJL_GET_OBJECT(node)->keys[i];
        const char* v = YAJL_GET_STRING((YAJL_GET_OBJECT(node)->values[i]));
        if (v)
            f[k] = v;
    }
    return f;
#else
    throw std::runtime_error("Not supported");
#endif
}
}
}
