#ifndef DISK_H
#define DISK_H

/**
 * @struct Disk
 * @brief State of disk
 * 
 */
struct Disk {
    unsigned long read;
    unsigned long prev_read;
    unsigned long read_rate;

    unsigned long write;
    unsigned long prev_write;
    unsigned long write_rate;

    int initialized;
};

/**
 * @brief Function that reads /proc/diskstats
 * 
 * @param disk Struct to store disk state
 * 
 * @return void
 * 
 */
void get_disk_info(struct Disk *disk);

#endif