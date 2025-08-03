#include <REG_MG82F6D17.h>
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"


#include "timers.h"
#include "Dice.h"
#include "LedBoard.h"
#include "UserInput.h"
#include "Buzzer.h"
#include "USER.h" 

void Timer0_INIT(void)
{
	TM_SetT0Mode_1_16BIT_TIMER();					     // TIMER0 Mode: 16-Bit
	TM_SetT0Clock_SYSCLKDiv12();					     // TIMER0 Clock source: TIMER0_SRC_CLK = SYSCLK/12
	TM_SetT0Gate_Disable();							       // TIMER0 disable gate
	
	TM_SetT0LowByte(SetTimerTL(TIMER));			   // Set TL0 value
	TM_SetT0HighByte(SetTimerTH(TIMER));		   // Set TH0 value IN Milli second
	
	TM_EnableT0();									           // Enable TIMER0
	INT_EnTIMER0();									           // Enable TIMER0 Interrupt
}




void Timer0_ISR(void) interrupt INT_VECTOR_T0
{
  TimerStop();
	AnmtnTimer();
	DiceTimer();
	LedTimer();
	InputScanTimer();
	BeepTimer();
	ToknBlinkTimer();
	TimerStart();
}













