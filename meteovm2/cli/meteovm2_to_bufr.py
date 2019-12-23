import sys
from argparse import ArgumentParser

from meteovm2 import utils


def main():
    parser = ArgumentParser()
    parser.add_argument("tablefile", nargs="?")
    args = parser.parse_args()
    return utils.meteovm2_to_bufr(sys.stdin, sys.stdout.buffer, args.tablefile)


if __name__ == '__main__':
    main()
