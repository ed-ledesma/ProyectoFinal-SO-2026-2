#include <stdio.h>
#include <string.h>

#include "network.h"

void get_network_info(struct Network *network) {
    FILE *fp = fopen("/proc/net/dev", "r");
    if (!fp) return;

    char line[512];
    char interface[32];

    unsigned long rx_bytes;
    unsigned long tx_bytes;

    // Dummy for unused fields
    unsigned long dummy[14];

    // Save previous values
    network->prev_rx = network->rx;
    network->prev_tx = network->tx;

    fgets(line, sizeof(line), fp);
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp)) {

        sscanf(
            line,
            " %31[^:]: %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
            interface,
            &rx_bytes,
            &dummy[0],
            &dummy[1],
            &dummy[2],
            &dummy[3],
            &dummy[4],
            &dummy[5],
            &dummy[6],
            &tx_bytes,
            &dummy[7],
            &dummy[8],
            &dummy[9],
            &dummy[10],
            &dummy[11],
            &dummy[12],
            &dummy[13]
        );

        if (strcmp(interface, "lo") == 0)
            continue;
        
        if (!network->initialized) {
            network->prev_rx = rx_bytes;
            network->prev_tx = tx_bytes;

            network->rx_rate = 0;
            network->tx_rate = 0;

            network->initialized = 1;
        } else {
            network->rx_rate = rx_bytes - network->prev_rx;
            network->tx_rate = tx_bytes - network->prev_tx;

            network->prev_rx = rx_bytes;
            network->prev_tx = tx_bytes;
        }

        network->rx = rx_bytes;
        network->tx = tx_bytes;

        break;
    }

    fclose(fp);
}