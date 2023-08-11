#include "disp.h"
#include "stc8h.h"
#include "pwm.h"

static uint8_t idata _light_level[SEG_NUM]   = {6, 6, 6, 6};
static uint8_t idata _disp_code[SEG_NUM]     = {0, 0, 0, 0};
static uint8_t idata _flick_status[SEG_NUM]  = {1, 1, 1, 1};

void _disp_set_addr(uint8_t addr)
{
    uint8_t i;
    
    if (addr >= SEG_NUM)
    {
        return;
    }
    
    P3 &= 0x0F;
    for (i = 0; i < SEG_NUM; i++)
    {
        pwm_set_duty(i, 0);
    }
    pwm_set_duty(addr, _light_level[addr]);
}

void disp_set_light_level(uint8_t light_level[SEG_NUM])
{
    uint8_t i;
    
    for (i = 0; i < SEG_NUM; i++)
    {
        _light_level[i] = light_level[i];
    }
}

void disp_set_code(uint8_t dat[SEG_NUM])
{
    uint8_t i;
    
    for (i = 0; i < SEG_NUM; i++)
    {
        _disp_code[i] = dat[i];
    }
}

void disp_set_string(char *str)
{
    uint8_t i, j;
    
    for (i = 0, j = 0; j < SEG_NUM && str[i] != '\0'; i++, j++)
    {
        switch (str[i])
        {
            case '0': _disp_code[j] = 0x3F; break;
            case '1': _disp_code[j] = 0x06; break;
            case '2': _disp_code[j] = 0x5B; break;
            case '3': _disp_code[j] = 0x4F; break;
            case '4': _disp_code[j] = 0x66; break;
            case '5': _disp_code[j] = 0x6D; break;
            case '6': _disp_code[j] = 0x7D; break;
            case '7': _disp_code[j] = 0x07; break;
            case '8': _disp_code[j] = 0x7F; break;
            case '9': _disp_code[j] = 0x6F; break;
            case 'A':
            case 'a': _disp_code[j] = 0x77; break;
            case 'B':
            case 'b': _disp_code[j] = 0x7C; break;
            case 'C': _disp_code[j] = 0x39; break;
            case 'c': _disp_code[j] = 0x58; break;
            case 'D':
            case 'd': _disp_code[j] = 0x5E; break;
            case 'E':
            case 'e': _disp_code[j] = 0x79; break;
            case 'F':
            case 'f': _disp_code[j] = 0x71; break;
            case 'G':
            case 'g': _disp_code[j] = 0x3d; break;
            case 'H': _disp_code[j] = 0x76; break;
            case 'h': _disp_code[j] = 0x74; break;
            case 'J':
            case 'j': _disp_code[j] = 0x1E; break;
            case 'K':
            case 'k': _disp_code[j] = 0x70; break;
            case 'L':
            case 'l': _disp_code[j] = 0x38; break;
            case 'N':
            case 'n': _disp_code[j] = 0x37; break;
            case 'O': _disp_code[j] = 0x3F; break;
            case 'o': _disp_code[j] = 0x5C; break;
            case 'P':
            case 'p': _disp_code[j] = 0x73; break;
            case 'Q':
            case 'q': _disp_code[j] = 0x67; break;
            case 'R':
            case 'r': _disp_code[j] = 0x50; break;
            case 'T':
            case 't': _disp_code[j] = 0x78; break;
            case 'U':
            case 'u': _disp_code[j] = 0x3E; break;
            case 'Y':
            case 'y': _disp_code[j] = 0x6e; break;            
            case ' ': _disp_code[j] = 0x00; break;
            case '-': _disp_code[j] = 0x40; break;
            default: _disp_code[j] = 0x00; break;
        }
        if (str[i+1] == '.')
        {
            _disp_code[j] |= 0x80;
            i++;
        }
    }
    if (j != SEG_NUM)
    {
        for (; j < SEG_NUM; j++)
        {
            _disp_code[j] = 0x00;
        }
    }
}

void disp_set_flick_status(uint8_t addr, uint8_t status)
{
    if (addr >= SEG_NUM || status > 1)
    {
        return;
    }
    _flick_status[addr] = status;
}

uint8_t disp_get_flick_status(uint8_t addr)
{
    if (addr >= SEG_NUM)
    {
        return 0;
    }
    return _flick_status[addr];
}

void disp_scan(void)
{
    static uint8_t addr = 0;

    P1 = (_flick_status[addr] == 1) ? _disp_code[addr] : 0x00;
    _disp_set_addr(addr);
    
    addr++;
    if (addr == SEG_NUM)
    {
        addr = 0;
    }
}