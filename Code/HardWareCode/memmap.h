#ifndef __MEMMAP_H_
#define __MEMMAP_H_

//----------------------------DEFAULT MEMORY MAP START-----------------------------------

#define DEFAULT_BUZZER_ON_OR_OFF        1       

//-----------------------------DEFAULT MEMORY MAP END------------------------------------


#define EEPROM_MAX_SIZE						50	// Bytes
#define EEPROM_INITIALIZED				0x01
#define EEPROM_UNINITIALIZED			0x00

//EEPROM MEMORY MAP START
#define MEMMAP_START_ADDRESS							0x3E00 																	//Last Flash Page of 512 bytes	

#define MEMMAP_EEPROM_INIT_FLAG						MEMMAP_START_ADDRESS										//Add 0x00 for Initialization flag

#define RED_USER_START_ADRESS         		MEMMAP_EEPROM_INIT_FLAG						+ 1		

#define BLUE_USER_START_ADRESS					 	RED_USER_START_ADRESS          		+ 4

#define GREEN_USER_START_ADRESS      		 	BLUE_USER_START_ADRESS	         	+ 4	

#define YELLOW_USER_START_ADRESS  		   	GREEN_USER_START_ADRESS					 	+ 4		

#define ELECTRICITY_CUT_FLG_ADRESS  	    YELLOW_USER_START_ADRESS					+ 4

#define BUZZER_SETTING_ADRESS             ELECTRICITY_CUT_FLG_ADRESS        + 1

#define MEMMAP_LAST_USED_ADDRESS					BUZZER_SETTING_ADRESS     			 	+ 1
//EEPROM MEMORY MAP END


#define TOTAL_MEMORY_USED									MEMMAP_LAST_USED_ADDRESS - MEMMAP_START_ADDRESS

void SetupMemMap(void);
void SaveAllCrntSettings(void);
void LoadPreSettings(void);
#endif