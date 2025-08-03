

#include <REG_MG82F6D17.h>
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"

#include "LedBoard.h"
#include "shiftreg.h"
#include "UserDriverBridge.h"
#include "HardWare.h"
#include "SPI.h"

uint8_t DataPkt[5] = {0,0,0,0,0};
uint8_t LedState = 0;
uint8_t LedColm = 0;
uint32_t LedUpdateTimer = 0;

uint8_t ActiveLedColm = 0;

void GetTokenLocation(uint8_t pos,uint8_t LocationHolder[2]);
uint8_t GetColmNumber(uint8_t LedNumber);
void SetRowColm(uint8_t LedNumber,uint8_t LedColour);
void nShiftOut(uint8_t NumberOfReg);
void ResetLedDsply(void);
void StrtLedUpdateTimer(uint32_t TimerVal);
bit IsLedTimerExpr(void);
uint8_t SearchAndSetRow(uint8_t LedColm,uint8_t LedNumber,uint8_t LedColour);
void ClearRowColm(void);

void LedManager(void)
{
  int i=0 ;
  uint8_t scount = 0;
  switch(LedState)
  {
    case LED_INIT:
    ResetLedDsply();
    StrtLedUpdateTimer(0);
    LedColm = 1;
    LedState = LED_IDLE;
    break;
    
    case LED_IDLE:
    if(IsLedTimerExpr())
    {
      ClearRowColm();
      LedState = LED_SET_ROW_COLM;
    }
    break;
    
    case LED_SET_ROW_COLM:
    for(i = 0;i<4;i++)
    {
     scount += SearchAndSetRow(LedColm,DcodedRed[i],RED);
     scount += SearchAndSetRow(LedColm,DcodedBlue[i],BLUE);
     scount += SearchAndSetRow(LedColm,DcodedGreen[i],GREEN);
     scount += SearchAndSetRow(LedColm,DcodedYellow[i],YELLOW);
    }

    LedColm++;
    if(LedColm == 17)LedColm = 0;
		ActiveLedColm = LedColm-1;
    if((scount > 0)||(ActiveLedColm >= 13))
    {
      scount = 0;
      LedState = LED_UPDATE;
    }
		
    else
    break;

    case LED_UPDATE:
    nShiftOut(5);
		digitalWrite(LED_LTCH_PIN,0);
    digitalWrite(LED_LTCH_PIN,1);
    StrtLedUpdateTimer(1);
    LedState = LED_IDLE;
    break;
  }
  
}



//--------FUNCTIONS FOR LED_MANAGER-----------
void ResetLedDsply(void)
{
  DataPkt[0] = 0xFF;
  DataPkt[1] = 0xFF;
  DataPkt[2] = 0xFF;
  DataPkt[3] = 0;
  DataPkt[4] = 0;
  nShiftOut(5);
}

void ClearRowColm(void)
{
  DataPkt[0] = 0xFF;
  DataPkt[1] = 0xFF;
  DataPkt[2] = 0xFF;
  DataPkt[3] = 0;
  DataPkt[4] = 0; 
}

void StrtLedUpdateTimer(uint32_t TimerVal)
{
   LedUpdateTimer = TimerVal;
}

bit IsLedTimerExpr(void)
{
  if(LedUpdateTimer == 0)
  return 1;
  else
  return 0;
}
//---------------------------------------------


//----FUNC FOR MANAGE TIMER---------------------
void LedTimer(void)
{
  if(LedUpdateTimer>0)
  {
    LedUpdateTimer--;
  }
}
//----------------------------------------------

uint8_t SearchAndSetRow(uint8_t LedColm,uint8_t LedNumber,uint8_t LedColour)
{

  if(GetColmNumber(LedNumber) == LedColm)
  { 
   SetRowColm(LedNumber,LedColour);
   return 1;
  }

	else
		return 0;
  
}

uint8_t GetColmNumber(uint8_t LedNumber)
{
  uint8_t ColmRow[2];
  GetTokenLocation(LedNumber,ColmRow);
  return ColmRow[0];
}
	
void SetRowColm(uint8_t LedNumber,uint8_t LedColour)
{
  uint8_t colmNumber,rowNumber;
  uint8_t ColmRow[2];
  uint8_t subRow = 0;
  int subRow1 = -1;
  uint16_t Colm = 0;
  
   GetTokenLocation(LedNumber,ColmRow);
   colmNumber = ColmRow[0];
   rowNumber = ColmRow[1];

  //DataPkt[0-2] contain Row 0-20 and last 4bit is reserved for ohter work
  //DataPkt[3-4] contain Colm 1-16
   bitSet(Colm,(15-(colmNumber-1)));
   DataPkt[3] |= HIBYTE(Colm);
   DataPkt[4] |= LOBYTE(Colm); 
	  

   
  //--------Find Sub Row Set That between 0-20---------------------
  switch(LedColour)
  {
    case RED:   subRow = (rowNumber*3)+0;                 break; 
    case GREEN: subRow = (rowNumber*3)+1;                 break;
    case BLUE:  subRow = (rowNumber*3)+2;                 break;  
    case YELLOW:subRow = (rowNumber*3);subRow1 = subRow+1;break;
  }
  //---------------------------------------------------------------

    if(subRow < 8){ bitClear(DataPkt[0] ,7-subRow);}
    else if(subRow < 16){bitClear(DataPkt[1] ,15-subRow);}
    else if(subRow < 24){bitClear(DataPkt[2] ,23-subRow);}
    
    if((subRow1 > -1)&&(subRow1 < 8)){bitClear(DataPkt[0] ,7-subRow1);} 
    else if((subRow1 > -1)&&(subRow1 < 16)){bitClear(DataPkt[1] ,15-subRow1);}
    else if((subRow1 > -1)&&(subRow1 < 24)){bitClear(DataPkt[2] ,23-subRow1);}

}





