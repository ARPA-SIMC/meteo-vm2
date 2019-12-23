import sys
from argparse import ArgumentParser

from meteovm2 import utils


def main():
    parser = ArgumentParser()
    parser.add_argument("tablefile", nargs="?")
    args = parser.parse_args()
    return utils.bufr_to_meteovm2(sys.stdin.buffer, sys.stdout, args.tablefile)


if __name__ == '__main__':
    main()
