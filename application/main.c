#include "stc8h.h"
#include "type.h"
#include "delay.h"
#include "uart.h"
#include "timer.h"
#include "pwm.h"
#include "disp.h"
#include "proc.h"


void main()
{
    P1M1 = 0x00;
    P1M0 = 0xFF;
    P1 = 0x00;
    
    uart_init();
    timer0_init();

    pwm_init();
    EA = 1;

    while (1)
    {
        if (g_rx_status & 0x80)
        {
            proc_rx_decode_exe();
            g_rx_status = 0;
        }
//        delay_ms(1000);
    }
}