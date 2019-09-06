import sys
from argparse import ArgumentParser

from .. import utils


def meteovm2_to_bufr():
    parser = ArgumentParser()
    parser.add_argument("tablefile", nargs="?")
    args = parser.parse_args()
    return utils.meteovm2_to_bufr(sys.stdin, sys.buffer.stdout, args.tablefile)


def bufr_to_meteovm2():
    parser = ArgumentParser()
    parser.add_argument("tablefile", nargs="?")
    args = parser.parse_args()
    return utils.bufr_to_meteovm2(sys.buffer.stdin, sys.stdout, args.tablefile)
