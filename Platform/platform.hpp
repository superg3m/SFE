#pragma once

#include <Common/common.hpp>
#include <Error/error.hpp>
// Date: August 04, 2025
// NOTE(Jovanni): Im cheating here a bit I should have like win32 platform and linux platform but im just gonna do glfw and win32

namespace Platform {
    typedef void* DLL;

    bool initialize();
    void shutdown();
    void sleep(u32 ms);
    double getSecondsElapsed();
    bool filePathExists(const char* path);
    /**
     * @brief returns true if copy succeeded
     * 
     * @param source_path 
     * @param dest_path 
     * @param block_until_success
     */
    bool copyFile(const char* source_path, const char* dest_path, bool block_until_success = true);
    u8* readEntireFile( const char* file_path, byte_t& out_file_size, Error& error);
    DLL loadDLL(const char* dll_path, Error& error);
    DLL freeDLL(DLL dll, Error& error);
    void* getProcAddress(DLL dll, const char* proc_name, Error& error);
}