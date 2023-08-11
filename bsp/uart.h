#ifndef __UART_H
#define __UART_H

#include "type.h"

extern uint8_t idata g_rx_status;
extern uint8_t idata g_rx_buf[];

void uart_init(void);
void print_string(u8 *puts);

#endif
