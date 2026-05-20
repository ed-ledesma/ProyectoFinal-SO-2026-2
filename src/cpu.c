#include <stdio.h>

#include "cpu.h"

void get_cpu_info(struct CPU *cpu) {
	if (!cpu) return;

	FILE *fp = fopen("/proc/stat", "r");
	if (!fp) return;

	char line[256];
	// 
	unsigned long user = 0;
	unsigned long nice = 0;
	unsigned long system = 0;
	unsigned long idle = 0;
	unsigned long iowait = 0;
	unsigned long irq = 0;
	unsigned long softirq = 0;
	unsigned long steal = 0;
	unsigned long guest = 0;
	unsigned long guest_nice = 0;
	unsigned long total = 0;
	unsigned long idle_all = 0;
	unsigned long delta_total = 0;
	unsigned long delta_idle = 0;

	cpu->cpu = 0;
	cpu->usage = 0.0;

	if (fgets(line, sizeof(line), fp) != NULL) {
		if (sscanf(
			line,
			"cpu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
			&user,
			&nice,
			&system,
			&idle,
			&iowait,
			&irq,
			&softirq,
			&steal,
			&guest,
			&guest_nice
		) >= 4) {
			total = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
			idle_all = idle + iowait;

			cpu->cpu = total;

			if (cpu->prev_total > 0 && total >= cpu->prev_total && idle_all >= cpu->prev_idle) {
				delta_total = total - cpu->prev_total;
				delta_idle = idle_all - cpu->prev_idle;
				if (delta_total > 0) {
					cpu->usage = (double)(delta_total - delta_idle) * 100.0 / (double)delta_total;
				}
			}

			cpu->prev_total = total;
			cpu->prev_idle = idle_all;
		}
	}

	fclose(fp);
}