#ifndef PROCESS_H
#define PROCESS_H

#define INITIAL_CAPACITY 1024
#define MAX_PID 65536

/**
 * @struct Process
 * @brief State of a process
 */
struct Process {
    int pid; // Process ID
    char name[256]; // Process name / command name
    char user[32]; // Username that owns the process
    char state; // Current process state

    unsigned long utime; // User CPU time
    unsigned long stime; // System CPU time

    unsigned long vsize;  // Virtual memory size
    long rss; // Resident set size

    float cpu_percent; // CPU Percentage
    float mem_percent; // Memory Percentage
};

/**
 * @struct ProcessList
 * @brief Dynamic list of processes
 */
struct ProcessList {
    struct Process *processes;

    int count;
    int capacity;

    unsigned long prev_system_total;
};

/**
 * @brief Initialize process list
 */
void init_process_list(struct ProcessList *plist);

/**
 * @brief Free process list memory
 */
void destroy_process_list(struct ProcessList *plist);

/**
 * @brief Read process information from /proc
 */
void get_process_info(struct ProcessList *plist);

#endif