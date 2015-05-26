#!/usr/bin/env python

import json
import sys


json.dump(json.load(sys.stdin), sys.stdout, sort_keys=True, indent=4)
