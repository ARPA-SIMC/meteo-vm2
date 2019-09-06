import os
import sys
import json
from argparse import ArgumentParser
from tempfile import NamedTemporaryFile

from meteovm2 import utils


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


def meteovm2_table_dump():
    parser = ArgumentParser()
    parser.add_argument("--user")
    parser.add_argument("--password")
    parser.add_argument("tablefile")
    args = parser.parse_args()

    table = utils.get_table_from_meteozen(args.user, args.password)

    try:
        tmpfile = NamedTemporaryFile(
            mode='w',
            delete=False,
            dir=os.path.dirname(os.path.abspath(args.tablefile)),
            prefix=".{}.".format(os.path.basename(args.tablefile)),
        )
        json.dump(table, tmpfile)
        os.chmod(tmpfile.name, 0o664)
        tmpfile.close()
        print(tmpfile.name)
        os.rename(tmpfile.name, args.tablefile)
    finally:
        if tmpfile and os.path.exists(tmpfile.name):
            os.remove(tmpfile.name)
