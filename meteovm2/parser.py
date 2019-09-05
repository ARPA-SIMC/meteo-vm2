from datetime import datetime


def parse_line(line):
    items = line.strip().split(",")
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
