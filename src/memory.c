#include <stdio.h>
#include <string.h>

#include "memory.h"

void get_memory_info(struct Memory *memory, struct Swap *swap) {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) return;

    char key[64];
    unsigned long value;
    char unit[16];

    while (fscanf(fp, "%63s %lu %15s\n", key, &value, unit) >= 2) {
        if      (strcmp(key, "MemTotal:")     == 0) memory->mem_total     = value;
        else if (strcmp(key, "MemFree:")      == 0) memory->mem_free      = value;
        else if (strcmp(key, "MemAvailable:") == 0) memory->mem_available = value;
        else if (strcmp(key, "SwapTotal:")    == 0) swap->swap_total      = value;
        else if (strcmp(key, "SwapFree:")     == 0) swap->swap_free       = value;
    }

    fclose(fp);
}