#include <unistd.h>
#include <ncurses.h>

#include "memory.h"
#include "ui.h"
#include "uptime.h"
#include "network.h"

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

void show_network(const struct Network *network) {
    mvprintw(13, 2, "Network");
    mvprintw(14, 4, "Receiving:       %lu MB", network->rx_rate / 1048576);
    mvprintw(15, 4, "Total Received:  %lu MB", network->rx / 1048576);
    mvprintw(16, 4, "Sending:         %lu MB", network->tx_rate / 1048576);
    mvprintw(17, 4, "Total Sent:      %lu MB", network->tx / 1048576);
    
}

void ncurses_ui(void) {
    struct Memory memory = {0};
    struct Swap swap = {0};
    struct Uptime uptime = {0};
    struct Network network = {0};

    initscr();
    noecho();
    cbreak();
    curs_set(0);

    while (1) {
        get_memory_info(&memory, &swap);
        get_uptime(&uptime);
        get_network_info(&network);

        clear();

        show_memory(&memory);
        show_swap(&swap);
        show_uptime(&uptime);
        show_network(&network);

        refresh();

        sleep(1);
    }

    endwin();
}