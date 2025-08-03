#include ".\include\REG_MG82F6D17.H"
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"
#include <Intrins.h>
#include <Absacc.h>


#include "eeprom.h"
#include "memmap.h"






//Total Flash Size of MG82F6D17 is 16KB i.e. 16384 Bytes or 4000h Bytes.
//Note: Flash may be divided into 3 parts i.e. AP, IAP and ISP. 
//			We are not _using ISP bootloader therefore ISP is 0.
//			AP is for the Code area and IAP is for non-volatile 
//			Data storage.
//The range of CODE flash area(AP+IAP) is 0x0000 to 0x3FFF
//Since the counting starts from 0 therefore the last addressable 
//Code flash byte will be 0x3FFF.

//Flash Page Size is 512 bytes (0x200). 
//Writing to single Flash Byte is not possible. The whole page is 
//to be erased before writing. So Write is Page wise.

//The last 512 bytes of FLASH is used as EEPROM to store
//application settings Data (IAP).

//Address Range of last 512 bytes will be 0x3E00 to 0x3FFF
//i.e.	IAP SIZE = 512 Bytes (0x200)
//			IAP HIGH BOUNDARY is 0x3FFF
//			and IAP LOW BOUNDARY is (0x3FFF - IAP Size + 1) = 3E00

//We are not loading IAP Boundary values in firmware as it will be loaded by the 
//ISP programmer.



//Reserve the last Flash Page of 512 bytes starting from 0x3E00 for EEPROM purpose.
//The next line tells the compiler to not to write any program Code in this area.
code char EEPROM_BLOCK [EEPROM_MAX_SIZE] _at_ MEMMAP_START_ADDRESS;


//The first EEPROM Data will be stored at 0x3E00
//The last EEPROM Data will be stored at 0x3FFF

unsigned char EEPROM_Read_Byte(uint16_t code *address)
{
	uint8_t rdata;
	rdata = *address>>8;
	return rdata;
}
/*
unsigned int EEPROM_Read_Int(uint16_t code *address)
{
	unsigned int ReturnData;
	ReturnData = *address;
	return ReturnData;
}

double EEPROM_Read_Double(double code *address)
{

	double ReturnData;
	ReturnData = *address;
	return ReturnData;
}
*/

void EEPROM_Write_nByte(unsigned int u16EPAddr,unsigned char *u8EPData,unsigned char sizeofdata)
{
	bit bEA=EA;
	unsigned int idata looptmp=0;
	unsigned int idata u16_addrl_r;
	unsigned int idata RAMtmp;
	unsigned char xdata page_buffer[25];
	
	//Next Line will always load u16_addrl_r with 0x3E00, i.e. IAP Page Start Address, check to remove redundant Code
	u16_addrl_r= 0x3E00;																	       	
																																		//Save IAP Data to XRAM	
	for(looptmp=0;looptmp<25;looptmp++)												
	{
		RAMtmp = EEPROM_Read_Byte((unsigned int code *)(u16_addrl_r + looptmp));
		page_buffer[looptmp]=RAMtmp;
	}
	
	for(looptmp=0;looptmp<sizeofdata+1;looptmp++)												
	{
     page_buffer[((u16EPAddr+looptmp) & 0x1FF)] = u8EPData[looptmp];											//Modify customer Data in XRAM
	}

		IAP_ErasePage(0x3E);																						//Erase Last Page (0x3E00) last page of 512 Bytes	
	
	
		//Prepare for writing the XRAM buffer of 512 bytes to IAP
	
		EA = 0;																	// Disable Global Interrupt	
		ISPCR = 0x80; 													// Enable ISP/IAP Programming
		IFMT = ISP_WRITE;												// Set the Flash Mode to ISP WRITE BYTE	
	
		//for(looptmp=0;looptmp<0x200;looptmp++)	// Loop for 512 bytes to be written to flash IAP	
	looptmp = TOTAL_MEMORY_USED;
		for(looptmp=0;looptmp<TOTAL_MEMORY_USED;looptmp++)	// Loop for 512 bytes to be written to flash IAP	
		{
			IFADRL = (u16_addrl_r&0xff)+looptmp;	// Load the Add High byte of the Flash IAP with loop index
      IFADRH = (u16_addrl_r>>8)&0xff;				// Load the Add Low byte of the Flash IAP
			IFD = page_buffer[looptmp];						// Load the Data byte to be written to Flash IAP
						

			SCMD = 0x46;													// Trigger Engine for Program Seq1of2

			SCMD = 0xB9;													// Trigger Engine for Program Seq2of2
			nop();																// CPU will come here after finish processing	
		}
		
		IFMT=0;																	// Select Flash Mode to Standby
		ISPCR = 0;															// Disable Flash ISP/IAP Programming
		EA = bEA; 															// Rinstate the Interrupt state
}	

/*
void EEPROM_Clear()
{
	EEPROM_Write_Byte(MEMMAP_EEPROM_INIT_FLAG, EEPROM_UNINITIALIZED);
	POW_ResetToAP();													// Reset the uC and force start to AP Memory
}
*/

void IAP_ErasePage(u8 ByteAddr)
{
	bit bEA=EA;																// Backup the Interrupt Register
	IFADRH = ByteAddr;												// Load Page Address High Byte (IAP Address)
	IFADRL= 0x00;															// Load Page Address Low Byte as 0x00 (start page)
	EA = 0;																		// Disable Global Interrupt
	IFMT = ISP_ERASE;													// Set the Flash Mode to ErasePage
	ISPCR = 0x80; 														// Enable ISP/IAP Programming

	SCMD = 0x46;															// Trigger Engine for Program Seq1of2
	SCMD = 0xB9;															// Trigger Engine for Program Seq2of2		
	nop();																		// CPU will come here after finish processing	
	IFMT=0;																		// Select Flash Mode to Standby
	ISPCR = 0;																// Disable Flash ISP/IAP Programming
	EA = bEA; 																// Rinstate the Interrupt state
}


void IAP_WriteByte(unsigned int ByteAddr, unsigned char ByteData)
{
	bit bEA=EA;																// Backup the Interrupt Register
	IFD = ByteData;														// Load the Data byte to be written to Flash IAP
	IFADRH = ByteAddr>>8;											// Load the Add High byte of the Flash IAP
	IFADRL= ByteAddr;													// Load the Add Low byte of the Flash IAP
	EA = 0;																		// Disable Global Interrupt
	IFMT = ISP_WRITE;													// Set the Flash Mode to ISP WRITE BYTE	
	ISPCR = 0x80; 														// Enable ISP/IAP Programming

	SCMD = 0x46;															// Trigger Engine for Program Seq1of2

	SCMD = 0xB9;															// Trigger Engine for Program Seq1of2
	
	nop();																		// CPU will come here after finish processing	
	IFMT=0;																		// Select Flash Mode to Standby
	ISPCR = 0;																// Disable Flash ISP/IAP Programming
	EA = bEA; 																// Rinstate the Interrupt state
}