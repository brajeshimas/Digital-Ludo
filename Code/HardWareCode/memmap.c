#include ".\include\REG_MG82F6D17.H"
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"

#include "memmap.h"
#include "eeprom.h"
#include "USER.h"


/*
	Function Name	:	SetupMemMap()
	Called by			:	Main.c
	Scope					: Public
	Description		: SetupMemMap() is called once by main() while booting the system.
									It first checks the first byte of the Data Flash Memory which is located
									at the last flash page having the start address as 0x3E00. If the first byte
									is 0x01 then it does not make any changes to the Data Flash and exits after
									clearing the Trapflags. But if the first byte is not 0x01 which means that 
									Data Flash has not been initialized, then it clears the last Flash page and 
									writes each setting byte one by one and then returns back after clearing 
									the Trapflags.
									
*/

void SetupMemMap(void)
{
   unsigned char EEPROM_Init_Flag;
   
   EEPROM_Init_Flag = EEPROM_Read_Byte((unsigned int) MEMMAP_EEPROM_INIT_FLAG);
																						
	
   if (EEPROM_Init_Flag!=EEPROM_INITIALIZED)
   {	
		 		 
		IAP_ErasePage(HIBYTE(MEMMAP_START_ADDRESS));		 							//Erase once and multiple writes	
		 
		//RED_USER_DEFAULT TOKN VALUE---------------------------------
		IAP_WriteByte(RED_USER_START_ADRESS, 0);
    IAP_WriteByte(RED_USER_START_ADRESS+1,0);		 
		IAP_WriteByte(RED_USER_START_ADRESS+2,0);
		IAP_WriteByte(RED_USER_START_ADRESS+3,0);

		//BLUE_USER_DEFAULT TOKN VALUE---------------------------------
		IAP_WriteByte(BLUE_USER_START_ADRESS, 0);
    IAP_WriteByte(BLUE_USER_START_ADRESS+1,0);		 
		IAP_WriteByte(BLUE_USER_START_ADRESS+2,0);
		IAP_WriteByte(BLUE_USER_START_ADRESS+3,0);
		 
		 
		//GREEN_USER_DEFAULT TOKN VALUE---------------------------------
		IAP_WriteByte(GREEN_USER_START_ADRESS, 0);
    IAP_WriteByte(GREEN_USER_START_ADRESS+1,0);		 
		IAP_WriteByte(GREEN_USER_START_ADRESS+2,0);
		IAP_WriteByte(GREEN_USER_START_ADRESS+3,0);
		

		//YELLOW_USER_DEFAULT TOKN VALUE---------------------------------
		IAP_WriteByte(YELLOW_USER_START_ADRESS, 0);
    IAP_WriteByte(YELLOW_USER_START_ADRESS+1,0);		 
		IAP_WriteByte(YELLOW_USER_START_ADRESS+2,0);
		IAP_WriteByte(YELLOW_USER_START_ADRESS+3,0);
		
		IAP_WriteByte(BUZZER_SETTING_ADRESS,DEFAULT_BUZZER_ON_OR_OFF);
		
		IAP_WriteByte(MEMMAP_EEPROM_INIT_FLAG, EEPROM_INITIALIZED);				//set this byte to skip this Code on next run
		
   }  
	 
}

void SaveAllCrntSettings(void)
{
	unsigned char xdata Data[17];
	Data[0]  =  Red[0];   Data[1]  = Red[1];   Data[2]  = Red[2];   Data[3]  = Red[3];
	Data[4]  =  Blue[0];  Data[5]  = Blue[1];  Data[6]  = Blue[2];  Data[7]  = Blue[3];
	Data[8]  =  Green[0]; Data[9]  = Green[1]; Data[10] = Green[2]; Data[11] = Green[3];
	Data[12] =  Yellow[0];Data[13] = Yellow[1];Data[14] = Yellow[2];Data[15] = Yellow[3];
	Data[17] =  1;
	EEPROM_Write_nByte(RED_USER_START_ADRESS,Data,16);
}

void LoadPreSettings(void)
{
	unsigned char ElcCutFlg; 
  ElcCutFlg = EEPROM_Read_Byte((unsigned int) ELECTRICITY_CUT_FLG_ADRESS);
	if(ElcCutFlg == 1)
	{
		IAP_WriteByte(ELECTRICITY_CUT_FLG_ADRESS, 0);
		
		Red[0]   = EEPROM_Read_Byte((unsigned int)RED_USER_START_ADRESS);
		Red[1]   = EEPROM_Read_Byte((unsigned int)RED_USER_START_ADRESS+1);
		Red[2]   = EEPROM_Read_Byte((unsigned int)RED_USER_START_ADRESS+2);
		Red[3]   = EEPROM_Read_Byte((unsigned int)RED_USER_START_ADRESS+3);
				
		Blue[0]  = EEPROM_Read_Byte((unsigned int)BLUE_USER_START_ADRESS);
		Blue[1]  = EEPROM_Read_Byte((unsigned int)BLUE_USER_START_ADRESS+1);
		Blue[2]  = EEPROM_Read_Byte((unsigned int)BLUE_USER_START_ADRESS+2);
		Blue[3]  = EEPROM_Read_Byte((unsigned int)BLUE_USER_START_ADRESS+3);
		
		Green[0] = EEPROM_Read_Byte((unsigned int)GREEN_USER_START_ADRESS);
		Green[1] = EEPROM_Read_Byte((unsigned int)GREEN_USER_START_ADRESS+1);
		Green[2] = EEPROM_Read_Byte((unsigned int)GREEN_USER_START_ADRESS+2);
		Green[3] = EEPROM_Read_Byte((unsigned int)GREEN_USER_START_ADRESS+3);
			
		Yellow[0]= EEPROM_Read_Byte((unsigned int)YELLOW_USER_START_ADRESS);
		Yellow[1]= EEPROM_Read_Byte((unsigned int)YELLOW_USER_START_ADRESS+1);
		Yellow[2]= EEPROM_Read_Byte((unsigned int)YELLOW_USER_START_ADRESS+2);
		Yellow[3]= EEPROM_Read_Byte((unsigned int)YELLOW_USER_START_ADRESS+3);
 }
	

}