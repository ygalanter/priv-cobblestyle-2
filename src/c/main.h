#include <pebble.h>


// bluetooth vibe patterns
const VibePattern VIBE_PATTERN_WEAK = {
	.durations = (uint32_t []) {100},
	.num_segments = 1
};

const VibePattern VIBE_PATTERN_NORMAL = {
	.durations = (uint32_t []) {300},
	.num_segments = 1
};

const VibePattern VIBE_PATTERN_STRONG = {
	.durations = (uint32_t []) {500},
	.num_segments = 1
};

const VibePattern VIBE_PATTERN_DOUBLE = {
	.durations = (uint32_t []) {500,100,500},
	.num_segments = 3
};
