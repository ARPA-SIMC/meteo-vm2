import json


def create_table(path):
    with open(path) as fp:
        return Table(json.load(fp))


class ItemTable:
    def __init__(self, json_cfg):
        self.cfg = json_cfg

    def get_by_vm2id(self, vm2id):
        return self.cfg[str(vm2id)]

    def get_by_attrs(self, query):
        results = self.find_by_attrs(query)
        if len(results) == 0:
            raise Exception("No station found")
        elif len(results) > 1:
            raise Exception("More than one station found")
        else:
            return results[0]

    def find_by_attrs(self, query):
        return [(int(k), s)
                for (k, s) in self.cfg.items()
                if all((s[qk] == qv) for qk, qv in query.items())]


class Table:
    def __init__(self, json_cfg):
        self.cfg = json_cfg
        self.station = ItemTable(json_cfg["stations"])
        self.variable = ItemTable(json_cfg["variables"])
