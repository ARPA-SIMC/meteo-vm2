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

    def test_parse_line_invalid(self):
        with self.assertRaises(parser.InvalidRecordException):
            for line in (
                "201901,1,2,3.5,3.6,BAU,123",  # invalid date
                "201901020300,a,2,3.5,3.6,BAU,000000000",  # invalid station
                "201901020300,1,b,3.5,3.6,BAU,000000000",  # invalid variable
                "201901020300,1,2,xxx,3.6,BAU,000000000",  # invalid value1
                "201901020300,1,2,3.5,xxx,BAU,000000000",  # invalid value2
                "201901020300,1,2,3.5,3.6,BAU,123",  # invalid flag
                "dkasjdkdjk",  # invalid line
                "",            # empty line
            ):
                parser.parse_line(line)

    def test_parse_line_valid(self):
            for line in (
                "201901020300,1,2,3.5,3.6,BAU,",
            ):
                parser.parse_line(line)

    def test_record_to_line(self):
        line = "201901020300,1,2,3.5,3.6,BAU,000000000"
        record = parser.parse_line(line)
        self.assertEqual(record.to_line(), line)

    def test_parse_file(self):
        from io import StringIO
        fileobj = StringIO("\n".join([
            "201901020300,a,2,3.5,3.6,BAU,123",  # invalid station
            "201901020300,1,2,3.5,3.6,BAU,000000000",  # valid
        ]))

        # Must raise error
        with self.assertRaises(parser.InvalidRecordException):
            list(parser.parse_file(fileobj, raise_on_invalid=True))

        fileobj.seek(0)

        # Must ignore invalid lines
        self.assertEqual(len(list(parser.parse_file(fileobj, raise_on_invalid=False))), 1)

        # Must skip empty lines
        fileobj = StringIO("\n".join([
            "",  # empty
            "201901020300,1,2,3.5,3.6,BAU,000000000",  # valid
        ]))
        self.assertEqual(len(list(parser.parse_file(fileobj, raise_on_invalid=True))), 1)
