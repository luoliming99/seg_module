#ifndef __DISP_H
#define __DISP_H

#include "type.h"

void disp_set_light_level(uint8_t light_level[SEG_NUM]);
void disp_set_code(uint8_t dat[SEG_NUM]);
void disp_set_string(char *str);
void disp_set_flick_status(uint8_t addr, uint8_t status);
uint8_t disp_get_flick_status(uint8_t addr);
void disp_scan(void);

#endif
