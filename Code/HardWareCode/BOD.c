
#include <Intrins.h>
#include ".\include\REG_MG82F6D17.H"
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"

#include "eeprom.h"
#include "memmap.h"
bit bBOD1Flag;



/***********************************************************************************
Function:   	u8 IAP_ReadPPage(u8 PsfrAddr)
Description:	read P page sfr
Input:    		u8 PsfrAddr: sfr Address
Output:      	u8: sfr data
*************************************************************************************/
/*
u8 IAP_ReadPPage(u8 PsfrAddr)
{
	bit bEA=EA;
	EA = 0; 					
	IFADRH = 0; 				// IFADRH must be 0
	IFADRL= PsfrAddr;			// sfr Address
	IFMT = ISP_READ_P;			// read P page sfr
	ISPCR = 0x80;				// Enable ISP/IAP
	SCMD = 0x46;
	SCMD = 0xB9;
	nop();
	IFMT=0;
	ISPCR = 0;					// clear
	EA = bEA;					
	return IFD;					// return sfr data
}
*/
/***********************************************************************************
Function:		void IAP_WritePPage(u8 PsfrAddr,u8 PsfrData)
Description:	write P page sfr
Input:   
		 		u8 PsfrAddr: sfr Address
		 		u8 PsfrData: sfr data
Output:     
*************************************************************************************/
void IAP_WritePPage(u8 PsfrAddr,u8 PsfrData)
{
	bit bEA=EA;
	EA = 0; 					//
	IFADRH = 0; 				// IFADRH must be 0
	IFADRL= PsfrAddr;			// sfr Address
	IFD= PsfrData;				// sfr data
	IFMT = ISP_WRITE_P;			// write P page sfr
	ISPCR = 0x80;				// Enable ISP/IAP
	SCMD = 0x46;
	SCMD = 0xB9;
	nop();
	IFMT=0;
	ISPCR = 0;					// clear
	EA = bEA;					
}



/***********************************************************************************
Function:   void INT_SF(void)
Description:SF Interrupt handler
Input:   
Output:     
*************************************************************************************/
void INT_SF(void) interrupt INT_VECTOR_SF
{
	if((PCON1 & BOF0)!=0)
	{ // BOD0 Interrupt, 1.7V
		PCON1 = BOF0;
	}

	if((PCON1 & BOF1)!=0)
	{ // BOD1 ÖÐ¶Ï, 4.2V/3.7V/2.4V/2.0V
		PCON1 = BOF1;
		bBOD1Flag = 1;
		INT_DisSF_BOD1();			// Disable BOD1 interrupt
		
	}

	if((PCON1 & WDTF)!=0)
	{ // WDT ÖÐ¶Ï
		PCON1 = WDTF;
	}

	if((PCON1 & RTCF)!=0)
	{ // RTC ÖÐ¶Ï
		PCON1 = RTCF;
	}

}





/***********************************************************************************
Function:   	void InitInterrupt()
Description:	Initialize Interrupt
Input:   
Output:   		
*************************************************************************************/
void InitInterrupt(void)
{
	INT_EnSF_BOD1();					// Enable BOD1 interrupt
	INT_EnSF();							// Enable SF interrupt
}	



/***********************************************************************************
Function:   	void InitBOD()
Description:	Initialize BOD
Input:   
Output:   		
*************************************************************************************/
void InitBOD(void)
{
	u8 x;
	x=BIT0;
	
	x=x|POW_BOD1_Enable;				// Enable BOD1
	//x=x|POW_BOD1_Disable;				// Disable BOD1

	x=x|POW_BOD1_Set42V;				// BOD1 Detecting 4.2V
	//x=x|POW_BOD1_Set37V;				// BOD1 Detecting 3.7V
	//x=x|POW_BOD1_Set24V;				// BOD1 Detecting 2.4V
	//x=x|POW_BOD1_Set20V;				// BOD1 Detecting 2.0V
	
	//x=x|POW_BOD1_Enable_OnPD;			// Enable BOD1 on PD mode
	x=x|POW_BOD1_Disable_OnPD;			// Disable BOD1 on PD mode

	
	//x=x|POW_BOD1_EnReset;				// Enable BOD1 to trigger a MCU reset
	x=x|POW_BOD1_DisReset;			// Disable BOD1 to trigger a MCU reset

	//x=x|POW_BOD0_EnReset;				// Enable BOD0(1.7V) to trigger a MCU reset
	//x=x|POW_BOD0_DisReset;			// Disable BOD0(1.7V) to trigger a MCU reset
	
	IAP_WritePPage(PCON2_P, x);	
 
	InitInterrupt();

	
}	
