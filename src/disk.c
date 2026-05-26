#include <stdio.h>
#include <string.h>

#include "disk.h"

void get_disk_info(struct Disk *disk) {
    FILE *fp = fopen("/proc/diskstats", "r");
    if (!fp) return;

    char line[512];
    char dev[32];

    unsigned long sectors_read;
    unsigned long sectors_written;

    unsigned long read_bytes;
    unsigned long write_bytes;

    // Dummy for unused fields
    unsigned long dummy[17];

    // Save previous values
    disk->prev_read = disk->read;
    disk->prev_write = disk->write;

    while (fgets(line, sizeof(line), fp)) {

        sscanf(
            line,
            "%lu %lu %31s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
            &dummy[0],
            &dummy[1],
            dev,
            &dummy[2],
            &dummy[3],
            &sectors_read,
            &dummy[4],
            &dummy[5],
            &dummy[6],
            &sectors_written,
            &dummy[7],
            &dummy[8],
            &dummy[9],
            &dummy[10],
            &dummy[11],
            &dummy[12],
            &dummy[13],
            &dummy[14],
            &dummy[15],
            &dummy[16]
        );

        read_bytes = sectors_read * 512;
        write_bytes = sectors_written * 512;
        
        if (!disk->initialized) {
            disk->prev_read = read_bytes;
            disk->prev_write = write_bytes;

            disk->read_rate = 0;
            disk->write_rate = 0;

            disk->initialized = 1;
        } else {
            disk->read_rate = read_bytes - disk->prev_read;
            disk->write_rate = write_bytes - disk->prev_write;

            disk->prev_read = read_bytes;
            disk->prev_write = write_bytes;
        }

        disk->read = read_bytes;
        disk->write = write_bytes;

        break;
    }

    fclose(fp);
}