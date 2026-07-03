#pragma once
#include "hash.h"

// Compile-time hashes the (English) key string, so at runtime only the 32-bit
// DJB2 hash is passed to localize_str() -- the original text is never shipped.
#define _(str) localize_str(HASH_DJB2(str))

void localize_init(uint32_t resource_locale);

void localize_deinit();

void localize_set_cache_size(uint16_t size);

const char *localize_str(int hashval);
