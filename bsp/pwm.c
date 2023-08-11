#include "pwm.h"
#include "stc8h.h"

#define Timer1_Reload   (MAIN_Fosc / 10000)      //Timer 1 中断频率, 10000次/秒

static uint8_t idata _duty[SEG_NUM] = {0, 0, 0, 0};

void pwm_init(void)
{
    P3M1 &= 0x0F;
    P3M0 &= 0x0F;
    P_SW2 |= 0x80;  /* 使能访问XFR */
    P3PU |= 0xF0;   /* 使能内部4.1k上拉电阻 */

    TR1 = 0;    //停止计数
    ET1 = 1;    //允许中断
    
    TMOD &= ~0x03;
    TMOD |= 0;          //工作模式, 0: 16位自动重装, 1: 16位定时/计数, 2: 8位自动重装, 3: 16位自动重装, 不可屏蔽中断
    TMOD &= ~0x04;      //定时
    
    INT_CLKO &= ~0x02;  //不输出时钟
    
    AUXR |=  0x40;  //1T mode
    TH1 = (u8)((65536UL - Timer1_Reload) / 256);
    TL1 = (u8)((65536UL - Timer1_Reload) % 256);

    TR1 = 1;    //开始运行
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