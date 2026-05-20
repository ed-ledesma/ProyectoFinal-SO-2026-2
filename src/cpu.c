#include <stdio.h>
#include <string.h>

#include "cpu.h"

void get_cpu_info(struct CPU *cpu) {
	FILE *fp = fopen("/proc/stat", "r");
	if (!fp) return;

	char key[64];
	unsigned long value;
	char unit[16];

	//Initialize to 0
	cpu->algo 		= 0;

	while (fscanf(fp, "%63s %lu"))
}
