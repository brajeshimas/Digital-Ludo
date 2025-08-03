
#include <REG_MG82F6D17.h>
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"

#include "UserInput.h"
#include "SPI.h"
#include "HardWare.h"
#include "Delay.h"
#include "LedBoard.h"
#include "Buzzer.h"
#include "User.h"
#include "UserDriverBridge.h"

#define KEYDEBOUNCE_TIME          100  //millli second
#define KEYLONGPRESSS_TIME        1500 //milli second
#define KEY_RAMP_TIME             5//milli second

#define ROWKEY1_PIN      P10    
#define ROWKEY2_PIN      P11
#define ROWKEY3_PIN      P15
#define ROWKEY4_PIN      P16

#define KEY_COLM1_PIN        P61
#define KEY_COLM2_PIN        P60
#define KEY_COLM3_PIN        P24
#define KEY_COLM4_PIN        P22




#define INPUT_INIT           0
#define INPUT_IDLE           1
#define INPUT_CHECK          2
#define INPUT_DEBOUNCE       3
#define INPUT_LONG_PRESSED   4



#define KEY_VS_TOKN_INIT                0
#define KEY_VS_TOKN_IDLE                1
#define KEY_TOKN_BLINK                  2
#define KEY_TOKN_BLINK_OFF              3
#define KEY_TOKN_BLINK_ON               4
#define KEY_TOKN_BLINK_STOP             5

#define KEY_TOKN_BLINK_TIME         80//MILLI SECOND
uint8_t BlinkTokn;
uint8_t TokenNmbr;
uint8_t ToknColour;
uint8_t BlinkCounter;
uint8_t KeyToknBlinkTimer;
uint8_t keyvsToknState;



uint8_t InputState;
uint8_t KeyPressedRowValue;
uint8_t KeyColmValue;
uint8_t PreColmValue;
uint8_t LongPressedCounter;
uint8_t UserInput;
uint8_t ToknCheckInput;
uint32_t KeyPressedTimer;
bit KeyPressedFlg;
bit CheckToknFlg = 0;
bit KeyPressedOkFlg = 0;
//uint8_t GetKeyPressed(void);
void DeactivateColm(uint8_t KeyColmNumber);
void ActivateColm(uint8_t KeyColmNumber);






void ScanInput(void)
{
	switch(InputState)
	{
		case INPUT_INIT:
			UserInput = 0;
		  ToknCheckInput = 0;
		  KeyColmValue = 12;
			InputState = INPUT_IDLE;
			break;
		
		case INPUT_IDLE:
			KeyColmValue++;
			ActivateColm(KeyColmValue);
		  DeactivateColm(KeyColmValue-1);
		  if(KeyColmValue == 17)
			{
				KeyColmValue = 12;
			}
			else
			{
			KeyPressedTimer = KEY_RAMP_TIME;
			InputState = INPUT_CHECK;
			}
			break;
		
	  case INPUT_CHECK:
			if(KeyPressedTimer == 0)
			{
				KeyPressedRowValue = 0;
				
				KeyPressedRowValue = GetKeyPressed();
				if(KeyPressedRowValue > 0)
				{
					PreColmValue = KeyColmValue;
					KeyPressedTimer = KEYDEBOUNCE_TIME;
					InputState = INPUT_DEBOUNCE;
				}
				else
				{InputState = INPUT_IDLE;}				
			}
			break;
			
	
	 case INPUT_DEBOUNCE:
		 if(KeyPressedTimer == 0)
		 {
			 if(GetKeyPressed() == KeyPressedRowValue)
			 {
				SetBeepIndication(SHORT_BEEP,1);
				KeyPressedTimer = KEYLONGPRESSS_TIME;
				InputState = INPUT_LONG_PRESSED;
				KeyPressedFlg = 1;LongPressedCounter = 0;
				ToknCheckInput = KeyColmValue*KeyPressedRowValue;
			 }
			 else
			 {InputState = INPUT_IDLE;}
		 }
		 break;
			
	 case INPUT_LONG_PRESSED:
			if(KeyPressedTimer == 0)
			{				
				   if((GetKeyPressed() == KeyPressedRowValue))
					 {
						 KeyPressedTimer = 3000;
						 LongPressedCounter++;
						 SetBeepIndication(SHORT_BEEP,1);
						 /*if(LongPressedCounter > 2)
             {
						  LongPressedCounter = 0;
						  LongKeyFlg = 1;
						  ToknCheckInput = KeyColmValue*KeyPressedRowValue;
						 }*/
						 
					 }	
					else
					{
					 KeyPressedFlg = 0;
					 if(LongPressedCounter>0)
					 UserInput = 0; 
					 else
					 UserInput = KeyColmValue*KeyPressedRowValue;
					 InputState = INPUT_IDLE;
					}		

			}
			break;
		
	}
}


