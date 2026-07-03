// Vendored re-implementation of the pebble-localize runtime.
//
// The upstream library ships only as a per-platform binary (.a) that has no
// flint/gabbro build, which blocked those targets. This is a self-contained
// replacement over the exact same on-disk dictionary format produced by the
// library's dict2bin.py, so every existing loc_*.bin resource is reused as-is.
//
// Binary format (little-endian), as written by dict2bin.py:
//   uint32  entry_count
//   repeated entry_count times:
//     uint32  key      (DJB2 hash of the English string, & 0x7FFFFFFF)
//     uint32  length   (strlen(value) + 1, i.e. includes the null terminator)
//     char[]  value    (UTF-8 string followed by a single null byte)

#include <pebble.h>
#include "pebble-localize.h"

static uint8_t *s_dict = NULL;   // the currently loaded language dictionary
static uint32_t s_count = 0;     // number of entries in s_dict

// Reads a 4-byte little-endian value from a (possibly unaligned) location.
static uint32_t read_u32(const uint8_t *p) {
  uint32_t v;
  memcpy(&v, p, sizeof(v));
  return v;
}

void localize_init(uint32_t resource_locale) {
  localize_deinit();

  ResHandle rh = resource_get_handle(resource_locale);
  size_t sz = resource_size(rh);
  if (sz < sizeof(uint32_t)) return;

  s_dict = malloc(sz);
  if (!s_dict) return;

  resource_load(rh, s_dict, sz);
  s_count = read_u32(s_dict);
}

void localize_deinit() {
  if (s_dict) { free(s_dict); s_dict = NULL; }
  s_count = 0;
}

// The upstream cache only affected lookup speed; dictionaries are tiny (~20
// entries) and looked up a handful of times per redraw, so a linear scan is
// fine and the cache size is a no-op.
void localize_set_cache_size(uint16_t size) { (void)size; }

const char *localize_str(int hashval) {
  if (!s_dict) return "";

  const uint8_t *p = s_dict + sizeof(uint32_t); // skip the entry count
  for (uint32_t i = 0; i < s_count; i++) {
    uint32_t key = read_u32(p); p += sizeof(uint32_t);
    uint32_t len = read_u32(p); p += sizeof(uint32_t);
    if (key == (uint32_t)hashval) {
      return (const char *)p; // value is null-terminated on disk
    }
    p += len; // len already includes the trailing null
  }
  return "";
}
