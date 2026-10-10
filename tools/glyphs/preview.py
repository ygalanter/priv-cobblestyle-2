# Preview bell glyphs: large, plus actual watch sizes (AA for colour, 1-bit for B&W)
import sys; sys.path.insert(0, sys.argv[1]); import bellglyph as bg
from PIL import Image, ImageDraw
SLASH, GAP = int(sys.argv[2]), int(sys.argv[3])
def render(H, cap_px, ss, bw):
    sc = cap_px / H * ss
    sets = [bg.bell(H), bg.crossed(H, SLASH, GAP)]
    adv = bg.advance(H)
    w = int(adv*sc) + 2*ss; h = int(H*1.1*sc) + 2*ss
    out = Image.new('L', (w*2 + 4*ss, h), 0); d = ImageDraw.Draw(out)
    for k, polys in enumerate(sets):
        for p in polys: d.polygon([(ss + k*(w + 4*ss) + x*sc, h - ss - y*sc) for x, y in p], fill=255)
    if ss > 1: out = out.resize((out.width//ss, out.height//ss), Image.LANCZOS)
    if bw: out = out.point(lambda v: 255 if v >= 128 else 0)
    return out
rows = [('large', render(820, 160, 1, False))]
for name, cap, bw in [('diorite 12px 1-bit', 12, True), ('chalk 12px AA', 12, False), ('basalt 12px AA', 12, False), ('emery 17px AA', 17, False), ('gabbro 18px AA', 18, False)]:
    im = render(806 if bw else 864, cap, 1 if bw else 4, bw)
    rows.append((name, im.resize((im.width*6, im.height*6), Image.NEAREST)))
W = max(i.width for _, i in rows) + 10; Hh = sum(i.height + 18 for _, i in rows)
sh = Image.new('RGB', (W, Hh), (90, 90, 90)); dr = ImageDraw.Draw(sh); y = 0
for t, i in rows: dr.text((4, y + 2), t, fill='white'); sh.paste(i.convert('RGB'), (4, y + 16)); y += i.height + 18
sh.save(sys.argv[4])
