import json
from itertools import chain
import sys
import string


class LuaTemplate(string.Formatter):
    def format_field(self, value, spec):
        if spec == 'lua':
            spec = ''
            if value is None:
                value = "'-'"
            elif isinstance(value, int):
                value = str(value)
            else:
                value = "'{}'".format(str(value).replace("'", "\\'"))

        return super(LuaTemplate, self).format_field(value, spec)



if __name__ == '__main__':
    files = sys.argv[1:]
    stations = chain(*[json.load(open(f))["stations"] for f in files])
    variables = chain(*[json.load(open(f))["variables"] for f in files])

    sys.stdout.write("return {\n")
    sys.stdout.write(" stations={\n")
    for s in stations:
        s["vals"] = ",".join([LuaTemplate().format(
            "{0:}={1:lua}", k, s[k]
        ) for k in filter(
            lambda o: s[o] is not None and o.startswith("B"),
            s.keys()
        )])
        sys.stdout.write(LuaTemplate().format(
            "  [{id}]={{ident={ident:lua},lon={lon},lat={lat},rep={rep:lua},{vals}}},\n",
            **s
        ))

    sys.stdout.write(" },\n")

    sys.stdout.write(" variables={\n")
    for v in variables:
        if v.get("unit"):
            v["vals"] = LuaTemplate().format("unit={0:lua}", v["unit"])
        sys.stdout.write(LuaTemplate().format(
            "  [{id}]={{bcode={bcode:lua},tr={tr:lua},p1={p1:lua},p2={p2:lua},lt1={lt1:lua},lv1={lv1:lua},lt2={lt2:lua},lv2={lv2:lua},{vals}}},\n",
            **v
        ))

    sys.stdout.write(" },\n")
    sys.stdout.write("}\n")
