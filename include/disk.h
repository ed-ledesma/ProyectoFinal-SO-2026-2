#ifndef DISK_H
#define DISK_H

/**
 * @struct Disk
 * @brief State of disk
 * 
 */
struct Disk {
    unsigned long algo;
};

/**
 * @brief Function that reads /proc/diskstats}
 * 
 * @param disk Struct to store disk state
 * 
 * @return void
 * 
 */
void get_disk_info(struct Disk disk);

#endif