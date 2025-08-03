#include ".\include\REG_MG82F6D17.H"
#include ".\include\API_Macro_MG82F6D17.H"
#include ".\include\Type.h"

#include "shiftreg.h"
#include "HardWare.h"
#include "Delay.h"


void shiftOut(bit DataPin , bit ClkPin,bit BitOrder,uint8_t Data )
{
		 int j;
		 if(BitOrder)
		 {
				for(j=0;j<=7;j++)
				{  				        				
					digitalWrite(DataPin,bitRead(Data,j));
					digitalWrite(ClkPin,0);
					digitalWrite(ClkPin,1);					
				} 
				
		 }
		 
		 else 
		 {
				for(j=0;j<=7;j++)
				{   				
					 digitalWrite(DataPin,bitRead(Data,(7-j)));            //LSBFIRST
					 digitalWrite(ClkPin,0);
					 digitalWrite(ClkPin,1);  
				} 
		 }

}

