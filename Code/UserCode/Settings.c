

#include <REG_MG82F6D17.h>
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"

#include "Settings.h"
#include "UserInput.h"
#include "BOD.h"
#include "memmap.h"

#define SETTING_INIT                  0
#define SETTING_IDLE                  1
#define SETTING_CHECK_CMD             2


#define BEEP_DISABLE_ENABLE            5
#define BOARD_RESTART_AGAIN            6
#define SMART_DICE_DISABLE_ENABLE      7
#define PLAYER_DISABLE_OR_ENABLE       8  
#define BOARD_SWITCH_OFF               9  
#define BOARD_FACTORY_RESET            10


uint8_t idata SettingState=0;
uint8_t idata SettingInput;
bit BeepDisFlg;
bit RedUserEnDisFlg;
bit BlueUserEnDisFlg;
bit GreenUserEnDisFlg;
bit YellowUserEnDisFlg;
bit SmartDiceEnDisFlg;
void SettingsManager(void)
{
	switch(SettingState)
	{
		case SETTING_INIT:
			 BeepDisFlg = 0;
			 RedUserEnDisFlg = 0;
			 BlueUserEnDisFlg = 0;
			 GreenUserEnDisFlg = 0;
			 YellowUserEnDisFlg = 0;
			 SmartDiceEnDisFlg = 0;
			SettingState = SETTING_IDLE;
			break;
		
		case SETTING_IDLE:
			if((KeyPressedFlg == 1)&&(LongPressedCounter>3))
			{
       SettingInput = KeyColmValue*KeyPressedRowValue;				
			 SettingState = SETTING_CHECK_CMD;
			}
			break;
			
		case SETTING_CHECK_CMD:
			if(KeyPressedFlg == 0)
			{
				switch(LongPressedCounter)
				{
					case PLAYER_DISABLE_OR_ENABLE:
						if((SettingInput == RED_KEY1)||(SettingInput == RED_KEY2)||(SettingInput == RED_KEY3)||(SettingInput == RED_KEY4))
						{RedUserEnDisFlg = !RedUserEnDisFlg;}
						else if((SettingInput == BLUE_KEY1)||(SettingInput == BLUE_KEY2)||(SettingInput == BLUE_KEY3)||(SettingInput == BLUE_KEY4))
						{BlueUserEnDisFlg = !BlueUserEnDisFlg;}
						else if((SettingInput == GREEN_KEY1)||(SettingInput == GREEN_KEY2)||(SettingInput == GREEN_KEY3)||(SettingInput == GREEN_KEY4))
						{GreenUserEnDisFlg = !GreenUserEnDisFlg;}
						else if((SettingInput == YELLOW_KEY1)||(SettingInput == YELLOW_KEY2)||(SettingInput == YELLOW_KEY3)||(SettingInput == YELLOW_KEY4))
						{YellowUserEnDisFlg = !YellowUserEnDisFlg;}
						break;
						
						
					case BEEP_DISABLE_ENABLE:
						BeepDisFlg = !BeepDisFlg;
						break;
					
					case BOARD_SWITCH_OFF:
						break;
					
					case BOARD_RESTART_AGAIN:
						break;
					
					case BOARD_FACTORY_RESET:
						break;
					
					case SMART_DICE_DISABLE_ENABLE:
						break;		
				}
				SettingState = SETTING_IDLE;
			}
			//if(bBOD1Flag ==1)
			//{bBOD1Flag = 0;SaveAllCrntSettings();while(1);}
			break;
	}
}