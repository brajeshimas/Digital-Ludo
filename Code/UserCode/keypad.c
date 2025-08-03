

#include ".\include\REG_MG82F6D17.H"
#include ".\include\API_Macro_MG82F6D17.H"
#include ".\include\Type.h"

#include "keypad.h"

volatile uint8_t KeyValue;
volatile uint8_t KeypadState;
volatile uint8_t KeyPressedValue;
volatile uint8_t KeyPressTimer;
volatile signed char  KeyPressCounter;
volatile signed char	LongKeyPressTimer;

void KeypadScan(void)
{
	switch(KeypadState)
	{
		case KEYPAD_STATE_INIT:	
			KeypadState = KEYPAD_STATE_IDLE;
			break;
		
		case KEYPAD_STATE_IDLE:
			KeyPressCounter=-1;						
			LongKeyPressTimer=-1;
			KeyPressedValue = GetKeyPressed();
			if(KeyPressedValue != NULL_KEY)
	    {
	      KeypadState=KEYPAD_STATE_KEYDEBOUNCE;
	      KeyPressTimer=KEY_DEBOUNCE_TIME; 			
	    }
      break;
	
	
    case KEYPAD_STATE_KEYDEBOUNCE:
			if (KeyPressTimer==0)						
	    {
				if (GetKeyPressed() != KeyPressedValue)
				{
					KeyPressedValue=NULL_KEY;
					KeypadState=KEYPAD_STATE_IDLE;
					break;
				}    
	       KeypadState=KEYPAD_STATE_WAITFORRELEASE;		
	       KeyPressCounter=0;
	       LongKeyPressTimer=0;
				 
	    }
			break;
	
	    
      case KEYPAD_STATE_WAITFORRELEASE:
			if(LongKeyPressTimer==KEY_REPEAT_TIME)
	    {
	      LongKeyPressTimer=0;
				KeyPressCounter++;
				if(KeyPressCounter > 2)
				{
					if(KeyPressedValue == 7)
					{SetLedIndication(5,ON,INFINITE);}
				}
	    }
	
			if (GetKeyPressed() == NULL_KEY)
	    {
	      KeyValue = KeyPressedValue;
        if(KeyValue == 7)
				{
					if(KeyPressCounter > 2)
					KeyValue = 8;	
				}
	      
	       KeypadState=KEYPAD_STATE_IDLE;
	       KeyPressCounter=-1;
	       LongKeyPressTimer=-1;
	       break;
	    }	    
		break;
   }
}

char GetKeyPressed(void)
{
	if (!SW1)	
	{return KEY1;}
	
	else if (!SW2)	
	{return KEY2;}
	
	else if (!SW3)	
	{return KEY3;}
	
	else if (!SW4)	
	{return KEY4;}
	
	else if (!SW5)	
	{return KEY5;}
	
	else if (!SW6)	
	{return KEY6;}
	
	else if (!SW7)	
	{return KEY7;}

	else 
	{return NULL_KEY;}
}






