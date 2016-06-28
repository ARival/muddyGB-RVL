import subprocess
import math

def createFreqTable(fn):
    freqs = []
    bfreq = 110
    quant = 12
    scalen = quant*12
    step = math.pow(2.0, 1.0/scalen)
    for oc in range(0, 6):
        afreq = bfreq
        for sc in range(0, scalen):
            freqs.append(afreq)
            afreq *= step
        bfreq *= 2

    with open(fn, 'w') as f:
        f.write("const USHORT note_frequencies[] = {\n")
        row = ''
        c = 0
        for freq in freqs:
            row += str(int(2**11 - (2**17/freq)))+', '
            c += 1
            if c == quant:
                c = 0
                row += '\n'
                f.write(row)
                row = ''
        f.write('};')
    pass



print "hello MuddyGB-DSD"

createFreqTable("freqtable.c")

subprocess.call(["make"])

subprocess.call(["vba", "muddygb_dsd.gb"])


