#pragma once

#include "../Common/sfe_common.hpp"
#include "sfe_allocator.hpp"

namespace Memory {
    void Zero(void* data, byte_t data_size_in_bytes);
    void Copy(void* destination, byte_t destination_size, const void* source, byte_t source_size);
    bool Equal(const void* buffer_one, byte_t b1_size, const void* buffer_two, byte_t b2_size);
}