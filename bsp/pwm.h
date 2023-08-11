#ifndef __PWM_H
#define __PWM_H

#include "type.h"

void pwm_init(void);
void pwm_set_duty(uint8_t addr, uint8_t duty);

#endif
