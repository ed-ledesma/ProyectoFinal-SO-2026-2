#include <unistd.h>
#include <ncurses.h>

#include "memory.h"
#include "ui.h"
#include "uptime.h"
#include "network.h"
#include "disk.h"
#include "cpu.h"
#include "process.h"

#define STATS_ROWS 7

void show_memory(const struct Memory *memory) {
    int col = getmaxx(stdscr) / 3 * 0;
    mvprintw(0, col + 2, "Memory");
    mvprintw(1, col + 4, "Total:  %lu MB", memory->mem_total / 1024);
    mvprintw(2, col + 4, "Free:   %lu MB", memory->mem_free / 1024);
    mvprintw(3, col + 4, "Avail:  %lu MB", memory->mem_available / 1024);
}

void show_swap(const struct Swap *swap) {
    int col = getmaxx(stdscr) / 3 * 1;
    mvprintw(0, col + 2, "Swap");
    mvprintw(1, col + 4, "Total:  %lu MB", swap->swap_total / 1024);
    mvprintw(2, col + 4, "Free:   %lu MB", swap->swap_free / 1024);
}

void show_uptime(const struct Uptime *uptime) {

    int col =
        (getmaxx(stdscr) / 3) * 2;

    long total_seconds =
        (long) uptime->time;

    int days =
        total_seconds / 86400;

    int hours =
        (total_seconds % 86400) / 3600;

    int minutes =
        (total_seconds % 3600) / 60;

    int seconds =
        total_seconds % 60;

    mvprintw(0, col + 2, "Uptime");

    if (days > 0) {

        mvprintw(
            1,
            col + 4,

            "%dd %dh %dm",

            days,
            hours,
            minutes
        );

    } else if (hours > 0) {

        mvprintw(
            1,
            col + 4,

            "%dh %dm",

            hours,
            minutes
        );

    } else if (minutes > 0) {

        mvprintw(
            1,
            col + 4,

            "%dm %ds",

            minutes,
            seconds
        );

    } else {

        mvprintw(
            1,
            col + 4,

            "%ds",

            seconds
        );
    }
}

void show_cpu(const struct CPU *cpu) {
    int col = getmaxx(stdscr) / 3 * 2;
    mvprintw(2, col + 2, "CPU");
    mvprintw(3, col + 4, "Usage:  %.2f%%", cpu->usage);
}

void show_network(const struct Network *network) {
    int col = getmaxx(stdscr) / 3 * 0;
    mvprintw(4, col + 2, "Network");
    mvprintw(5, col + 4, "RX: %lu MB/s   TX: %lu MB/s",
        network->rx_rate / (1024*1024),
        network->tx_rate / (1024*1024));
    mvprintw(6, col + 4, "TRX: %lu MB TTX: %lu MB",
        network->rx / (1024*1024),
        network->tx / (1024*1024));
}

void show_disk(const struct Disk *disk) {
    int col = getmaxx(stdscr) / 3 * 1;
    mvprintw(4, col + 2, "Disk");
    mvprintw(5, col + 4, "RX: %lu MB/s     WX: %lu MB/s",
        disk->read_rate  / (1024*1024),
        disk->write_rate / (1024*1024));
    mvprintw(6, col + 4, "TRX: %lu MB  TWX: %lu MB",
        disk->read  / (1024*1024),
        disk->write / (1024*1024));
}

void show_processes(const struct ProcessList *plist) {

    int rows, cols;

    getmaxyx(stdscr, rows, cols);

    int start_row = STATS_ROWS;

    mvhline(start_row, 0, ACS_HLINE, cols);

    /* Header */

    mvprintw(
        start_row + 1,
        2,

        "%6s "
        "%-12s "
        "%-2s "
        "%7s "
        "%7s "
        "%12s "
        "%10s "
        "%10s "
        "%s",

        "PID",
        "USER",
        "S",

        "%CPU",
        "%MEM",

        "VSZ",
        "RSS",

        "TIME+",
        "COMMAND"
    );

    int visible_processes =
        rows - (start_row + 3);

    if (visible_processes < 0)
        visible_processes = 0;

    if (visible_processes > plist->count)
        visible_processes = plist->count;

    long ticks =
        sysconf(_SC_CLK_TCK);

    for (int i = 0;
         i < visible_processes;
         i++) {

        const struct Process *p =
            &plist->processes[i];

        /* Total CPU time */

        unsigned long total_ticks =
            p->utime + p->stime;

        unsigned long total_seconds =
            total_ticks / ticks;

        /* HH:MM:SS */

        int hours   = total_seconds / 3600;
        int minutes = (total_seconds % 3600) / 60;
        int seconds = total_seconds % 60;

        mvprintw(
            start_row + 2 + i,
            2,

            "%6d "
            "%-12.12s "
            "%-2c "
            "%7.1f "
            "%7.1f "
            "%12lu "
            "%10ld "
            "%02d:%02d:%02d "
            "%-.20s",

            p->pid,
            p->user,
            p->state,

            p->cpu_percent,
            p->mem_percent,

            p->vsize / 1024,
            p->rss   / 1024,

            hours,
            minutes,
            seconds,

            p->name
        );
    }
}

void ncurses_ui(void) {

    struct Memory      memory  = {0};
    struct Swap        swap    = {0};
    struct Uptime      uptime  = {0};
    struct Network     network = {0};
    struct Disk        disk    = {0};
    struct CPU         cpu     = {0};
    struct ProcessList plist   = {0};

    initscr();
    noecho();
    cbreak();
    curs_set(0);

    init_process_list(&plist);

    while (1) {

        get_memory_info(&memory, &swap);
        get_uptime(&uptime);
        get_network_info(&network);
        get_disk_info(&disk);
        get_cpu_info(&cpu);
        get_process_info(&plist);

        clear();

        show_memory(&memory);
        show_swap(&swap);
        show_uptime(&uptime);
        show_cpu(&cpu);
        show_network(&network);
        show_disk(&disk);
        show_processes(&plist);

        refresh();

        sleep(1);
    }

    destroy_process_list(&plist);

    endwin();
}