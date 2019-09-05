from unittest import TestCase

from meteovm2 import utils

import dballe


class TestUtils(TestCase):
    def test_meteovm2_to_bufr(self):
        from io import StringIO, BytesIO
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
        self.assertEqual(data[0]["B12101"], 3.5)
        self.assertEqual(data[0]["level"], dballe.Level(103, 2000))
        self.assertEqual(data[0]["trange"], dballe.Trange(254, 0, 0))
