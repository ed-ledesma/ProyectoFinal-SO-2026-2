#ifndef NETWORK_H
#define NETWORK_H

/**
 * @struct Network
 * @brief State of network
 * 
 */
struct Network {
    unsigned long rx;
    unsigned long prev_rx;
    unsigned long tx;
    unsigned long prev_tx;
};

/**
 * @brief Function that reads /proc/net/dev
 * 
 * @param network Struct to store network state
 * 
 * @return void
 * 
 */
void get_network_info(struct Network network);

#endif