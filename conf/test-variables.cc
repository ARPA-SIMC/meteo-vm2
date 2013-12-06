#include <meteo-vm2/source.h>
#include <wreport/var.h>
#include <wreport/conv.h>
#include <dballe/core/var.h>
#include <iostream>

int main()
{
    meteo::vm2::Source& source = *meteo::vm2::Source::get();
    lua_State* L = source.L;

    lua_newtable(L);
    int result = 0;
    int idx = lua_gettop(L);
    std::vector<int> vars = source.lua_find_variables(idx);
    idx = lua_gettop(L);
    for (std::vector<int>::const_iterator i = vars.begin();
         i != vars.end(); ++i) {
        source.lua_push_variable(*i);

        lua_getfield(L, -1, "bcode");
        wreport::Varcode varcode = wreport::descriptor_code(lua_tostring(L, -1));
        lua_pop(L, 1);
        try {
            wreport::Varinfo varinfo = dballe::varinfo(varcode);
            wreport::Var var(varinfo);
            if (!varinfo->is_string()) {
                lua_getfield(L, -1, "unit");
                if (lua_isstring(L, -1)) {
                    std::string unit = lua_tostring(L, -1);
                    lua_pop(L, 1);
                    var.set(varinfo->imin);
                    double v = wreport::convert_units(varinfo->unit, unit.c_str(), var.enqd());
                    wreport::convert_units(unit.c_str(), varinfo->unit, v);
                }
            }
        } catch (wreport::error_notfound& e) {
            std::cerr << "While testing variable " << *i << ": " << e.what() << std::endl;
            result = 1;
        }

        lua_pop(L, 1);

        lua_settop(L, idx);
    }
    return result;
}