void GetTokenLocation(uint8_t pos,uint8_t LocationHolder[2])
{
  uint8_t Row,colm;
  //pos means LedPositions
  if(pos < 6 )                    {colm = 1;Row = pos;}
  else if((pos > 5)&&(pos <12))   {colm = 12;Row = (11-pos);}
  else if(pos == 12)              {colm = 11;Row = 0;}
  else if((pos > 12)&&(pos < 19)) {colm = 10;Row = (5-(18-pos));}
  else if((pos > 18)&&(pos < 25)) {colm = 9;Row = 24-pos;}
  else if((pos == 25))            {colm = 8;Row = 0;}
  else if((pos > 25)&&(pos < 32)) {colm = 7;Row = (5-(31-pos));}
  else if((pos > 31)&&(pos < 38)) {colm = 6;Row = 37-pos;}
  else if((pos == 38))            {colm = 5;Row = 0;}
  else if((pos > 38)&&(pos <45))  {colm = 4;Row = (5-(44-pos));}
  else if((pos > 44)&&(pos < 51)) {colm = 3;Row = 50-pos;}
  else if((pos == 51))            {colm = 2;Row = 0;}
  else 
  {
    switch(pos)
    {
      case R1:colm = 2;Row = 1;break;
      case R2:colm = 2;Row = 2;break;
      case R3:colm = 2;Row = 3;break;
      case R4:colm = 2;Row = 4;break;
      case R5:colm = 2;Row = 5;break;
      
      case B1:colm = 11;Row = 1;break;
      case B2:colm = 11;Row = 2;break;
      case B3:colm = 11;Row = 3;break;
      case B4:colm = 11;Row = 4;break;
      case B5:colm = 11;Row = 5;break;
      
      case G1:colm = 8;Row = 1;break;
      case G2:colm = 8;Row = 2;break;
      case G3:colm = 8;Row = 3;break;
      case G4:colm = 8;Row = 4;break;
      case G5:colm = 8;Row = 5;break;
      
      case Y1:colm = 5;Row = 1;break;
      case Y2:colm = 5;Row = 2;break;
      case Y3:colm = 5;Row = 3;break;
      case Y4:colm = 5;Row = 4;break;
      case Y5:colm = 5;Row = 5;break;
      
      //For Home Led ex RH1 :- RED HOME first Led
      case RH1:colm = 13;Row = 5;break;
      case RH2:colm = 13;Row = 4;break;
      case RH3:colm = 13;Row = 3;break;
      case RH4:colm = 13;Row = 2;break;
        
      case BH1:colm = 14;Row = 5;break;
      case BH2:colm = 14;Row = 4;break;
      case BH3:colm = 14;Row = 3;break;
      case BH4:colm = 14;Row = 2;break;
      
      case GH1:colm = 15;Row = 5;break;
      case GH2:colm = 15;Row = 4;break;
      case GH3:colm = 15;Row = 3;break;
      case GH4:colm = 15;Row = 2;break;
        
      case YH1:colm = 16;Row = 5;break;
      case YH2:colm = 16;Row = 4;break;
      case YH3:colm = 16;Row = 3;break;
      case YH4:colm = 16;Row = 2;break;
     
      case TOKEN_OUT_R1:colm = 1;Row = 6;break;
      case TOKEN_OUT_R2:colm = 2;Row = 6;break;
      case TOKEN_OUT_R3:colm = 3;Row = 6;break;
      case TOKEN_OUT_R4:colm = 4;Row = 6;break;

      case TOKEN_OUT_B1:colm = 10;Row = 6;break;
      case TOKEN_OUT_B2:colm = 11;Row = 6;break;
      case TOKEN_OUT_B3:colm = 12;Row = 6;break;
      case TOKEN_OUT_B4:colm = 14;Row = 6;break;

      case TOKEN_OUT_G1:colm = 7;Row = 6;break;
      case TOKEN_OUT_G2:colm = 8;Row = 6;break;
      case TOKEN_OUT_G3:colm = 9;Row = 6;break;
      case TOKEN_OUT_G4:colm = 15;Row = 6;break;

      case TOKEN_OUT_Y1:colm = 4;Row = 6;break;
      case TOKEN_OUT_Y2:colm = 5;Row = 6;break;
      case TOKEN_OUT_Y3:colm = 6;Row = 6;break;
      case TOKEN_OUT_Y4:colm = 16;Row = 6;break;
				
    }
		
  }
  
  LocationHolder[0] = colm;
  LocationHolder[1] = Row;
}
void nShiftOut(uint8_t NumberOfReg)
{
  int i= 0;
  NumberOfReg -= 1;
  for(i= NumberOfReg;i>=0;i--)
  {
		SPI_tranfer(DataPkt[i]);
	}
	
}


/*
next Code will be Written Here
*/












