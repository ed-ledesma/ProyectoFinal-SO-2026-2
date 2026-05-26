#ifndef UPTIME_H
#define UPTIME_H

/**
 * @struct Uptime
 * @brief System uptime
 * 
 */
struct Uptime {
    double time;
};

/**
 * @brief Function that reads /proc/uptime
 * 
 * @param uptime Struct to store uptime info
 * 
 * @return void
 * 
 */
void get_uptime(struct Uptime *uptime);

#endif