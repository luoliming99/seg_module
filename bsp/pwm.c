#include "pwm.h"
#include "stc8h.h"

#define Timer1_Reload   (MAIN_Fosc / 10000)      //Timer 1 �ж�Ƶ��, 10000��/��

static uint8_t idata _duty[SEG_NUM] = {0, 0, 0, 0};

void pwm_init(void)
{
    P3M1 &= 0x0F;
    P3M0 &= 0x0F;
    P_SW2 |= 0x80;  /* ʹ�ܷ���XFR */
    P3PU |= 0xF0;   /* ʹ���ڲ�4.1k�������� */

    TR1 = 0;    //ֹͣ����
    ET1 = 1;    //�����ж�
    
    TMOD &= ~0x03;
    TMOD |= 0;          //����ģʽ, 0: 16λ�Զ���װ, 1: 16λ��ʱ/����, 2: 8λ�Զ���װ, 3: 16λ�Զ���װ, ���������ж�
    TMOD &= ~0x04;      //��ʱ
    
    INT_CLKO &= ~0x02;  //�����ʱ��
    
    AUXR |=  0x40;  //1T mode
    TH1 = (u8)((65536UL - Timer1_Reload) / 256);
    TL1 = (u8)((65536UL - Timer1_Reload) % 256);

    TR1 = 1;    //��ʼ����
}

void pwm_set_duty(uint8_t addr, uint8_t duty)
{
    if (duty > 10)
    {
        _duty[addr] = 10;
    }
    else
    {
        _duty[addr] = duty;
    }
}

void timer1_isr (void) interrupt 3
{
    static uint8_t cnt = 0;
    uint8_t i;
    
    for (i = 0; i < SEG_NUM; i++)
    {
        if (cnt < _duty[i])
        {
            P3 |= 1 << (i + 4);
        }
        else
        {
            P3 &= ~(1 << (i + 4));
        }
    }
    
    cnt++;
    if (cnt == 10)
    {
        cnt = 0;
    }
}