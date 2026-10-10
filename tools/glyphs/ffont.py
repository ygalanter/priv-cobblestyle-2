# Minimal reader for pebble-fctx 1.6.x .ffont files
# Layout: header(12) | ranges[n](begin,end-exclusive) | glyphs[m](offset,length,adv) | path data
import struct
def read(path):
    d = open(path, 'rb').read()
    upem, asc, desc, cap, nidx, ntab = struct.unpack('<hhhhHH', d[:12])
    o = 12; ranges = [struct.unpack('<HH', d[o+4*i:o+4*i+4]) for i in range(nidx)]
    o += 4*nidx; glyphs = [struct.unpack('<HHh', d[o+6*i:o+6*i+6]) for i in range(ntab)]
    o += 6*ntab
    return dict(hdr=(upem, asc, desc, cap), ranges=ranges, glyphs=glyphs, paths=d[o:], size=len(d))
def cmap(f):
    m = {}; i = 0
    for b, e in f['ranges']:
        for c in range(b, e): m[c] = f['glyphs'][i]; i += 1
    return m
NARGS = {ord('M'): 2, ord('L'): 2, ord('H'): 1, ord('V'): 1, ord('C'): 6, ord('S'): 4, ord('Q'): 4, ord('T'): 2, ord('Z'): 0}
def parse(blob):
    w = struct.unpack('<%dh' % (len(blob)//2), blob); i = 0; out = []
    while i < len(w):
        c = w[i]
        if c not in NARGS: raise ValueError('bad cmd %r at %d' % (c, i))
        k = NARGS[c]; out.append((chr(c), w[i+1:i+1+k])); i += 1+k
    return out
