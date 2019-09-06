from datetime import datetime
import re


LINE_RE = re.compile((
    r'^(?P<date>\d{12}(\d{2})?),'
    r'(?P<station>\d+),'
    r'(?P<variable>\d+),'
    r'(?P<value1>[+-]?[\d.]*),'
    r'(?P<value2>[+-]?[\d.]*),'
    r'(?P<value3>[^,\n\r]*),'
    r'(?P<flags>\d{9})?$'
))


def parse_file(fileobj, raise_on_invalid=True):
    for line in fileobj:
        if not line.strip():
            continue
        else:
            try:
                yield parse_line(line)
            except InvalidRecordException:
                if raise_on_invalid:
                    raise


def parse_line(line):
    m = LINE_RE.match(line)
    if not m:
        raise InvalidRecordException("Invalid record: {}".format(line))

    g = m.groupdict()
    items = [g["date"], g["station"], g["variable"],
             g["value1"], g["value2"], g["value3"], g["flags"]]
    return parse_list(items)


def parse_list(items):
    datefmt = "%Y%m%d%H%M%S" if len(items[0]) > 12 else "%Y%m%d%H%M"
    return Record(
        datetime.strptime(items[0], datefmt),
        int(items[1]),
        int(items[2]),
        items[3],
        items[4],
        items[5],
        items[6],
    )


class InvalidRecordException(Exception):
    pass


class Record:
    def __init__(self, reftime, station_id, variable_id,
                 value1, value2, value3, flags):
        self.reftime = reftime
        self.station_id = station_id
        self.variable_id = variable_id
        self.value1 = value1
        self.value2 = value2
        self.value3 = value3
        self.flags = flags

    def to_line(self):
        if self.reftime.second == 0:
            datefmt = "{:%Y%m%d%H%M}"
        else:
            datefmt = "{:%Y%m%d%H%M%S}"

        d = datefmt.format(self.reftime)
        return ",".join(map(str, (d, self.station_id, self.variable_id,
                                  self.value1, self.value2, self.value3,
                                  self.flags)))
