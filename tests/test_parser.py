from unittest import TestCase
from datetime import datetime

from meteovm2 import parser


class TestParser(TestCase):
    def setUp(self):
        pass

    def test_parse_line(self):
        line = "201901020300,1,2,3.5,3.6,BAU,000000000"
        record = parser.parse_line(line)
        self.assertEqual(record.__class__, parser.Record)
        self.assertEqual(record.reftime, datetime(2019, 1, 2, 3))
        self.assertEqual(record.station_id, 1)
        self.assertEqual(record.variable_id, 2)
        self.assertEqual(record.value1, "3.5")
        self.assertEqual(record.value2, "3.6")
        self.assertEqual(record.value3, "BAU")
        self.assertEqual(record.flags, "000000000")

    def test_record_to_line(self):
        line = "201901020300,1,2,3.5,3.6,BAU,000000000"
        record = parser.parse_line(line)
        self.assertEqual(record.to_line(), line)
