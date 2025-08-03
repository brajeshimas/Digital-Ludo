


#include <Intrins.h>
#include <REG_MG82F6D17.h>
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"


#include "SystemClock.h"
#include "Delay.h"

void usDelay(u8 xUs)
{
	while(xUs!=0)
	{
#if (MCU_CPUCLK>=11059200)
		_nop_();
#endif
#if (MCU_CPUCLK>=14745600)
		_nop_();
		_nop_();
		_nop_();
		_nop_();
#endif
#if (MCU_CPUCLK>=16000000)
		_nop_();
#endif

#if (MCU_CPUCLK>=22118400)
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
#endif
#if (MCU_CPUCLK>=24000000)
		_nop_();
		_nop_();
#endif		
#if (MCU_CPUCLK>=29491200)
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
#endif
#if (MCU_CPUCLK>=32000000)
		_nop_();
		_nop_();
#endif

		xUs--;
	}
}


void msDelay(u16 xMs)
{
	while(xMs!=0)
	{
		CLRWDT();
		usDelay(200);
		usDelay(200);
		usDelay(200);
		usDelay(200);
		usDelay(200);
		xMs--;
		
	}
}
