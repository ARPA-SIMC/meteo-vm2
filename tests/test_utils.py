from unittest import TestCase

from meteovm2 import utils


class TestUtils(TestCase):
    def test_meteovm2_to_bufr(self):
        from io import StringIO
        inputstring = StringIO("201901020300,1,158,3.5,,,")
        outputstring = StringIO()

        utils.meteovm2_to_bufr(inputstring, outputstring, "tests/table.json")
