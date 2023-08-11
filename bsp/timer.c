#include "timer.h"
#include "stc8h.h"
#include "disp.h"
#include "uart.h"


#define Timer0_Reload   (MAIN_Fosc / 1000)      //Timer 0 中断频率, 1000次/秒

static uint16_t idata _flick_cycle[SEG_NUM] = {0};   /* 闪烁半周期，单位ms */


void timer0_init(void)
{
    TR0 = 0;    //停止计数
    ET0 = 1;    //允许中断
    
    TMOD &= ~0x03;
    TMOD |= 0;          //工作模式, 0: 16位自动重装, 1: 16位定时/计数, 2: 8位自动重装, 3: 16位自动重装, 不可屏蔽中断
    TMOD &= ~0x04;      //定时
    
    INT_CLKO &= ~0x01;  //不输出时钟
    
    AUXR |=  0x80;  //1T mode
    TH0 = (u8)((65536UL - Timer0_Reload) / 256);
    TL0 = (u8)((65536UL - Timer0_Reload) % 256);

    TR0 = 1;    //开始运行
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