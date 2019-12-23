from . import parser
from .table import create_table
from .parser import Record
import re

import requests
import dballe
import wreport


def get_table_from_meteozen(user, password):
    meteozen_baseurl = "http://meteozen.metarpa/simcstations/api/v1/"

    table = {
        "stations": {},
    }

    for s in requests.get(meteozen_baseurl + "stations",
                          auth=(user, password)).json():
        item = {
            "ident": s["ident"],
            "lon": s["lon"],
            "lat": s["lat"],
            "rep": s["network"],
        }

        for key, bcode, transform in (
            ("name", "B01019", lambda v: v),
            ("height", "B07030", lambda h: str(int(h*10))),
            ("height_barometer", "B07031", lambda h: str(int(h*10))),
        ):
            if key in s and s[key] is not None:
                item[bcode] = transform(s[key])

        table["stations"][str(s["id"])] = item

    table["variables"] = {
        str(v["id"]): {
            "bcode": v["bcode"],
            "tr": v["trange_pind"],
            "p1": v["trange_p1"],
            "p2": v["trange_p2"],
            "lt1": v["level_t1"],
            "lv1": v["level_v1"],
            "lt2": v["level_t2"],
            "lv2": v["level_v2"],
            "unit": v["vm2unit"],
        }
        for v in requests.get(meteozen_baseurl + "variables",
                              auth=(user, password)).json()
    }

    return table


def meteovm2_to_bufr(fp_input, fp_output, tablepath):
    table = create_table(tablepath)
    bcode_re = re.compile("^B[0-9]{5}$")
    exporter = dballe.Exporter(encoding="BUFR")

    for line in fp_input:
        try:
            record = parser.parse_line(line)
            station = table.station.get_by_vm2id(record.station_id)
            variable = table.variable.get_by_vm2id(record.variable_id)
            msg = dballe.Message("generic")
            msg.set_named("year", dballe.var("B04001", record.reftime.year))
            msg.set_named("month", dballe.var("B04002", record.reftime.month))
            msg.set_named("day", dballe.var("B04003", record.reftime.day))
            msg.set_named("hour", dballe.var("B04004", record.reftime.hour))
            msg.set_named("minute", dballe.var("B04005", record.reftime.minute))
            msg.set_named("second", dballe.var("B04006", record.reftime.second))
            msg.set_named("longitude", dballe.var("B06001", station["lon"]))
            msg.set_named("latitude", dballe.var("B05001", station["lat"]))
            msg.set_named("rep_memo", dballe.var("B01194", station["rep"]))
            level = (
                variable["lt1"],
                variable["lv1"],
                variable["lt2"],
                variable["lv2"],
            )
            trange = (
                variable["tr"],
                variable["p1"],
                variable["p2"],
            )

            value = record.value1
            attrs = []

            if record.flags:
                if record.flags[0] == "1":
                    attrs.append(dballe.var("B33196", 1))

                if record.flags[0] in ("1", "2") and record.value2 != "":
                    attrs.append(dballe.var("B33197", 1))
                    value = record.value2

                if record.flags[1:3] == "54" and record.value2 == "":
                    attrs.append(dballe.var("B33192", 0))

            # TODO check missing value
            val = wreport.convert_units(variable["unit"],
                                        dballe.varinfo(variable["bcode"]).unit,
                                        float(value))
            var = dballe.var(variable["bcode"], val)
            for a in attrs:
                var.seta(a)

            msg.set(level, trange, var)

            for k, v in station.items():
                if bcode_re.match(k):
                    msg.set(None, None, dballe.var(k, v))

            fp_output.write(exporter.to_binary(msg))
        except:
            # TODO log error
            import traceback
            traceback.print_exc()


def bufr_to_meteovm2(fp_input, fp_output, tablepath):
    table = create_table(tablepath)
    importer = dballe.Importer("BUFR")
    with importer.from_file(fp_input) as msgfile:
        for msgs in msgfile:
            for msg in msgs:
                for data in msg.query_data({"query": "attrs"}):
                    try:
                        d = data.data_dict
                        station_id, _ = table.station.get_by_attrs({
                            "ident": d.get("ident"),
                            "lon": data.enqi("lon"),
                            "lat": data.enqi("lat"),
                            "rep": d["report"],
                        })
                        variable_id, variable = table.variable.get_by_attrs({
                            "bcode": data["variable"].code,
                            "lt1": d["level"].ltype1,
                            "lv1": d["level"].l1,
                            "lt2": d["level"].ltype2,
                            "lv2": d["level"].l2,
                            "tr": d["trange"].pind,
                            "p1": d["trange"].p1,
                            "p2": d["trange"].p2,
                        })
                        var = data["variable"]
                        val1 = "{:f}".format(
                            wreport.convert_units(data["variable"].info.unit,
                                                  variable["unit"],
                                                  var.enqd())
                        )
                        val2 = ""
                        flags = ["0"]*9

                        for attr in var.get_attrs():
                            if attr.code == "B33196":
                                if attr.enqi() == 0:
                                    flags[0] = "0"
                                else:
                                    flags[0] = "1"
                            elif attr.code == "B33192" and attr.enqi() == 0:
                                flags[1] = "5"
                                flags[2] = "4"
                            elif attr.code == "B33197" and attr.enqi() == 1:
                                val2 = val1

                        record = Record(
                            d["datetime"],
                            station_id,
                            variable_id,
                            # TODO convert unit
                            val1,
                            val2,
                            "",
                            # TODO parse attributes
                            "".join(flags),
                        )
                        fp_output.write(record.to_line() + "\n")
                    except:
                        # TODO log error
                        import traceback
                        traceback.print_exc()
