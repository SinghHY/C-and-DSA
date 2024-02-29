#ifndef _SYSTICK_H
#define _SYSTICK_H
#include <cstdint>

void  systick_init_1sec(void);
int get_count_flag(void);


void systick_delayMs(std::uint32_t dly);

#endif