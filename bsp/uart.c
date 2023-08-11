#include "uart.h"
#include "stc8h.h"

#define Baudrate            9600
#define UART_BUF_LENGTH    (32 + 1)

uint8_t idata g_rx_status = 0;                   //����״̬
uint8_t idata g_rx_buf[UART_BUF_LENGTH] = {0};   //���ջ���

static void _SetTimer2Baudraye(u16 dat)
{
    AUXR &= ~(1<<4);    //Timer stop
    AUXR &= ~(1<<3);    //Timer2 set As Timer
    AUXR |=  (1<<2);    //Timer2 set as 1T mode
    TH2 = dat / 256;
    TL2 = dat % 256;
    IE2  &= ~(1<<2);    //��ֹ�ж�
    AUXR |=  (1<<4);    //Timer run enable
}

void uart_init(void)
{
    P_SW1 &= 0x3F;  /* RxD = P3.0, TxD = P3.1 */

    P3M1 &= ~(1 << 0);  /* ׼˫��� */
    P3M0 &= ~(1 << 0);

    P3M1 &= ~(1 << 1);  /* ׼˫��� */
    P3M0 &= ~(1 << 1);

    AUXR |= 0x01;       //S1 BRT Use Timer2;
    SetTimer2Baudraye(65536UL - (MAIN_Fosc / 4) / Baudrate);

    SCON = (SCON & 0x3f) | 0x40;    //UART1ģʽ, 0x00: ͬ����λ���, 0x40: 8λ����,�ɱ䲨����, 0x80: 9λ����,�̶�������, 0xc0: 9λ����,�ɱ䲨����
    REN = 1;    //�������
    ES  = 1;    //�����ж�

    g_rx_status = 0;
}

void print_string(u8 *puts)
{
    for (; *puts != 0;  puts++)     //����ֹͣ��0����
    {
        SBUF = *puts;
        while(!TI);
        TI = 0;
    }
}

/**
 * @brief ��дprintf���õ�putchar�������ض��򵽴������,��Ҫ����ͷ�ļ�<stdio.h>
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
        if ((g_rx_status & 0x80) == 0)      /* ����δ��� */
        {
            if (g_rx_status & 0x40)         /* ���յ���0x0D */
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
            else                            /* ��û�յ�0x0D */
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