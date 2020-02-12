#!/usr/bin/python3

import sys
import os
import csv
import string

template = '''
<html>
<head>
  <title>Will it scale?</title>
  <script src="https://cdn.plot.ly/plotly-latest.min.js"></script>
  <script src="plot.js"></script>
</head>

<body>
  <div id="plot"></div>
  <script>
    var data = $data;
    createPlot(data, '$title');
  </script>
</body>
</html>
'''

def parse_data(csvfile):
    data = []
    fp = open(csvfile)
    csv_reader = csv.reader(fp, dialect='excel')
    for row in csv_reader:
        data.append(row)
    fp.close()
    return data


def process(base):
    csvfile = base + '.csv'
    titlefile = base + '.title'
    htmlfile = base + '.html'

    data = parse_data(csvfile)
    title = open(titlefile).readline().strip()

    t = string.Template(template)
    html = t.substitute(data=data, title=title)

    open(htmlfile, 'w').write(html)
    print('Created %s' % (htmlfile))


def for_each_file(dirname, subdirs, filenames):
    for f in filenames:
        if f.endswith('.csv'):
            process(f)


if __name__ == '__main__':

    for root, dirs, files in os.walk('.'):
        if root == '.':
            for f in files:
                if f.endswith('.csv'):
                    tmp = f.split('.', 1)
                    try:
                        process(tmp[0])
                    except:
                        sys.exit(1)
