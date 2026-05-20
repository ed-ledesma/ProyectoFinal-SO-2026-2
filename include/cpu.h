#ifndef CPU_H
#define CPU_H

/**
 * @struct CPU
 * @brief State of CPU
 * 
 */
struct CPU {
    unsigned long cpu;
    unsigned long prev_total;
    unsigned long prev_idle;
    double usage;
};


/**
 * @brief Function that reads /proc/stat 
 * 
 * @param cpu Struct to store CPU state
 * 
 * @return void
 * 
 */
void get_cpu_info(struct CPU *cpu);

#endif