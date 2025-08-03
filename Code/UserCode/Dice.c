
#include <stdlib.h>
#include <REG_MG82F6D17.h>
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"

#include "HardWare.h"
#include "Dice.h"
#include "SPI.h"
#include "Buzzer.h"
#include "LedBoard.h"
#include "Settings.h"
#include "UserInput.h"

//-----------------------------------------
bit RollDiceCmd        = FALSE;
bit DiceResultReadyFlg = FALSE;
uint32_t DiceRoleTimer;
uint8_t DiceValue;
uint8_t idata DiceRandomVal = 0;
//------------------------------------------


//***DICE_MANAGER**************
#define DICE_INIT   0
#define DICE_IDLE   1
#define DICE_ROLL   2
#define DICE_WAIT   4
#define DICE_SHOW_ARROW_ON   5
#define DICE_SHOW_ARROW_OFF  6
#define DICE_RESULT_PREPARE  7
#define PRE_PLAYER_ACTIVATED_1 8
#define PRE_PLAYER_ACTIVATED_2 9
#define PRE_PLAYER_ACTIVATED_3 10
#define SHOW_SETTINGS_NUMBER   11
//*****************************
uint8_t DiceState;
uint8_t ArrowState;
uint8_t RollCount = 0;
uint8_t PlayerClr = 0;
uint8_t idata PrePlayerClr = 0;
bit ArrowPointFlg = 0;
uint8_t idata PreLongPressedCounter;
bit IsDiceRollTimerExpr(void);
void StrtDiceRollTimer(uint32_t TimerVal);
uint8_t random(uint8_t till /*range 1 - till*/);
void RandomDsplyOnSegment(uint8_t randomvalue);
void ShowKiskiBariHai(uint8_t PlayerClr);
void DiceManager(void)
{
                uint8_t DummyDiceValue = 0;
                switch(DiceState)
                {
                  case DICE_INIT:
										DsplyOnSegment(CLEAR_DICE,FALSE);
                    StrtDiceRollTimer(0);//Initialize Timer
                    DiceState = DICE_IDLE;
                  break;
                  
                  case DICE_IDLE:
                  if(RollDiceCmd)
                  {                                                                                               
                    DiceResultReadyFlg = 0;
                    DiceValue = 0;
                    RollDiceCmd = 0;
										StrtDiceRollTimer(0);
                    DiceState = DICE_ROLL;
                  }
									else if(ArrowPointFlg)
									{
										ArrowPointFlg = 0;
										PrePlayerClr = PlayerClr;
										StrtDiceRollTimer(10);
									  DiceState = DICE_SHOW_ARROW_ON;
									}
                  break;
                  
                  case DICE_ROLL:
                    if(IsDiceRollTimerExpr())
                    {
									  RandomDsplyOnSegment(RollCount);
                    StrtDiceRollTimer(RollCount*25);//In Milli Second
                    RollCount++;
                    if(RollCount == 10)
                    {
											DiceValue = DiceRandomVal;//random(6);
                      RollCount = 0;									
											DsplyOnSegment(DiceValue,TRUE);//Indcate with dot Result is Ready
											StrtDiceRollTimer(1000);
											DiceState = DICE_RESULT_PREPARE;
                    }
                    else 
                    {StartBeep(); DiceState = DICE_WAIT;}
									}
                  break;
              
                  case DICE_WAIT:
                    if(IsDiceRollTimerExpr())
                    {
											StopBeep();
											StrtDiceRollTimer(20);
                      DiceState = DICE_ROLL;
                    }
                  break;
										
								  case DICE_SHOW_ARROW_ON:
										if(IsDiceRollTimerExpr())
                    {
											ShowKiskiBariHai(PlayerClr);
											StrtDiceRollTimer(500);
											DiceState = DICE_SHOW_ARROW_OFF;
										}
										if((BeepDisFlg == 1)&&(KeyPressedFlg == 1)&&(LongPressedCounter > 4))
										{
											PreLongPressedCounter = 0;
											DiceState = SHOW_SETTINGS_NUMBER;
										}
										break;
										
									case DICE_SHOW_ARROW_OFF:
										if(IsDiceRollTimerExpr())
                    {
											ShowKiskiBariHai(0);
											StrtDiceRollTimer(50);
											DiceState = DICE_SHOW_ARROW_ON;
										}
										if(RollDiceCmd)
										{DiceState = DICE_IDLE;}

										break;
										
						    case DICE_RESULT_PREPARE:
									   if(IsDiceRollTimerExpr())
									   {
											 DiceResultReadyFlg = 1;
											 StrtDiceRollTimer(5000);
                       DiceState = PRE_PLAYER_ACTIVATED_1;	
								     }
										break;	

						    case PRE_PLAYER_ACTIVATED_1:
									   if(IsDiceRollTimerExpr())
									   {
											ShowKiskiBariHai(PlayerClr);
											StrtDiceRollTimer(500);	
											DiceState = PRE_PLAYER_ACTIVATED_2;
								     }
										 if((PrePlayerClr != PlayerClr)||(RollDiceCmd == 1))
										 DiceState = DICE_IDLE;
										break;	
										 
						    case PRE_PLAYER_ACTIVATED_2:
									   if(IsDiceRollTimerExpr())
									   {
											ShowKiskiBariHai(0);
											StrtDiceRollTimer(20);	
											DiceState = PRE_PLAYER_ACTIVATED_3;
								     }
										 if((PrePlayerClr != PlayerClr)||(RollDiceCmd == 1))
										 DiceState = DICE_IDLE;
										break;
											
						    case PRE_PLAYER_ACTIVATED_3:
									   if(IsDiceRollTimerExpr())
									   {
											StrtDiceRollTimer(1000);
											DsplyOnSegment(DiceValue,TRUE);
                      DiceState = PRE_PLAYER_ACTIVATED_1;	
								    	}
										 if((PrePlayerClr != PlayerClr)||(RollDiceCmd == 1))
											 DiceState = DICE_IDLE;
										 
										if((BeepDisFlg == 1)&&(KeyPressedFlg == 1)&&(LongPressedCounter > 3))
										{
											PreLongPressedCounter = 0;
											DiceState = SHOW_SETTINGS_NUMBER;
										}
										break;
										 
								case SHOW_SETTINGS_NUMBER:
									if(PreLongPressedCounter != LongPressedCounter)
									{
										PreLongPressedCounter = LongPressedCounter;
										DsplyOnSegment(LongPressedCounter,FALSE);
									}
									else if(KeyPressedFlg == 0)
									 DiceState = DICE_SHOW_ARROW_ON;
									break;
											
                  
                }
								

								

								
}
//**************************************************************************************



