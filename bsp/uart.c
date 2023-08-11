#include "uart.h"
#include "stc8h.h"

#define Baudrate            9600
#define UART_BUF_LENGTH    (32 + 1)

uint8_t idata g_rx_status = 0;                   //接收状态
uint8_t idata g_rx_buf[UART_BUF_LENGTH] = {0};   //接收缓冲

static void _SetTimer2Baudraye(u16 dat)
{
    AUXR &= ~(1<<4);    //Timer stop
    AUXR &= ~(1<<3);    //Timer2 set As Timer
    AUXR |=  (1<<2);    //Timer2 set as 1T mode
    TH2 = dat / 256;
    TL2 = dat % 256;
    IE2  &= ~(1<<2);    //禁止中断
    AUXR |=  (1<<4);    //Timer run enable
}

void uart_init(void)
{
    P_SW1 &= 0x3F;  /* RxD = P3.0, TxD = P3.1 */

    P3M1 &= ~(1 << 0);  /* 准双向口 */
    P3M0 &= ~(1 << 0);

    P3M1 &= ~(1 << 1);  /* 准双向口 */
    P3M0 &= ~(1 << 1);

    AUXR |= 0x01;       //S1 BRT Use Timer2;
    SetTimer2Baudraye(65536UL - (MAIN_Fosc / 4) / Baudrate);

    SCON = (SCON & 0x3f) | 0x40;    //UART1模式, 0x00: 同步移位输出, 0x40: 8位数据,可变波特率, 0x80: 9位数据,固定波特率, 0xc0: 9位数据,可变波特率
    REN = 1;    //允许接收
    ES  = 1;    //允许中断

    g_rx_status = 0;
}

void print_string(u8 *puts)
{
    for (; *puts != 0;  puts++)     //遇到停止符0结束
    {
        SBUF = *puts;
        while(!TI);
        TI = 0;
    }
}

/**
 * @brief 重写printf调用的putchar函数，重定向到串口输出,需要引入头文件<stdio.h>
 */
//char putchar(char c)
//{
//    SBUF = c;
//    while(!TI);
//    TI = 0;
//    return c;
//}

void UART1_isr (void) interrupt 4
{
    if(RI)
    {
        RI = 0;
        if ((g_rx_status & 0x80) == 0)      /* 接收未完成 */
        {
            if (g_rx_status & 0x40)         /* 接收到了0x0D */
            {
                if (0x0A != SBUF)
                {
                    g_rx_status = 0;
                }
                else
                {
                    g_rx_status |= 0x80;
                    g_rx_buf[g_rx_status & 0x3F] = '\0';
                }
            }
            else                            /* 还没收到0x0D */
            {
                if (0x0D == SBUF)
                {
                    g_rx_status |= 0x40;
                }
                else
                {
                    g_rx_buf[g_rx_status & 0x3F] = SBUF;
                    g_rx_status++;
                    if (g_rx_status >= UART_BUF_LENGTH)
                    {
                        g_rx_status = 0;
                    }
                }
            }
        }
    }
}