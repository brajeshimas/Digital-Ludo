
#include <REG_MG82F6D17.h>
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"

#include "SPI.h"


uint8_t SPI_tranfer(u8 Data)
{
	SPI_SendData(Data);							    // Send data
	while(SPI_ChkCompleteFlag()==0);		// Wait complete
	SPI_ClearCompleteFlag();						// Clear flag
	return SPI_GetData();	
}


void InitSPIPort(void)
{
	
	/*
	PORT_SetP1PushPull(BIT5|BIT7);				// Set P15(MOSI),P17(SPICLK) as push-pull for output
	PORT_SetP1OpenDrainPu(BIT6);				  // Set P16(MISO) as open-drain with pull-high for digital input
	PORT_SetP3PushPull(BIT3);					    // Set P33(nSS) as push-pull for output
	PORT_P14P15P16P17_HighDrive();  
	PORT_P1_EnFastDrive(BIT5|BIT6|BIT7);
	PORT_P3_EnFastDrive(BIT3);
	*/
	


	PORT_SetP3PushPull(BIT3|BIT5);//MOSI AND SPICLK
	PORT_SetP3OpenDrain(BIT4);//MISO
	PORT_SetP1PushPull(BIT7); //For LedLatch
	PORT_SetP3PushPull(BIT1); //For DiceLatch
		
	PORT_SetP3PushPull(BIT0);//for buzzer
	P30 = 0;
	
	PORT_SetP1OpenDrain(BIT0|BIT1|BIT5|BIT6);//FOR KEY ROW INPUT
	P10 = 1;
	P11 = 1;
	P15 = 1;
	P16 = 1;
	PORT_SetP6PushPull(BIT0|BIT1);//for input key colm scaning
	PORT_SetP2PushPull(BIT2|BIT4);//for input key colm scaning
	P60 = 0;P61 = 0;P22 = 0;P24 = 0;

	PORT_P30P31P32P33_HighDrive();
	PORT_P34P35P36P37_HighDrive();
	
	PORT_P3_EnFastDrive(BIT0|BIT1|BIT3|BIT5);
	PORT_P1_EnFastDrive(BIT7);

  //PORT_P1_EnFastDrive(BIT0|BIT1|BIT5|BIT6);
	
	


}


void InitSPI(void)
{
	InitSPIPort();
	SPI_Enable();									// Enable SPI
	SPI_SelectMASTERByMSTRbit();					// Set to MASTER
	SPI_SetClock(SPI_CLK_SYSCLK_2);					// Set Clock SYSCLK/4 48M/4=12M
	SPI_SetCPOL_0();								// CPOL=0 
	SPI_SetDataLSB();								// Data MSB
	SPI_SetCPHA_0();								// CPHA=1
	// SPI  nSS/MOSI/MISO/SPICLK IO Port
	SPI_SetUseP17P35P34P33();			
}




