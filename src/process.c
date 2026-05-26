#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <pwd.h>

#include "process.h"

static unsigned long prev_process_times[MAX_PID];

/**
 * @brief Auxiliar function that checks if char is numeric
 */
static int is_numeric(const char *str) {

    while (*str) {

        if (!isdigit(*str))
            return 0;

        str++;
    }

    return 1;
}

/**
 * @brief Auxiliar function that compares two processes cpu usage
 */
static int compare_cpu(const void *a,
                       const void *b) {

    const struct Process *p1 = a;
    const struct Process *p2 = b;

    if (p2->cpu_percent > p1->cpu_percent)
        return 1;

    if (p2->cpu_percent < p1->cpu_percent)
        return -1;

    return 0;
}

/**
 * @brief Auxiliar function that gets CPU time
 */
static unsigned long get_total_cpu_time(void) {

    FILE *fp = fopen("/proc/stat", "r");

    if (!fp)
        return 0;

    char line[512];

    unsigned long user;
    unsigned long nice;
    unsigned long system;
    unsigned long idle;
    unsigned long iowait;
    unsigned long irq;
    unsigned long softirq;
    unsigned long steal;

    fgets(line, sizeof(line), fp);

    fclose(fp);

    sscanf(
        line,
        "cpu %lu %lu %lu %lu %lu %lu %lu %lu",
        &user,
        &nice,
        &system,
        &idle,
        &iowait,
        &irq,
        &softirq,
        &steal
    );

    return
        user +
        nice +
        system +
        idle +
        iowait +
        irq +
        softirq +
        steal;
}

void init_process_list(struct ProcessList *plist) {

    plist->count = 0;

    plist->capacity = INITIAL_CAPACITY;

    plist->prev_system_total = 0;

    plist->processes =
        malloc(
            sizeof(struct Process) *
            plist->capacity
        );

    if (!plist->processes) {

        perror("malloc");

        exit(EXIT_FAILURE);
    }
}

void destroy_process_list(struct ProcessList *plist) {

    free(plist->processes);

    plist->processes = NULL;

    plist->count = 0;

    plist->capacity = 0;
}

void get_process_info(struct ProcessList *plist) {

    DIR *proc = opendir("/proc");

    if (!proc)
        return;

    plist->count = 0;

    long total_mem =
        sysconf(_SC_PHYS_PAGES) * // Number of pages in RAM
        sysconf(_SC_PAGESIZE);    // Page size

    unsigned long system_total =
        get_total_cpu_time();

    unsigned long system_delta =
        system_total -
        plist->prev_system_total;

    struct dirent *entry;

    while ((entry = readdir(proc)) != NULL) {

        // Ignore non-PID entries

        if (!is_numeric(entry->d_name))
            continue;

        int pid = atoi(entry->d_name);

        // Ignore invalid PID

        if (pid < 0 || pid >= MAX_PID)
            continue;

        // Expand dynamic array if needed

        if (plist->count >= plist->capacity) {

            if (plist->capacity == 0)
                plist->capacity = INITIAL_CAPACITY;
            else
                plist->capacity *= 2;

            struct Process *tmp =
                realloc(
                    plist->processes,
                    sizeof(struct Process) *
                    plist->capacity
                );

            if (!tmp) {

                perror("realloc");

                break;
            }

            plist->processes = tmp;
        }

        struct Process *p =
            &plist->processes[plist->count];

        memset(p, 0, sizeof(struct Process));

        p->pid = pid;

        char path[256];

        // Read process name

        snprintf(
            path,
            sizeof(path),
            "/proc/%d/comm",
            pid
        );

        FILE *fp = fopen(path, "r");

        if (fp) {

            fgets(
                p->name,
                sizeof(p->name),
                fp
            );

            p->name[
                strcspn(p->name, "\n")
            ] = '\0';

            fclose(fp);
        }

        // Read process stats

        snprintf(
            path,
            sizeof(path),
            "/proc/%d/stat",
            pid
        );

        fp = fopen(path, "r");

        if (fp) {

            char buffer[2048];

            if (fgets(
                    buffer,
                    sizeof(buffer),
                    fp)) {

                char *right_paren =
                    strrchr(buffer, ')');

                if (right_paren) {

                    // Dummy variables for unused fields
                    int       f_ppid, f_pgrp, f_session,
                              f_tty_nr, f_tpgid;
                    unsigned  f_flags;
                    unsigned long f_minflt, f_cminflt,
                                  f_majflt, f_cmajflt;
                    long      f_cutime, f_cstime,
                              f_priority, f_nice,
                              f_num_threads, f_itrealvalue;
                    unsigned long long f_starttime;

                    int n = sscanf(
                        right_paren + 2,

                        "%c "               //  3 state
                        "%d %d %d %d %d "   //  4-8
                        "%u "               //  9 flags
                        "%lu %lu %lu %lu "  // 10-13
                        "%lu %lu "          // 14-15 utime,stime
                        "%ld %ld "          // 16-17 cutime,cstime
                        "%ld %ld "          // 18-19 priority,nice
                        "%ld "              // 20 num_threads
                        "%ld "              // 21 itrealvalue
                        "%llu "             // 22 starttime
                        "%lu "              // 23 vsize
                        "%ld",              // 24 rss (pages)

                        &p->state,
                        &f_ppid, &f_pgrp, &f_session,
                            &f_tty_nr, &f_tpgid,
                        &f_flags,
                        &f_minflt, &f_cminflt,
                            &f_majflt, &f_cmajflt,
                        &p->utime, &p->stime,
                        &f_cutime, &f_cstime,
                        &f_priority, &f_nice,
                        &f_num_threads,
                        &f_itrealvalue,
                        &f_starttime,
                        &p->vsize,
                        &p->rss
                    );

                    if (n != 22) {
                        // Parsing failed, skip process
                        fclose(fp);
                        continue;
                    }
                }
            }

            fclose(fp);
        }

        // Read user that owns process

        snprintf(
            path,
            sizeof(path),
            "/proc/%d/status",
            pid
        );

        fp = fopen(path, "r");

        if (fp) {

            char line[256];

            while (fgets(
                       line,
                       sizeof(line),
                       fp)) {

                if (strncmp(line, "Uid:", 4) == 0) {

                    uid_t uid;

                    sscanf(
                        line,
                        "Uid:\t%d",
                        &uid
                    );

                    struct passwd *pw =
                        getpwuid(uid);

                    if (pw) {

                        strncpy(
                            p->user,
                            pw->pw_name,
                            sizeof(p->user) - 1
                        );

                        p->user[
                            sizeof(p->user) - 1
                        ] = '\0';
                    }

                    break;
                }
            }

            fclose(fp);
        }

        // Convert RSS pages to bytes

        p->rss *= sysconf(_SC_PAGESIZE); // Syscall to get pagesize

        // Memory percent

        p->mem_percent =
            ((float)p->rss /
            (float)total_mem) * 100.0f;

        // CPU percent

        unsigned long current_total_time =
            p->utime + p->stime;

        unsigned long prev_total_time =
            prev_process_times[pid];

        unsigned long process_delta =
            current_total_time -
            prev_total_time;

        if (system_delta > 0) {

            p->cpu_percent =
                ((float)process_delta /
                (float)system_delta)
                * 100.0f;

        } else {

            p->cpu_percent = 0.0f;
        }

        // Save process CPU time

        prev_process_times[pid] =
            current_total_time;

        plist->count++;
    }

    closedir(proc);

    plist->prev_system_total =
        system_total;

    qsort(
        plist->processes,
        plist->count,
        sizeof(struct Process),
        compare_cpu
    );
}