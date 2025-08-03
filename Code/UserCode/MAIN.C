

#define _MAIN_C
#include <REG_MG82F6D17.h>
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"


#include "HardWare.h"
#include "SystemClock.h"
#include "SPI.h"
#include "Delay.h"
#include "Dice.h"
#include "shiftreg.h"
#include "LedBoard.h"
#include "UserDriverBridge.h"
#include "UserInput.h"
#include "Buzzer.h"
#include "USER.h"
#include "memmap.h"
#include "BOD.h"
#include "Settings.h"


void main()
{
	msDelay(100);
	InitClock();
  HardWareInit();
	InitSPI();
	InitBOD();
	INT_EnAll();	
  SetupMemMap();
	LoadPreSettings();
	
    while(1)
    {	
			RedUser();
		  GreenUser();
		  BlueUser();
		  YellowUser();
		  SettingsManager();
      DiceManager();
			DecodeManager();
			LedManager();
			ScanInput();
			CheckKeyVsTokn();
			BuzzerManager();
			ManageAnimation();
    }
		
}
