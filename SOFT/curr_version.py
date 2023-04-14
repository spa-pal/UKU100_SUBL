#!/usr/bin/env python

import re
from datetime import datetime

source_file = 'curr_version.c'
now=datetime.now()

build_string = 'const short BUILD = {};'
build_re = re.compile(build_string.format('(\d+)'))

build_year_string = 'const short BUILD_YEAR = {};'
build_year_re = re.compile(build_year_string.format('(\d+)'))

build_month_string = 'const short BUILD_MONTH = {};'
build_month_re = re.compile(build_month_string.format('(\d+)'))

build_day_string = 'const short BUILD_DAY = {};'
build_day_re = re.compile(build_day_string.format('(\d+)'))

data = []
with open(source_file) as f:
    for line in f:
        match_build = build_re.search(line)
        match_build_year = build_year_re.search(line)
        match_build_month = build_month_re.search(line)
        match_build_day = build_day_re.search(line)
        if match_build:
            build = int(match_build.group(1))
            data.append(build_string.format(build + 1) + '\n')
        elif match_build_year:
            data.append(build_year_string.format(now.year) + '\n')
        elif match_build_month:
            data.append(build_month_string.format(now.month) + '\n')
        elif match_build_day:
            data.append(build_day_string.format(now.day) + '\n')
        else:
            data.append(line)

with open(source_file, 'w') as f:
    f.write(''.join(data))

