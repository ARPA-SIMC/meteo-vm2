import unittest

from meteovm2.table import create_station_table


class TestStationTable(unittest.TestCase):
    def test_create(self):
        st = create_station_table("tests/stations.json")

    def test_get_vm2id(self):
        st = create_station_table("tests/stations.json")
        s = st.get_by_vm2id(5591)
        self.assertEqual(s.__class__, dict)

    def test_find_by_attrs(self):
        st = create_station_table("tests/stations.json")
        s = st.find_by_attrs({
            "ident": None,
            "lon": 1083529,
            "lat": 4400469,
            "rep": "idrost",
        })
        self.assertEqual(len(s), 1)

    def test_get_by_attrs(self):
        st = create_station_table("tests/stations.json")
        s = st.get_by_attrs({
            "ident": None,
            "lon": 1083529,
            "lat": 4400469,
            "rep": "idrost",
        })
        self.assertEqual(s.__class__, tuple)
        self.assertEqual(s[0], 5591)

        with self.assertRaises(Exception):
            s = st.get_by_attrs({
                "ident": "ciccioriccio"
            })
