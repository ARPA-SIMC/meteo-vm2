from . import parser
from .table import create_table
import re

import requests
import dballe


def get_table_from_meteozen(user, password):
    table = {
        "stations": {
            str(s["id"]): {
                "ident": s["ident"],
                "lon": s["lon"],
                "lat": s["lat"],
                "rep": s["network"],
            }
            for s in requests.get(
                "http://meteozen.metarpa/simcstations/api/v1/stations",
                auth=(user, password)
            ).json()
        },
        "variables": {
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
            for v in requests.get(
                "http://meteozen.metarpa/simcstations/api/v1/variables",
                auth=(user, password),
            ).json()
        },
    }
    return table


def meteovm2_to_bufr(fp_input, fp_output, tablepath):
    table = create_table(tablepath)
    bcode_re = re.compile("^B[0-9]{5}$")
    exporter = dballe.Exporter(encoding="BUFR")

    for line in fp_input:
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
        # TODO convert unit
        var = dballe.var(variable["bcode"], float(record.value1))
        msg.set(level, trange, var)
        # TODO parse attributes

        for k, v in station.items():
            if bcode_re.match(k):
                msg.set(None, None, dballe.var(k, v))

        fp_output.write(exporter.to_binary(msg))


def bufr_to_meteovm2(fp_input, fp_output, tablepath):
    table = create_table(tablepath)
    importer = dballe.Importer("BUFR")
    with importer.from_file(infile) as msgfile:
        for msgs in msgfile:
            for msg in msgs:
                for data in msg.query_data():
                    d = data.data_dict
                    station_id, _ = table.station.get_by_attrs({
                        "ident": d["ident"],
                        "lon": d["lon"],
                        "lat": d["lat"],
                        "rep_memo": d["rep"],
                    })
                    variable_id, _ = table.station.get_by_attrs({
                        "bcode": data["variable"].code,
                        "lt1": d["level"][0],
                        "lv1": d["level"][1],
                        "lt2": d["level"][2],
                        "lv2": d["level"][3],
                    })
                    record = Record(
                        d["datetime"],
                        station_id,
                        variable_id,
                        # TODO convert unit
                        d["variable"].enqd(),
                        "",
                        "",
                        # TODO parse attributes
                        "",
                    )
