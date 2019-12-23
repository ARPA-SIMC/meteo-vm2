import os
import json
from argparse import ArgumentParser
from tempfile import NamedTemporaryFile

from meteovm2 import utils


def main():
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
        json.dump(table, tmpfile, indent=4)
        os.chmod(tmpfile.name, 0o664)
        tmpfile.close()
        os.rename(tmpfile.name, args.tablefile)
    finally:
        if tmpfile and os.path.exists(tmpfile.name):
            os.remove(tmpfile.name)


if __name__ == '__main__':
    main()
