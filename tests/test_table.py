import unittest

from meteovm2.table import create_table


class TestTable(unittest.TestCase):
    def setUp(self):
        self.table = create_table("tests/table.json")

    def test_get_vm2id(self):
        s = self.table.station.get_by_vm2id(5591)
        self.assertEqual(s.__class__, dict)

    def test_find_by_attrs(self):
        s = self.table.station.find_by_attrs({
            "ident": None,
            "lon": 1083529,
            "lat": 4400469,
            "rep": "idrost",
        })
        self.assertEqual(len(s), 1)

    def test_get_by_attrs(self):
        s = self.table.station.get_by_attrs({
            "ident": None,
            "lon": 1083529,
            "lat": 4400469,
            "rep": "idrost",
        })
        self.assertEqual(s.__class__, tuple)
        self.assertEqual(s[0], 5591)

        with self.assertRaises(Exception):
            s = self.table.station.get_by_attrs({
                "ident": "ciccioriccio"
            })
