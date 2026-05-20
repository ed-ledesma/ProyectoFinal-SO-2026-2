#include <stdio.h>
#include <string.h>

#include "uptime.h"

void get_uptime(struct Uptime *uptime) {
    FILE *fp = fopen("/proc/uptime", "r");
    if (!fp) return;

    double value;

    // Initialize to 0
    uptime->time    = 0;

    fscanf(fp, "%lf", &value);

    uptime->time = value;

    fclose(fp);
}