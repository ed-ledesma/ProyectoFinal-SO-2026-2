#include <unistd.h>
#include <ncurses.h>

#include "memory.h"
#include "ui.h"
#include "uptime.h"

void show_memory(const struct Memory *memory) {
    mvprintw(1, 2, "Memory");
    mvprintw(2, 4, "Total:      %lu MB", memory->mem_total / 1024);
    mvprintw(3, 4, "Free:       %lu MB", memory->mem_free / 1024);
    mvprintw(4, 4, "Available:  %lu MB", memory->mem_available / 1024);
}

void show_swap(const struct Swap *swap) {
    mvprintw(6, 2, "Swap");
    mvprintw(7, 4, "Total:      %lu MB", swap->swap_total / 1024);
    mvprintw(8, 4, "Free:       %lu MB", swap->swap_free / 1024);
}

void show_uptime(const struct Uptime *uptime) {
    mvprintw(10, 2, "Uptime");
    mvprintw(11, 4, "%lf seconds", uptime->time);
}

void ncurses_ui(void) {
    struct Memory memory;
    struct Swap swap;
    struct Uptime uptime;

    initscr();
    noecho();
    cbreak();
    curs_set(0);

    while (1) {
        get_memory_info(&memory, &swap);
        get_uptime(&uptime);

        clear();

        show_memory(&memory);
        show_swap(&swap);
        show_uptime(&uptime);

        refresh();

        sleep(1);
    }

    endwin();
}