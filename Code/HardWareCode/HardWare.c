#include <REG_MG82F6D17.h>
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"

#include "HardWare.h"
#include "timers.h"

void HardWareInit(void)
{
//------------FOR SPI---------------
	
	//CS_PIN P17 , MOSI/DATA(SHIFTREG)  P35
	//MISO/SIN(SerialInput of Shiftreg) P34
	//SPICLK/CLK(SHIFTREG)  P33

//-----------------------------------
	
	Timer0_INIT();
	INT_EnAll();
}