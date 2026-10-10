# Adds the Quiet Time glyphs to pebble-fctx 1.6.x .ffont files:
#   0x02 = bell, 0x03 = crossed-out bell
# Re-running replaces previously added glyphs, so the shapes can be tweaked in bellglyph.py and re-applied.
#
# usage (from repo root):
#   python3 tools/glyphs/add_bell_glyphs.py resources/data/RobotoCondensed-{Bold,Regular}{,~diorite,~flint}.ffont
#
# .ffont layout: header(12) | ranges[n](begin,end-exclusive) | glyphs[m](offset,length,adv) | path data
import os, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import bellglyph as bg

FIRST = 0x02
SLASH_W, GAP = 130, 90   # slash thickness and the clear gap above it (font units)

for path in sys.argv[1:]:
    d = open(path, 'rb').read()
    upem, asc, desc, cap, nidx, ntab = struct.unpack('<hhhhHH', d[:12])
    o = 12; ranges = d[o:o + 4*nidx]; o += 4*nidx
    table = d[o:o + 6*ntab]; o += 6*ntab; paths = d[o:]

    if struct.unpack('<H', ranges[:2])[0] == FIRST:  # already patched: strip our range, glyphs and path data
        ranges, nidx = ranges[4:], nidx - 1
        ours = [struct.unpack('<HHh', table[6*i:6*i + 6]) for i in range(2)]
        table, ntab = table[12:], ntab - 2
        paths = paths[:min(off for off, _, _ in ours)]

    blobs = [struct.pack('<%dh' % len(w), *w) for w in
             (bg.to_commands(bg.bell(cap)), bg.to_commands(bg.crossed(cap, SLASH_W, GAP)))]
    adv = bg.advance(cap)
    new_table, off = b'', len(paths)
    for b in blobs:
        new_table += struct.pack('<HHh', off, len(b), adv); off += len(b)

    out = (struct.pack('<hhhhHH', upem, asc, desc, cap, nidx + 1, ntab + 2)
           + struct.pack('<HH', FIRST, FIRST + 2) + ranges + new_table + table + paths + b''.join(blobs))
    open(path, 'wb').write(out)
    print('%s: %d -> %d bytes' % (path, len(d), len(out)))
