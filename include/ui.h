#ifndef UI_H
#define UI_H

#include "memory.h"

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

#endif
