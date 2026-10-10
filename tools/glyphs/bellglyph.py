# Bell / crossed-bell glyph outlines as polygons in font units (y up, baseline 0, height = cap height H).
import math
LSB = 50
STEP = 10  # degrees per arc segment

def _qbez(p0, p1, p2, n=8):
    return [((1-t)**2*p0[0] + 2*(1-t)*t*p1[0] + t*t*p2[0], (1-t)**2*p0[1] + 2*(1-t)*t*p1[1] + t*t*p2[1])
            for t in (i/n for i in range(n+1))]

def _arc(c, r, a0, a1):
    n = max(2, int(abs(a1 - a0) / STEP))
    return [(c[0] + r*math.cos(math.radians(a0 + (a1-a0)*i/n)), c[1] + r*math.sin(math.radians(a0 + (a1-a0)*i/n))) for i in range(n+1)]

# --- design space (normalised to height H afterwards) ---
W2 = 380           # half width of the rim
RIM_B, RIM_T = 290, 400   # rim bottom/top
BODY = 225         # half width of the body
BODY_Y = 520       # where the flare meets the straight side
DOME_C = 560       # dome centre y (dome radius = BODY)
KNOB_R = 55
CLAP_R = 95        # clapper radius (circle sitting on the baseline)

def _raw():
    cx = W2
    dome_c = (cx, DOME_C); knob_c = (cx, DOME_C + BODY + KNOB_R*0.35)
    # angle on the dome where it meets the knob circle
    a = 90.0
    while math.hypot(dome_c[0] + BODY*math.cos(math.radians(a)) - knob_c[0], dome_c[1] + BODY*math.sin(math.radians(a)) - knob_c[1]) < KNOB_R:
        a -= 0.25
    pk = (dome_c[0] + BODY*math.cos(math.radians(a)), dome_c[1] + BODY*math.sin(math.radians(a)))
    ka = math.degrees(math.atan2(pk[1] - knob_c[1], pk[0] - knob_c[0]))
    right = _qbez((cx + W2, RIM_T), (cx + BODY, RIM_T + 20), (cx + BODY, BODY_Y))
    left = [(2*cx - x, y) for x, y in reversed(right)]
    body = ([(cx - W2, RIM_B), (cx + W2, RIM_B)] + right + [(cx + BODY, DOME_C)]
            + _arc(dome_c, BODY, 0, a)[1:] + _arc(knob_c, KNOB_R, ka, 180 - ka)[1:-1]
            + _arc(dome_c, BODY, 180 - a, 180)[:-1] + [(cx - BODY, DOME_C)] + left)
    clapper = _arc((cx, CLAP_R), CLAP_R, 0, 360)[:-1]
    return [body, clapper]

def _norm(polys, H):
    top = max(y for p in polys for x, y in p)
    k = H / top
    return [[(LSB + x*k, y*k) for x, y in p] for p in polys], k

def bell(H):
    return _norm(_raw(), H)[0]

def advance(H):
    polys, k = _norm(_raw(), H)
    return int(round(2*LSB + 2*W2*k))

def _clip(poly, nx, ny, d):
    # keep the part of poly where nx*x + ny*y >= d (Sutherland-Hodgman, one edge)
    out = []
    for i in range(len(poly)):
        a, b = poly[i-1], poly[i]
        ia, ib = nx*a[0] + ny*a[1] - d, nx*b[0] + ny*b[1] - d
        if ib >= 0:
            if ia < 0: out.append((a[0] + (b[0]-a[0])*ia/(ia-ib), a[1] + (b[1]-a[1])*ia/(ia-ib)))
            out.append(b)
        elif ia >= 0:
            out.append((a[0] + (b[0]-a[0])*ia/(ia-ib), a[1] + (b[1]-a[1])*ia/(ia-ib)))
    return out

def crossed(H, slash_w, gap):
    # slash from top-left to bottom-right through the bell's centre; a clear gap only on its upper side
    # (a gap on both sides breaks the bell into stripes at ~12px)
    polys = bell(H)
    xs = [x for p in polys for x, y in p]; cx = (min(xs) + max(xs)) / 2; cy = H / 2
    ux, uy = math.cos(math.radians(-45)), math.sin(math.radians(-45))
    nx, ny = -uy, ux                     # normal pointing to the upper-right side
    dc = nx*cx + ny*cy; sw = slash_w / 2
    parts = []
    for p in polys:
        for q in (_clip(p, nx, ny, dc + sw + gap), _clip(p, -nx, -ny, -(dc - sw))):
            if len(q) > 2: parts.append(q)
    L = H * 0.50
    slash = [(cx - ux*L + nx*sw, cy - uy*L + ny*sw), (cx + ux*L + nx*sw, cy + uy*L + ny*sw),
             (cx + ux*L - nx*sw, cy + uy*L - ny*sw), (cx - ux*L - nx*sw, cy - uy*L - ny*sw)]
    return parts + [slash]

def to_commands(polys):
    words = []
    for poly in polys:
        pts = [(int(round(x)), int(round(y))) for x, y in poly]
        dd = [pts[0]] + [p for i, p in enumerate(pts[1:], 1) if p != pts[i-1]]
        if dd[-1] == dd[0]: dd.pop()
        words += [ord('M'), *dd[0]]
        for p in dd[1:]: words += [ord('L'), *p]
        words += [ord('Z')]
    return words
