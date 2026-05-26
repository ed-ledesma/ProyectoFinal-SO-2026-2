#ifndef MEMORY_H
#define MEMORY_H

/**
 * @struct Memory
 * @brief State of memory
 * 
 */
struct Memory {
    unsigned long mem_total;
    unsigned long mem_free;
    unsigned long mem_available;

};

/**
 * @struct Swap
 * @brief State of swap
 * 
 */
struct Swap {
    unsigned long swap_total;
    unsigned long swap_free;

};

/**
 * @brief Function that reads /proc/meminfo
 * 
 * @param memory Struct to store memory state
 * @param swap Struct to store swap state
 * 
 * @return void
 * 
 */
void get_memory_info(struct Memory *memory, struct Swap *swap);

#endif