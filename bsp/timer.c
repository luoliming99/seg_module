#include "timer.h"
#include "stc8h.h"
#include "disp.h"
#include "uart.h"


#define Timer0_Reload   (MAIN_Fosc / 1000)      //Timer 0 �ж�Ƶ��, 1000��/��

static uint16_t idata _flick_cycle[SEG_NUM] = {0};   /* ��˸�����ڣ���λms */


void timer0_init(void)
{
    TR0 = 0;    //ֹͣ����
    ET0 = 1;    //�����ж�
    
    TMOD &= ~0x03;
    TMOD |= 0;          //����ģʽ, 0: 16λ�Զ���װ, 1: 16λ��ʱ/����, 2: 8λ�Զ���װ, 3: 16λ�Զ���װ, ���������ж�
    TMOD &= ~0x04;      //��ʱ
    
    INT_CLKO &= ~0x01;  //�����ʱ��
    
    AUXR |=  0x80;  //1T mode
    TH0 = (u8)((65536UL - Timer0_Reload) / 256);
    TL0 = (u8)((65536UL - Timer0_Reload) % 256);

    TR0 = 1;    //��ʼ����
}

void disp_set_flick_half_cycle(uint16_t flick_cycle[SEG_NUM])
{
    uint8_t i;
    
    for (i = 0; i < SEG_NUM; i++)
    {
        _flick_cycle[i] = flick_cycle[i];
        disp_set_flick_status(i, 1);
    }
}

void timer0_isr(void) interrupt 1
{
    static uint32_t cnt = 0;
    uint8_t i;
    
    if ((cnt % 5) == 0)
    {
        disp_scan();
    }
    
    for (i = 0; i < SEG_NUM; i++)
    {
        if (_flick_cycle[i] != 0 && (cnt % _flick_cycle[i]) == 0)
        {
            disp_set_flick_status(i, !disp_get_flick_status(i));
        }
    }
    
    cnt++;
    if (cnt == 1000000)
    {
        cnt = 0;
    } 
}