//--------FUNCTIONS FOR DICE MANAGER-----------
uint8_t random(uint8_t till /*range 1 - till*/)
{
		uint8_t randval = 0;
	  randval= rand();
	  randval = (randval%till);
	  if(randval == 0)
		randval = till;
		return randval;
}

void StrtDiceRollTimer(uint32_t TimerVal)
{
  DiceRoleTimer = TimerVal;
}

bit IsDiceRollTimerExpr(void)
{
  if(DiceRoleTimer == 0)
  return 1;
  else
  return 0;
}
//----------------------------------------------



//----FUNC FOR MANAGE TIMER---------------------
void DiceTimer(void)
{
  if(DiceRoleTimer>0)
  {
    DiceRoleTimer--;
  }
	
	DiceRandomVal++;
	if(DiceRandomVal == 7)
	{	DiceRandomVal = 1;}
}
//----------------------------------------------


//******************************************
//These Function will be used by User Layer
uint8_t ReadDiceValue(void)
{
  if(DiceResultReadyFlg == 1)
  return DiceValue;
  else 
  return 0;
}
void RollTheDice(void)
{
  RollDiceCmd = TRUE;
	DiceValue = 0;
	DiceResultReadyFlg=0;
}
//*******************************************



//************Driver For 7 Segment****************************



void DsplyOnSegment(uint8_t Number, bit DotPrintTrueFalse)
{
  uint8_t value = 0;
	if(DotPrintTrueFalse){value |= DOT;}
  if(Number == CLEAR_DICE){value = 0;}
		switch(Number)
		{
			case 0x00:value |= ZERO;  break;
			case 0x01:value |= ONE;   break; 
			case 0x02:value |= TWO;   break;
			case 0x03:value |= THREE; break;
			case 0x04:value |= FOUR;  break;
			case 0x05:value |= FIVE;  break; 
			case 0x06:value |= SIX;   break;  
			case 0x07:value |= SEVEN; break; 
			case 0x08:value |= EIGHT; break;
			case 0x09:value |= NINE;  break;
    }

	SPI_tranfer(value);
  digitalWrite(DICE_LATCH_PIN,0);		
	digitalWrite(DICE_LATCH_PIN,1);

}
//************************************************

void RandomDsplyOnSegment(uint8_t randomvalue)
{
	uint8_t value = 0;
	switch(randomvalue)
	{
			case 0x00:value |= A;  break;
			case 0x01:value |= F;   break; 
			case 0x02:value |= D;   break;
			case 0x03:value |= F; break;
			case 0x04:value |= A;  break;
			case 0x05:value |= G;  break; 
			case 0x06:value |= B;   break;  
			case 0x07:value |= E; break; 
			case 0x08:value |= C; break;
			case 0x09:value |= F;  break;
	}
	SPI_tranfer(value);
  digitalWrite(DICE_LATCH_PIN,0);		
	digitalWrite(DICE_LATCH_PIN,1);
}

void ShowKiskiBariHai(uint8_t PlayerClr)
{
	uint8_t value = 0;
	switch(PlayerClr)
	{
		case RED:value |= A|G;break;
		case BLUE:value |= A|B;break;
		case GREEN:value |= C|D;break;
		case YELLOW:value |= E|D;break;				
	}
	
	SPI_tranfer(value);
  digitalWrite(DICE_LATCH_PIN,0);		
	digitalWrite(DICE_LATCH_PIN,1);
}


void SetArrowPointTo(uint8_t PlyrClr)
{
	ArrowPointFlg = 1;PlayerClr = PlyrClr;
}