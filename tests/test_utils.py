from unittest import TestCase
from io import StringIO, BytesIO

from meteovm2 import utils

import dballe


class TestUtils(TestCase):
    def test_meteovm2_to_bufr(self):
        inputstring = StringIO("201901020300,1,158,3.5,,,")
        outputstring = BytesIO()

        utils.meteovm2_to_bufr(inputstring, outputstring, "tests/table.json")

        outputstring.seek(0)

        importer = dballe.Importer("BUFR")

        data = []

        with importer.from_file(outputstring) as f:
            for msgs in f:
                for msg in msgs:
                    for d in msg.query_data():
                        data.append(d.data_dict)

        self.assertEqual(len(data), 1)
        self.assertEqual(data[0].get("ident"), None)
        self.assertEqual(data[0]["B12101"], 3.5 + 273.15)
        self.assertEqual(data[0]["level"], dballe.Level(103, 2000))
        self.assertEqual(data[0]["trange"], dballe.Trange(254, 0, 0))

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
        outputstring = StringIO()
        with open("tests/1-158.bufr", "rb") as fp:
            utils.bufr_to_meteovm2(fp, outputstring, "tests/table.json")
            outputstring.seek(0)
            self.assertEqual(outputstring.getvalue(), "201901020300,1,158,3.5,,,")
