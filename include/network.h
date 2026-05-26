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
    unsigned long rx_rate;
    
    unsigned long tx;
    unsigned long prev_tx;
    unsigned long tx_rate;

    int initialized;
};

/**
 * @brief Function that reads /proc/net/dev
 * 
 * @param network Struct to store network state
 * 
 * @return void
 * 
 */
void get_network_info(struct Network *network);

#endif