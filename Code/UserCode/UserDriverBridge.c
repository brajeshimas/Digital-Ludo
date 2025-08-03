

#include <REG_MG82F6D17.h>
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"

#include "UserDriverBridge.h"
#include "USER.h"
#include "LedBoard.h"


#define DECODE_INIT      0
#define DECODE_RED       1
#define DECODE_BLUE      2
#define DECODE_GREEN     3
#define DECODE_YELLOW    4
#define DECODE_NEXT      5





 uint8_t DcodedRed[4];
 uint8_t DcodedBlue[4];
 uint8_t DcodedGreen[4];
 uint8_t DcodedYellow[4];

uint8_t DecodeState;
uint8_t ToknNmbr = 0;

uint8_t PreRed[4]    = {0,0,0,0};
uint8_t PreBlue[4]   = {0,0,0,0};
uint8_t PreGreen[4]  = {0,0,0,0};
uint8_t PreYellow[4] = {0,0,0,0};

//void DcodeToknToLedNumber(uint8_t TokenNumber,uint8_t TokenValue,uint8_t ToknColour);
void DecodeManager(void)
{
  switch(DecodeState)
  {
    case DECODE_INIT:
    for(ToknNmbr = 0;ToknNmbr<4;ToknNmbr++)
    {
      DcodeToknToLedNumber(ToknNmbr,Red[ToknNmbr],RED);
      DcodeToknToLedNumber(ToknNmbr,Blue[ToknNmbr],BLUE);
      DcodeToknToLedNumber(ToknNmbr,Green[ToknNmbr],GREEN);
      DcodeToknToLedNumber(ToknNmbr,Yellow[ToknNmbr],YELLOW);
    } 
    ToknNmbr = 0;
    DecodeState = DECODE_RED;
    break;
    
    case DECODE_RED:
    if(PreRed[ToknNmbr] != Red[ToknNmbr])
    {PreRed[ToknNmbr] = Red[ToknNmbr];DcodeToknToLedNumber(ToknNmbr,Red[ToknNmbr],RED);}
    DecodeState = DECODE_BLUE;
    break;

    case DECODE_BLUE:
    if(PreBlue[ToknNmbr] != Blue[ToknNmbr])
    {PreBlue[ToknNmbr] = Blue[ToknNmbr];DcodeToknToLedNumber(ToknNmbr,Blue[ToknNmbr],BLUE);}
    DecodeState = DECODE_GREEN;
    break;

    case DECODE_GREEN:
    if(PreGreen[ToknNmbr] != Green[ToknNmbr])
    {PreGreen[ToknNmbr] = Green[ToknNmbr];DcodeToknToLedNumber(ToknNmbr,Green[ToknNmbr],GREEN);}
    DecodeState = DECODE_YELLOW;
    break;

    case DECODE_YELLOW:
    if(PreYellow[ToknNmbr] != Yellow[ToknNmbr])
    {PreYellow[ToknNmbr] = Yellow[ToknNmbr];DcodeToknToLedNumber(ToknNmbr,Yellow[ToknNmbr],YELLOW);}
    DecodeState = DECODE_NEXT;
    break;

    case DECODE_NEXT:
    ToknNmbr++;
    if(ToknNmbr == 4)ToknNmbr = 0;
    DecodeState = DECODE_RED;
    break;
    
  }
}
void DcodeToknToLedNumber(uint8_t TokenNumber,uint8_t TokenValue,uint8_t ToknColour)
{
  switch(ToknColour)
  {
    case RED:
    if(TokenValue == 0)
    {DcodedRed[TokenNumber] = RH1+TokenNumber;}
    else if(TokenValue < 57)
    {DcodedRed[TokenNumber] = TokenValue;}
    else if(TokenValue > 56 )
    {DcodedRed[TokenNumber] = TOKEN_OUT_R1 + TokenNumber;}
    break;


    case BLUE:
    if(TokenValue == 0)
    {DcodedBlue[TokenNumber] = BH1+TokenNumber;}
    
    else if((TokenValue+13)<= 51)
    {DcodedBlue[TokenNumber] = TokenValue+13;}
    
    else if(((TokenValue+13)> 51)&& ((TokenValue+13) < 65))
    {DcodedBlue[TokenNumber] = (TokenValue+13)-52;}

    else if(((TokenValue+13)> 64)&& ((TokenValue+13) < 70))
    {DcodedBlue[TokenNumber] = (TokenValue+13)-8;}
    
    else if((TokenValue+13) > 69 )
    {DcodedBlue[TokenNumber] = TOKEN_OUT_B1 + TokenNumber;}
    break;

    case GREEN:
    if(TokenValue == 0)
    {DcodedGreen[TokenNumber] = GH1+TokenNumber;}
    
    else if((TokenValue+26)<= 51)
    {DcodedGreen[TokenNumber] = TokenValue+26;}
    
    else if(((TokenValue+26)> 51)&& ((TokenValue+26) < 78))
    {DcodedGreen[TokenNumber] = (TokenValue+26)-52;}

    else if(((TokenValue+26)> 77)&& ((TokenValue+26) < 83))
    {DcodedGreen[TokenNumber] = (TokenValue+26)-16;}
    
    else if((TokenValue+26) > 82 )
    {DcodedGreen[TokenNumber] = TOKEN_OUT_G1 + TokenNumber;}
    break;


    case YELLOW:
    if(TokenValue == 0)
    {DcodedYellow[TokenNumber] = YH1+TokenNumber;}
    
    else if((TokenValue+39)<= 51)
    {DcodedYellow[TokenNumber] = TokenValue+39;}
    
    else if(((TokenValue+39)> 51)&& ((TokenValue+39) < 91))
    {DcodedYellow[TokenNumber] = (TokenValue+39)-52;}

    else if(((TokenValue+39)> 90)&& ((TokenValue+39) < 96))
    {DcodedYellow[TokenNumber] = (TokenValue+39)-24;}
    
    else if((TokenValue+39) > 95 )
    {DcodedYellow[TokenNumber] = TOKEN_OUT_Y1 + TokenNumber;}
    break;
  }
}