#ifndef UTILS_H
#define UTILS_H

#include <jni.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

typedef unsigned long DWORD;
static uintptr_t libBase;

DWORD findLibrary(const char *library);

DWORD getAbsoluteAddress(const char *libraryName, DWORD relativeAddr);

bool isLibraryLoaded(const char *libraryName);

DWORD findLibrary(const char *library) {
    char filename[0xFF] = {0},
            buffer[1024] = {0};
    FILE *fp = NULL;
    DWORD address = 0;

    sprintf(filename, "/proc/self/maps");

    fp = fopen(filename, "rt");
    if (fp == NULL) {
        perror("fopen");
        goto done;
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strstr(buffer, library)) {
            address = (DWORD) strtoul(buffer, NULL, 16);
            goto done;
        }
    }

    done:

    if (fp) {
        fclose(fp);
    }

    return address;
}

DWORD getAbsoluteAddress(const char *libraryName, DWORD relativeAddr) {
    libBase = findLibrary(libraryName);
    if (libBase == 0)
        return 0;
    return (reinterpret_cast<DWORD>(libBase + relativeAddr));
}

bool isLibraryLoaded(const char *libraryName) {
    char line[512] = {0};
    FILE *fp = fopen("/proc/self/maps", "rt");
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, libraryName))
                return true;
        }
        fclose(fp);
    }
    return false;
}
static uintptr_t wait_for_library(const char *lib_name, uint32_t timeout_ms = 10000)
{
    auto start_time = std::chrono::steady_clock::now();

    LOGI("Waiting for %s...", lib_name);

    while (true)
    {
        uintptr_t base_addr = Tools::GetBaseAddress(lib_name);
        if (base_addr != 0)
        {
            LOGI("%s found at: 0x%p", lib_name, (void *)base_addr);
            return base_addr;
        }

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                           std::chrono::steady_clock::now() - start_time)
                           .count();

        if (elapsed >= timeout_ms)
        {
            LOGW("Timeout waiting for %s after %d ms", lib_name, timeout_ms);
            return 0;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


ProcMap waitForLib(const char* libName, int poll_us = 100000) {
    ProcMap map;
    while (!map.isValid()) {
        map = KittyMemory::getLibraryMap(libName);
        if (!map.isValid()) usleep(poll_us);
    }
    LOGI("%s found at: 0x%lx", libName, (unsigned long)map.startAddr);
    return map;
}


#endif