void ActivateColm(uint8_t KeyColmNumber)
{
	switch(KeyColmNumber)
	{
		case 13:KEY_COLM1_PIN = 1;break;
		case 14:KEY_COLM2_PIN = 1;break;
		case 15:KEY_COLM3_PIN = 1;break;
		case 16:KEY_COLM4_PIN = 1;break;
	}
	
}

void DeactivateColm(uint8_t KeyColmNumber)
{
	switch(KeyColmNumber)
	{
		case 13:KEY_COLM1_PIN = 0;break;
		case 14:KEY_COLM2_PIN = 0;break;
		case 15:KEY_COLM3_PIN = 0;break;
		case 16:KEY_COLM4_PIN = 0;break;
	}
	
}


uint8_t GetKeyPressed(void)
{

	if(ROWKEY1_PIN)
	return 1;
	else if(ROWKEY2_PIN)
	return 2;
	else if(ROWKEY3_PIN)
	return 3;
	else if(ROWKEY4_PIN)
	return 4;
	else 
	return 0;
		
}

void InputScanTimer(void)
{
	if(KeyPressedTimer>0)
		KeyPressedTimer--;
}



/*
void CheckKeyVsTokn(void)
{
	switch(keyvsToknState)
	{
		case KEY_VS_TOKN_INIT:
			BlinkCounter = 0;
			keyvsToknState = KEY_VS_TOKN_IDLE;
			break;
		
		case KEY_VS_TOKN_IDLE:
			if(LongKeyFlg)
			{
				if(ToknCheckInput>0)
        keyvsToknState = ToknCheckInput;ToknCheckInput = 0;
			}
			break;
		case RED_KEY1:BlinkTokn = Red[0];TokenNmbr = 0;ToknColour = RED;keyvsToknState = KEY_TOKN_BLINK;break;
		case RED_KEY2:BlinkTokn = Red[1];TokenNmbr = 1;ToknColour = RED;keyvsToknState = KEY_TOKN_BLINK;break;
		case RED_KEY3:BlinkTokn = Red[2];TokenNmbr = 2;ToknColour = RED;keyvsToknState = KEY_TOKN_BLINK;break;
		case RED_KEY4:BlinkTokn = Red[3];TokenNmbr = 3;ToknColour = RED;keyvsToknState = KEY_TOKN_BLINK;break;
			
		case BLUE_KEY1:BlinkTokn = Blue[0];TokenNmbr = 0;ToknColour = BLUE;keyvsToknState = KEY_TOKN_BLINK;break;
		case BLUE_KEY2:BlinkTokn = Blue[1];TokenNmbr = 1;ToknColour = BLUE;keyvsToknState = KEY_TOKN_BLINK;break;
		case BLUE_KEY3:BlinkTokn = Blue[2];TokenNmbr = 2;ToknColour = BLUE;keyvsToknState = KEY_TOKN_BLINK;break;
		case BLUE_KEY4:BlinkTokn = Blue[3];TokenNmbr = 3;ToknColour = BLUE;keyvsToknState = KEY_TOKN_BLINK;break;
			
		case GREEN_KEY1:BlinkTokn = Green[0];TokenNmbr = 0;ToknColour = GREEN;keyvsToknState = KEY_TOKN_BLINK;break;
		case GREEN_KEY2:BlinkTokn = Green[1];TokenNmbr = 1;ToknColour = GREEN;keyvsToknState = KEY_TOKN_BLINK;break;
		case GREEN_KEY3:BlinkTokn = Green[2];TokenNmbr = 2;ToknColour = GREEN;keyvsToknState = KEY_TOKN_BLINK;break;
		case GREEN_KEY4:BlinkTokn = Green[3];TokenNmbr = 3;ToknColour = GREEN;keyvsToknState = KEY_TOKN_BLINK;break;
			
		case YELLOW_KEY1:BlinkTokn = Yellow[0];TokenNmbr = 0;ToknColour = YELLOW;keyvsToknState = KEY_TOKN_BLINK;break;
		case YELLOW_KEY2:BlinkTokn = Yellow[1];TokenNmbr = 1;ToknColour = YELLOW;keyvsToknState = KEY_TOKN_BLINK;break;
		case YELLOW_KEY3:BlinkTokn = Yellow[2];TokenNmbr = 2;ToknColour = YELLOW;keyvsToknState = KEY_TOKN_BLINK;break;
		case YELLOW_KEY4:BlinkTokn = Yellow[3];TokenNmbr = 3;ToknColour = YELLOW;keyvsToknState = KEY_TOKN_BLINK;break;
			
		case KEY_TOKN_BLINK:
			DcodeToknToLedNumber(TokenNmbr,58,ToknColour);
		  KeyToknBlinkTimer = KEY_TOKN_BLINK_TIME;
		  keyvsToknState = KEY_TOKN_BLINK_OFF;
		  BlinkCounter = 0;
			break;
		
		case KEY_TOKN_BLINK_OFF:
			if(KeyToknBlinkTimer == 0)
			{
				DcodeToknToLedNumber(TokenNmbr,BlinkTokn,ToknColour);
				KeyToknBlinkTimer = KEY_TOKN_BLINK_TIME;
				keyvsToknState = KEY_TOKN_BLINK_ON;
			}
		break;
			
		case KEY_TOKN_BLINK_ON:
			if(KeyToknBlinkTimer == 0)
			{
				DcodeToknToLedNumber(TokenNmbr,58,ToknColour);
				KeyToknBlinkTimer = KEY_TOKN_BLINK_TIME;
				keyvsToknState = KEY_TOKN_BLINK_OFF;
				BlinkCounter++;
				if(BlinkCounter == 10)
				{
					BlinkCounter = 0;
					DcodeToknToLedNumber(TokenNmbr,BlinkTokn,ToknColour);
					keyvsToknState = KEY_VS_TOKN_IDLE;
				}
			}
		break;
				
			
	}
}
*/









