




#include <REG_MG82F6D17.h>
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"
/***********************************************************************************
Function:   	void InitTimer3_8bit_PWM(void)
Description: 	Initialize Timer3 for 8bit PWM
Input:   
Output:     
*************************************************************************************/
void InitTimer3_8bit_PWM(void)
{
	TM_SetT3Mode_8bitPWM();							        // T3 mode: 8bit PWM
	TM_SetT3Clock_SYSCLKDiv12();					      // T3 clock source: SYSCLK/12
	
	//TM_SetT3LowByte(T3_PWM_DUTY_10);			      // T3 low byte	(duty) 			
	//TM_SetT3RLLowByte(T3_PWM_DUTY_10);				  // T3 low byte reload  (duty)

	//TM_SetT3HighByte(256-T3_PWM_STEP_MAX);			// T3 high byte	(freq)  
	//TM_SetT3RLHighByte(256-T3_PWM_STEP_MAX);		// T3 high byte	reload (freq)

	TM_EnT3CKO();                                // Enable T3CKO output
	TM_EnableT3();									            // Enable T3
								            
}