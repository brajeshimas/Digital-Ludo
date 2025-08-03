
/*
COMPANY NAME:- YOURS AUTOMATED DEVICES AND SOLUTIONS
AUTHERS:-    BRAJESH KUMAR
OBJECTIVE :- FOR TLC V1.0 
DISCRIPTIONS:-
The File is Created For ManagingBeep sound Smartly
Inside The File There is Following Functions:-

1.SetBeepIndication() :- This Function is Non Loop Function , it is for Outer World means this will be used by Indication Manager 
                         for Managing Beep indication. this function Takes two argument as BeepType and BeepCount
												 Example:- SetBeepIndication(SHORT_BEEP ,5)  means buzzer beeps as ..----------..---------..----------..--------..-----------
												           SetBeepIndication(LONG_BEEP ,3)  means buzzer beeps as ......--------......---------.......--------
																	 SetBeepIndication(ULTRA_LONG_BEEP ,2) buzzer beeps as ............----...........-----
												the main work of the Function is to set Relevant flags and counter that will be used by BuzzerManager
												
												
2.BuzzerTimerReload(); :- This Function is Used by BuzzerManager() for Reloadding the Different types of Timer
                          This Function takes two argument as beepType and onOroffTime
													according to given argument set relevent Timervalue in BuzzerTimer variable
													Example:- BuzzerTimerReload(LONG_BEEP,ON_TIME);
													          it will Reload the Timer varible as BuzzerTimer = LONG_BEEP_ON_TIME
																		
3.BuzzerManager(); :- This is While Loop function and Handle different type of Buzzer sound as shortBeep and LongBeep etc
                      working flow chart of the Function is 
											 BZR_INIT
											      |
                           \|/											                                           yes
|---------------------->BZR_IDLE-------->check User command that is set by	SetBeepIndication();--->
|                                                                                                  |	
|                                                                                                 \|/																																																	
|BuzzerTimerReload(beepType,ON_TIME)<----BZR_LD_TIMER ----------------------------------------------      
|				|
|        |
|        |>---------------	BZR_ON_WAIT(if Timer Expires buzeer off)--------->BuzzerTimerReload(beepType,OFF_TIME)	
|                                /|\                                                            |
|                                 |<------------(if Timer Expires buzeer ON)BZR_OFF_WAIT<-----<|																																														 
|											                                               |
 <--------------------------------(if Counter Expires buzeer OFF)<--<|
	


Note:- All Hardware Port Pin initialization is Handled in Hardware.c and Hardware.h file
*/

#include <REG_MG82F6D17.h>
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"

#include "Buzzer.h"
#include "Settings.h"

//This is Extern Variable******************
bit BuzzerBeepcmd;
volatile unsigned char BuzzerBeepCounter;
//*****************************************

//***********TIMER_VARIABLE**********
volatile int BuzzerTimer;
//***********************************

//Locall used Variable*****************************
volatile unsigned char BeepType;
volatile unsigned char BzrState;
//*************************************************

void SetBeepIndication(unsigned char beepType ,unsigned char Beepcount)
{
	if(BeepDisFlg==0)
	{
		if(Beepcount>1)
		{
		BuzzerBeepcmd = 1;
		BuzzerBeepCounter = Beepcount;
		BeepType = beepType;
		}
		else if(Beepcount == 1)
		{
			BZR_PIN = BZR_ON;
			BzrState = BZR_ONE_BEEP;
			BuzzerTimerReload(BeepType,ON_TIME);
		}
	}
	
}

void StartBeep(void)
{
	if(BeepDisFlg==0)
	{
	 BZR_PIN = BZR_ON;
	}
}
void StopBeep(void)
{
	 BZR_PIN = BZR_OFF;
}

void BuzzerManager(void)
{

	switch(BzrState)
	{
		case BZR_INIT:
			BzrState = BZR_IDLE;
		break;
		
		case BZR_IDLE:
			if((BuzzerBeepcmd == 1)&&(BuzzerBeepCounter > 0))
			{BuzzerBeepcmd = 0;BzrState = BZR_LD_TIMER;}
		break;
		
		case BZR_LD_TIMER:
      BuzzerTimerReload(BeepType,ON_TIME);
			BZR_PIN = BZR_ON;  
			BzrState = BZR_ON_WAIT;
		break;

			
		case BZR_ON_WAIT:
			if(BuzzerTimer == 0)
			{
				BuzzerTimerReload(BeepType,OFF_TIME);
				BZR_PIN = BZR_OFF;  
				BzrState = BZR_OFF_WAIT;
			}
		break;
		
		case BZR_OFF_WAIT:
			if(BuzzerTimer == 0)
			{
				BuzzerBeepCounter--;
				if(BuzzerBeepCounter == 0)
				{BzrState = BZR_IDLE;}
				else
				{
				BuzzerTimerReload(BeepType,ON_TIME);
				BZR_PIN = BZR_ON;  
				BzrState = BZR_ON_WAIT;
				}		
			}
		break;
		
		case BZR_ONE_BEEP:
			if(BuzzerTimer == 0)
			{
				BZR_PIN = BZR_OFF;  
				BzrState = BZR_IDLE;
			}
			break;
		
	}

}



void BuzzerTimerReload(unsigned char beepType,bit onOroffTime)
{
	if(onOroffTime)
	{
		switch(beepType)
		{
			case SHORT_BEEP:
				BuzzerTimer = SHORT_BEEP_ON_TIME;
			break;
			
			case LONG_BEEP:
				BuzzerTimer = LONG_BEEP_ON_TIME;
			break;
			
			case ULTRA_LONG_BEEP:
				BuzzerTimer = ULTRA_LONG_BEEP_ON_TIME;
			break;
		}
	}
	
	else
	{
	  switch(beepType)
		{
			case SHORT_BEEP:
				BuzzerTimer = SHORT_BEEP_OFF_TIME;
			break;
			
			case LONG_BEEP:
				BuzzerTimer = LONG_BEEP_OFF_TIME;
			break;
			
			case ULTRA_LONG_BEEP:
				BuzzerTimer = ULTRA_LONG_BEEP_OFF_TIME;
			break;
		}	
	}
}


void BeepTimer(void)
{
	if(BuzzerTimer>0)
	{BuzzerTimer--;}
}

