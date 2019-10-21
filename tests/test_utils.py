from unittest import TestCase
from io import StringIO, BytesIO

from meteovm2 import utils

import dballe


class TestUtils(TestCase):
    def test_meteovm2_to_bufr(self):
        inputstring = StringIO((
            # Valid
            "201901020300,1,158,3.5,,,\n"
            "201901020300,1,158,3.5,,,000000000\n"
            # Manual invalidation
            "201901020300,1,158,3.5,,,100000000\n"
            # Gross error
            "201901020300,1,158,3.5,,,054000000\n"
            # Manual invalidation + gross error
            "201901020300,1,158,3.5,,,154000000\n"
            # Manual replacement
            "201901020300,1,158,3.5,3.6,,200000000\n"
        ))
        outputstring = BytesIO()

        utils.meteovm2_to_bufr(inputstring, outputstring, "tests/table.json")

        outputstring.seek(0)

        importer = dballe.Importer("BUFR")

        data = []
        attrs = []

        with importer.from_file(outputstring) as f:
            for msgs in f:
                for msg in msgs:
                    for d in msg.query_data():
                        data.append(d.data_dict)
                        attrs.append(d["variable"].get_attrs())

        self.assertEqual(len(data), 2)

        self.assertEqual(data[0].get("ident"), None)
        self.assertEqual(data[0]["B12101"], 3.5 + 273.15)
        self.assertEqual(data[0]["level"], dballe.Level(103, 2000))
        self.assertEqual(data[0]["trange"], dballe.Trange(254, 0, 0))

        self.assertEqual(attrs[1], [dballe.var("B33196", 1)])

    def test_meteovm2_to_bufr_invalid(self):
        for line in (
            "201901020300,9999,158,3.5,,,",  # invalid station
            "201901020300,1,9999,3.5,,,",  # invalid variable
        ):
            inputstring = StringIO(line)
            outputstring = BytesIO()
            utils.meteovm2_to_bufr(inputstring, outputstring, "tests/table.json")
            outputstring.seek(0)
            self.assertEqual(outputstring.getvalue(), b'')

    def test_bufr_to_meteovm2(self):
        from meteovm2 import parser
        outputstring = StringIO()
        with open("tests/1-158.bufr", "rb") as fp:
            utils.bufr_to_meteovm2(fp, outputstring, "tests/table.json")
            outputstring.seek(0)
            record = parser.parse_line(outputstring.getvalue())
            self.assertEqual(outputstring.getvalue(),
                             "201901020300,1,158,{},,,\n".format(record.value1))
