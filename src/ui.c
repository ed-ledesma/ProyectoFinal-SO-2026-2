#include <unistd.h>
#include <ncurses.h>

#include "memory.h"
#include "ui.h"
#include "uptime.h"
#include "network.h"
#include "disk.h"
#include "cpu.h"

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
    mvprintw(14, 4, "Receiving:       %lu MB", network->rx_rate / (1024*1024));
    mvprintw(15, 4, "Total Received:  %lu MB", network->rx / (1024*1024));
    mvprintw(16, 4, "Sending:         %lu MB", network->tx_rate / (1024*1024));
    mvprintw(17, 4, "Total Sent:      %lu MB", network->tx / (1024*1024));   
}

void show_disk(const struct Disk *disk) {
    mvprintw(19, 2, "Disk");
    mvprintw(20, 4, "Reading:        %lu MB", disk->read_rate / (1024*1024));
    mvprintw(21, 4, "Total Read:     %lu MB", disk->read / (1024*1024));
    mvprintw(22, 4, "Writing:        %lu MB", disk->write_rate / (1024*1024));
    mvprintw(23, 4, "Total Written:  %lu MB", disk->write / (1024*1024));   
}

void show_cpu(const struct CPU *cpu) {
    mvprintw(25, 2, "CPU");
    mvprintw(26, 4, "Usage:      %.2f%%", cpu->usage);
}

void ncurses_ui(void) {
    struct Memory memory = {0};
    struct Swap swap = {0};
    struct Uptime uptime = {0};
    struct Network network = {0};
    struct Disk disk = {0};
    struct CPU cpu = {0};

    initscr();
    noecho();
    cbreak();
    curs_set(0);

    while (1) {
        get_memory_info(&memory, &swap);
        get_uptime(&uptime);
        get_network_info(&network);
        get_disk_info(&disk);
        get_cpu_info(&cpu);

        clear();

        show_memory(&memory);
        show_swap(&swap);
        show_uptime(&uptime);
        show_network(&network);
        show_disk(&disk);
        show_cpu(&cpu);

        refresh();

        sleep(1);
    }

    endwin();
}