from . import parser
from .table import create_table


import requests


def create_table_from_meteozen(user, password):
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

    for line in fp_input:
        record = parser.parse_line(line)
        station = table.station.get_by_vm2id(record.station_id)
        variable = table.variable.get_by_vm2id(record.variable_id)
