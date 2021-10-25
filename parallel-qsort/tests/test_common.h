#pragma once

#include "src/raw_array.h"
#include <cstdint>

raw_array<int32_t> gen_array(size_t size, int32_t min_elem = -100'000, int32_t max_elem = 100'000);
