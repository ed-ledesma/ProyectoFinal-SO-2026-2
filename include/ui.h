#ifndef UI_H
#define UI_H

#include "memory.h"
#include "uptime.h"
#include "network.h"
#include "disk.h"
#include "cpu.h"
#include "process.h"

/**
 * Function that handles the ncurses ui
 *
 */
void ncurses_ui();

/**
 * Function that renders memory on the ui
 * 
 */
void show_memory(const struct Memory *memory);

/**
 * Function that renders swap on the ui
 * 
 */
void show_swap(const struct Swap *memory);

/**
 * Function that renders uptime on the ui
 * 
 */
void show_uptime(const struct Uptime *uptime);

/**
 * Function that renders network on the ui
 * 
 */
void show_network(const struct Network *network);

/**
 * Function that renders disk on the ui
 * 
 */
void show_disk(const struct Disk *disk);

/**
 * Function that renders cpu on the ui
 * 
 */
void show_cpu(const struct CPU *cpu);

/**
 * Function that renders process on the  ui
 * 
 */
void show_processes(const struct ProcessList *plist);

#endif