void CheckKeyVsTokn(void)
{
	switch(keyvsToknState)
	{
		case KEY_VS_TOKN_INIT:
			BlinkCounter = 0;
			keyvsToknState = KEY_VS_TOKN_IDLE;
			break;
		
		case KEY_VS_TOKN_IDLE:
			if((CheckToknFlg == 1)&&(KeyPressedFlg == 1))
			{
				if(ToknCheckInput>0)
        keyvsToknState = ToknCheckInput;ToknCheckInput = 0;
			}
			break;
		case RED_KEY1:BlinkTokn = Red[0];TokenNmbr = 0;ToknColour = RED;keyvsToknState = KEY_TOKN_BLINK;break;
		case RED_KEY2:BlinkTokn = Red[1];TokenNmbr = 1;ToknColour = RED;keyvsToknState = KEY_TOKN_BLINK;break;
		case RED_KEY3:BlinkTokn = Red[2];TokenNmbr = 2;ToknColour = RED;keyvsToknState = KEY_TOKN_BLINK;break;
		case RED_KEY4:BlinkTokn = Red[3];TokenNmbr = 3;ToknColour = RED;keyvsToknState = KEY_TOKN_BLINK;break;
			
		case BLUE_KEY1:BlinkTokn = Blue[0];TokenNmbr = 0;ToknColour = BLUE;keyvsToknState = KEY_TOKN_BLINK;break;
		case BLUE_KEY2:BlinkTokn = Blue[1];TokenNmbr = 1;ToknColour = BLUE;keyvsToknState = KEY_TOKN_BLINK;break;
		case BLUE_KEY3:BlinkTokn = Blue[2];TokenNmbr = 2;ToknColour = BLUE;keyvsToknState = KEY_TOKN_BLINK;break;
		case BLUE_KEY4:BlinkTokn = Blue[3];TokenNmbr = 3;ToknColour = BLUE;keyvsToknState = KEY_TOKN_BLINK;break;
			
		case GREEN_KEY1:BlinkTokn = Green[0];TokenNmbr = 0;ToknColour = GREEN;keyvsToknState = KEY_TOKN_BLINK;break;
		case GREEN_KEY2:BlinkTokn = Green[1];TokenNmbr = 1;ToknColour = GREEN;keyvsToknState = KEY_TOKN_BLINK;break;
		case GREEN_KEY3:BlinkTokn = Green[2];TokenNmbr = 2;ToknColour = GREEN;keyvsToknState = KEY_TOKN_BLINK;break;
		case GREEN_KEY4:BlinkTokn = Green[3];TokenNmbr = 3;ToknColour = GREEN;keyvsToknState = KEY_TOKN_BLINK;break;
			
		case YELLOW_KEY1:BlinkTokn = Yellow[0];TokenNmbr = 0;ToknColour = YELLOW;keyvsToknState = KEY_TOKN_BLINK;break;
		case YELLOW_KEY2:BlinkTokn = Yellow[1];TokenNmbr = 1;ToknColour = YELLOW;keyvsToknState = KEY_TOKN_BLINK;break;
		case YELLOW_KEY3:BlinkTokn = Yellow[2];TokenNmbr = 2;ToknColour = YELLOW;keyvsToknState = KEY_TOKN_BLINK;break;
		case YELLOW_KEY4:BlinkTokn = Yellow[3];TokenNmbr = 3;ToknColour = YELLOW;keyvsToknState = KEY_TOKN_BLINK;break;
			
		case KEY_TOKN_BLINK:
			if(KeyPressedFlg)
			{
				DcodeToknToLedNumber(TokenNmbr,58,ToknColour);
				KeyToknBlinkTimer = KEY_TOKN_BLINK_TIME;
				keyvsToknState = KEY_TOKN_BLINK_OFF;
				BlinkCounter = 0;
			}
			else
      {
				keyvsToknState = KEY_VS_TOKN_IDLE;
			}
			break;
		
		case KEY_TOKN_BLINK_OFF:
			if(KeyToknBlinkTimer == 0)
			{
				DcodeToknToLedNumber(TokenNmbr,BlinkTokn,ToknColour);
				KeyToknBlinkTimer = KEY_TOKN_BLINK_TIME;
				keyvsToknState = KEY_TOKN_BLINK_ON;
			}
			
			if(KeyPressedFlg == 0)
			{
			 if(LongPressedCounter==0)KeyPressedOkFlg = 1;
			 DcodeToknToLedNumber(TokenNmbr,BlinkTokn,ToknColour);keyvsToknState = KEY_VS_TOKN_IDLE;
			}
			
			else if(LongPressedCounter>0)
			{DcodeToknToLedNumber(TokenNmbr,BlinkTokn,ToknColour);keyvsToknState = KEY_TOKN_BLINK_STOP;}
		break;
			
		case KEY_TOKN_BLINK_ON:
			if(KeyToknBlinkTimer == 0)
			{
				DcodeToknToLedNumber(TokenNmbr,58,ToknColour);
				KeyToknBlinkTimer = KEY_TOKN_BLINK_TIME;
				keyvsToknState = KEY_TOKN_BLINK_OFF;
				BlinkCounter++;
				/*if(BlinkCounter == 10)
				{
					BlinkCounter = 0;
					DcodeToknToLedNumber(TokenNmbr,BlinkTokn,ToknColour);
					keyvsToknState = KEY_VS_TOKN_IDLE;
				}*/
			}
			if(KeyPressedFlg == 0)
			{
			 if(LongPressedCounter==0)KeyPressedOkFlg = 1;
			 DcodeToknToLedNumber(TokenNmbr,BlinkTokn,ToknColour);keyvsToknState = KEY_VS_TOKN_IDLE;
			}
			
			else if(LongPressedCounter>0)
			{DcodeToknToLedNumber(TokenNmbr,BlinkTokn,ToknColour);keyvsToknState = KEY_TOKN_BLINK_STOP;}
		break;
		
		case KEY_TOKN_BLINK_STOP:
			if(KeyPressedFlg == 0)
			{
				keyvsToknState = KEY_VS_TOKN_IDLE;
			}
			break;
				
			
	}
}


void ToknBlinkTimer(void)
{
	if(KeyToknBlinkTimer>0)
		KeyToknBlinkTimer--;
}

