INST_NAME_LEN = 71
NUM_LEN = 7

TOTAL = INST_NAME_LEN + (6*NUM_LEN)

univs = []

with open('Universities.csv', 'r') as f:
    for line in f.read().splitlines():
        fields = line.split(',')
        a = fields[0].replace(' ', '_').ljust(INST_NAME_LEN, ' ')
        b = fields[1].ljust(NUM_LEN, ' ')
        c = fields[2].ljust(NUM_LEN, ' ')
        d = fields[3].ljust(NUM_LEN, ' ')
        e = fields[4].ljust(NUM_LEN, ' ')
        g = fields[5].ljust(NUM_LEN, ' ')
        h = fields[6].ljust(NUM_LEN, ' ')

        new = a+""+b+""+c+""+d+""+e+""+g+""+h
        empty = ' '*TOTAL

        univs.append(new)
        univs.append(empty)


blank = '\n'.join(univs)
reset = open('ResetUniversities.db', 'w')
# reset.truncate()
reset.write(blank)
reset.close()
