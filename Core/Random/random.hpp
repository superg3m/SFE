#pragma once

#include <Common/common.hpp>

namespace Random {
    const int STATE_VECTOR_LENGTH = 624;
    const int STATE_VECTOR_M      = 397;

    struct Seed {
        u32 mt[STATE_VECTOR_LENGTH];
        u32 index;
    };

    Seed GenerateSeed(u32 seed); // non-zero seeds work
    u32 GenerateU32(Seed* seed);
    float GenerateF32(Seed* seed); // 0 to 1 0.12190845 for example
    float GenerateRange(Seed* seed, float min, float max); // min to max for example
}

