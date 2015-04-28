/*
 * python/meteovm2 - meteo-vm2 python bindings
 *
 * Copyright (C) 2015 ARPA-SIM <urpsim@smr.arpa.emr.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * Author: Emanuele Di Giacomo <edigiacomo@arpa.emr.it>
 */
#include <Python.h>
#include "meteo-vm2/source.h"
#include <iostream>

using namespace meteo::vm2;

typedef struct {
    PyObject_HEAD
    Source* source;
} py_Source;

static void py_Source_dealloc(py_Source* self)
{
    delete self->source;
}

static int py_Source_init(py_Source* self, PyObject* args)
{
    const char* path = 0;

    if (!PyArg_ParseTuple(args, "|s", &path))
        return -1;

    if (path) {
        self->source = new Source(path);
    } else {
        self->source = new Source(source::path());
    }

    return 0;
}

static PyObject* dict_from_lua(lua_State* L)
{
    int top = lua_gettop(L);
    PyObject* d = PyDict_New();
    lua_pushnil(L);
    while(lua_next(L, -2)) {
        const char* key = lua_tostring(L, -2);
        PyObject* value = 0;
        if (lua_isnumber(L, -1)) {
            if (std::string(lua_tostring(L, -1)).find(".") == std::string::npos)
                value = PyInt_FromLong(lua_tointeger(L, -1));
            else
                value = PyFloat_FromDouble(lua_tonumber(L, -1));
        } else if (lua_isstring(L, -1)) {
            value = PyString_FromString(lua_tostring(L, -1));
        } else {
            value = Py_None;
            Py_INCREF(value);
        }
        lua_pop(L, 1);
        PyDict_SetItemString(d, key, value);
        Py_DECREF(value);
    }
    lua_settop(L, top);
    return d;
}

static PyObject* py_Source_get_station(py_Source* self, PyObject* args)
{
    int id;
    lua_State* L = self->source->L;

    if (!PyArg_ParseTuple(args, "i", &id))
        return NULL;

    self->source->lua_push_station(id);
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        Py_RETURN_NONE;
    } else {
        PyObject* d = dict_from_lua(L);
        lua_pop(L, 1);
        return d;
    }
}

static PyObject* py_Source_get_variable(py_Source* self, PyObject* args)
{
    int id;
    lua_State* L = self->source->L;

    if (!PyArg_ParseTuple(args, "i", &id))
        return NULL;

    self->source->lua_push_variable(id);
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        Py_RETURN_NONE;
    } else {
        PyObject* d = dict_from_lua(L);
        lua_pop(L, 1);
        return d;
    }
}

static PyMethodDef py_Source_methods[] = {
    {"get_station", (PyCFunction)py_Source_get_station, METH_VARARGS, "Get station"},
    {"get_variable", (PyCFunction)py_Source_get_variable, METH_VARARGS, "Get variable"},
    // {"find_variables", (PyCFunction)py_Source_find_variables, METH_VARARGS | METH_KEYWORDS, "Find variables"},
    // {"find_stations", (PyCFunction)py_Source_find_stations, METH_VARARGS | METH_KEYWORDS, "Find stations"},
    {NULL}
};

static PyTypeObject py_SourceType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "meteovm2.Source",         /*tp_name*/
    sizeof(py_Source),         /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)py_Source_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "Source object",           /*tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    py_Source_methods,         /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)py_Source_init,  /* tp_init */
    0,                         /* tp_alloc */
    0,                         /* tp_new */
};

static PyMethodDef meteovm2_methods[] = {
    {NULL}
};

#ifndef PyMODINIT_FUNC  /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif

PyMODINIT_FUNC initmeteovm2(void)
{
    PyObject* m;

    py_SourceType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&py_SourceType) < 0)
        return;

    m = Py_InitModule3("meteovm2", meteovm2_methods,
                       "meteovm2 module.");
    Py_INCREF(&py_SourceType);
    PyModule_AddObject(m, "Source", (PyObject*)&py_SourceType);
}
