#ifndef _timers_H

#define _timers_H

#include "SystemClock.h"


#define TIMER     1
#define TIMER0_SRC_CLK	(MCU_SYSCLK/12)

#define TIMER_1T_1ms_TH	((65536-(u16)(float)(1000*((float)(TIMER0_SRC_CLK)/(float)(1000000)))) /256) 			
#define TIMER_1T_1ms_TL	((65536-(u16)(float)(1000*((float)(TIMER0_SRC_CLK)/(float)(1000000)))) %256)

#define TIMER_1T_50ms_TH	((65536-(u16)(float)(50000*((float)(TIMER0_SRC_CLK)/(float)(1000000)))) /256) 			
#define TIMER_1T_50ms_TL	((65536-(u16)(float)(50000*((float)(TIMER0_SRC_CLK)/(float)(1000000)))) %256)
	
#define SetTimerTH(x)     ((65536-(u16)(float)((x*1000)*((float)(TIMER0_SRC_CLK)/(float)(1000000)))) /256)      
#define SetTimerTL(x)     ((65536-(u16)(float)((x*1000)*((float)(TIMER0_SRC_CLK)/(float)(1000000)))) %256)
	
#define TimerStop()       TM_DisableT0();TM_SetT0LowByte(SetTimerTL(TIMER));TM_SetT0HighByte(SetTimerTH(TIMER)); 
#define TimerStart()     	TM_EnableT0();

void Timer0_INIT(void);

#endif