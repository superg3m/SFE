#pragma once

#include <cstdarg>
#include <cstdio>

#include <Common/common.hpp>
#include <DataStructure/ds.hpp>
#include <Memory/memory.hpp>

namespace String {
    char* allocate( const char* s1, u64 length);
    char* sprintf( u64* out_buffer_length, const char* fmt ...);
    char* sprintf( u64* out_buffer_length, const char* fmt, va_list args);

    u64 length(const char* c_string);
    bool equal(const char* s1, u64 s1_length, const char* s2, u64 s2_length);
    bool equal(const char* s1, const char* s2);
    bool equal(DS::View<char> s1, DS::View<char> s2);
    bool contains(const char* s1, u64 s1_length, const char* contains, u64 contains_length);

    s64 indexOf(const char* str, u64 str_length, const char* substring, u64 substring_length);
    s64 lastIndexOf(const char* str, u64 str_length, const char* substring, u64 substring_length);
    bool startsWith(const char* str, u64 str_length, const char* starts_with, u64 starts_with_length);
    bool endsWith(const char* str, u64 str_length, const char* ends_with, u64 ends_with_length);

    void copy(char* s1, byte_t s1_capacity, const char* s2, u64 s2_length);
    void append(char* str, u64 &str_length_out, byte_t str_capacity, const char* to_append, u64 to_append_length);
    void append(char* str, u64 &str_length_out, byte_t str_capacity, char to_append);
    void insert(char* str, u64 &str_length_out, byte_t str_capacity, const char* to_insert, u64 to_insert_length, u64 index);
    void insert(char* str, u64 &str_length_out, byte_t str_capacity, char to_insert, u64 index);
}