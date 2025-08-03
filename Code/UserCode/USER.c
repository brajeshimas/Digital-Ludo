


#include <REG_MG82F6D17.h>
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"
#include "USER.h"
#include "Dice.h"
#include "UserInput.h"
#include "LedBoard.h"
#include "UserDriverBridge.h"
#include "Buzzer.h"

#define USER_INIT                   0
#define USER_IDLE                   1
#define ROLL_DICE                   2
#define GET_DICE                    3
#define DICE_VALIDITY_CHECK         4
#define USER_MOVE_TOKEN             5
#define USER_TOKEN_PROCESS          6
#define USER_TOKEN_ANIMATING        7
#define USER_RUN_NEXT_CHANCE        8




//animation manager------------------------------------------
#define ANMTN_INIT                     0
#define ANMTN_IDLE                     1
#define ANIMATE_FORWARD1               2
#define ANIMATE_FORWARD2               3
#define ANMTN_CHECK_CONFLICT           4
#define ANIMATE_BACKWARD1              5
#define ANIMATE_BACKWARD2              6


#define ANIMATE_FORWARD_TIME          200  //milli seconds
#define ANIMATE_BACKWARD_TIME         100//115  //milli seconds

volatile uint8_t idata AnimtnState = 0;
bit AnimateCmd;
bit AnimationCmpltFlg;
volatile uint16_t idata AnimationTimer;
volatile uint8_t idata AnmtngTokenColour;
volatile uint8_t idata AnmtngTokenNumber;
volatile uint8_t idata AnmtngTokenValue;
 uint8_t conflictTknNmbr;
 uint8_t conflictTknClr;
bit ConflictFlg;
bit AnmtnBusy=0;
//uint8_t CheckAndGetConflictId(uint8_t srcColour,uint8_t srcTknNumber);
void CheckAndGetConflictId(uint8_t srcColour,uint8_t srcTknNumber);
uint8_t IsToknOnBaithki(uint8_t LedNumber);
//---------animation manager end------------------------------------------


volatile uint8_t idata Red[4] = {0,0,0,0}; 
volatile uint8_t idata Blue[4] = {0,0,0,0};
volatile uint8_t idata Green[4] = {0,0,0,0};
volatile uint8_t idata Yellow[4] = {0,0,0,0};


uint8_t RedUserState;
bit RedUserFlg;

uint8_t BlueUserState;
bit BlueUserFlg;

uint8_t GreenUserState;
bit GreenUserFlg;

uint8_t YellowUserState;
bit YellowUserFlg;

uint8_t DiceResult;
uint8_t NewToknValue;
uint8_t NToknNumber;

uint8_t DiceRollChance;



bit IsDiceValueValid(uint8_t DiceValue,uint8_t UserBuff[4]);
uint8_t MoveToken(uint8_t OldTokenNumber,uint8_t DiceVal);
bit IsAllTokenMoved(uint8_t Token[4]);
bit IsOnlyTokenMovable(uint8_t ToknClr,uint8_t DiceValue);
void RedUser(void)
{
	switch(RedUserState)
	{
		
	case USER_INIT:
		RedUserFlg = 1;
		RedUserState = USER_IDLE;
	  SetArrowPointTo(RED);
		break;
	
	case USER_IDLE:
		if(RedUserFlg)
		{
			RedUserFlg = 0;
			if(!IsAllTokenMoved(Red))
			RedUserState = ROLL_DICE;
			else
			{BlueUserFlg = 1;SetArrowPointTo(BLUE);}
		}
		break;
		
	case ROLL_DICE:
		if(UserInput == RED_KEY2)
		{
			UserInput = 0;
			RollTheDice();
			RedUserState = GET_DICE;
		}
		break;
	
	case GET_DICE:
		if(ReadDiceValue()>0)
		{
			DiceResult = ReadDiceValue();
			RedUserState = DICE_VALIDITY_CHECK;
		}
 		break;
		
	case DICE_VALIDITY_CHECK:
		if(IsDiceValueValid(DiceResult,Red))
		{
		   RedUserState = USER_MOVE_TOKEN;CheckToknFlg = 1;
			if(IsOnlyTokenMovable(RED,DiceResult))
      {
			  RedUserState = USER_TOKEN_PROCESS;CheckToknFlg = 0;
			}
		}
		else
		{RedUserState = USER_IDLE;BlueUserFlg = 1;SetArrowPointTo(BLUE);}
		break;
		
	case USER_MOVE_TOKEN:
		if((UserInput == RED_KEY1)&&(KeyPressedOkFlg == 1))
		{
			CheckToknFlg = 0;KeyPressedOkFlg = 0;
			NewToknValue = MoveToken(Red[0],DiceResult);
			NToknNumber = 0;RedUserState = USER_TOKEN_PROCESS;
			
		}
		
		else if((UserInput == RED_KEY2)&&(KeyPressedOkFlg == 1))
		{
			CheckToknFlg = 0;KeyPressedOkFlg = 0;
			NewToknValue = MoveToken(Red[1],DiceResult);
			NToknNumber = 1;RedUserState = USER_TOKEN_PROCESS;		
		}
		
		else if((UserInput == RED_KEY3)&&(KeyPressedOkFlg == 1))
		{
			CheckToknFlg = 0;KeyPressedOkFlg = 0;
			NewToknValue = MoveToken(Red[2],DiceResult);
			NToknNumber = 2;RedUserState = USER_TOKEN_PROCESS;
			
		}
		
		else if((UserInput == RED_KEY4)&&(KeyPressedOkFlg == 1))
		{
			CheckToknFlg = 0;KeyPressedOkFlg = 0;
			NewToknValue = MoveToken(Red[3],DiceResult);
			NToknNumber = 3;RedUserState = USER_TOKEN_PROCESS;		
		}
    break;
		
	case USER_TOKEN_PROCESS:
		if(NewToknValue>0)
		{
			UserInput = 0;
		  TokenAnimator(NToknNumber,RED,NewToknValue);
	    RedUserState = USER_TOKEN_ANIMATING;
		}
		else
		{
		 UserInput = 0;
		 CheckToknFlg = 1;
		 RedUserState = USER_MOVE_TOKEN;	
		}
		break;
	
	case USER_TOKEN_ANIMATING:
		if(AnimationCmpltFlg)
		{
			if(DiceResult == 6)DiceRollChance +=1;
			if(ConflictFlg){ConflictFlg = 0;DiceRollChance +=1;}
			if(NewToknValue == 57)DiceRollChance +=1;
			RedUserState = USER_RUN_NEXT_CHANCE;
		}
		break;
	case USER_RUN_NEXT_CHANCE:
		if(DiceRollChance>0)
		{
			if(!IsAllTokenMoved(Red))
			{
				DiceRollChance--;
			  RedUserState = ROLL_DICE;
			}
			else
			{
				BlueUserFlg = 1;SetArrowPointTo(BLUE);
				DiceRollChance = 0;
			  RedUserState = USER_IDLE;
			}
		}
		else
		{
			BlueUserFlg = 1;
			SetArrowPointTo(BLUE);
			RedUserState = USER_IDLE;
		}
		break;
	
  }
}





void BlueUser(void)
{
	switch(BlueUserState)
	{
		
	case USER_INIT:
		BlueUserFlg = 0;
		BlueUserState = USER_IDLE;
		break;
	
	case USER_IDLE:
		if(BlueUserFlg)
		{
			BlueUserFlg = 0;
			if(!IsAllTokenMoved(Blue))
			BlueUserState = ROLL_DICE;
			else
			{GreenUserFlg = 1;SetArrowPointTo(GREEN);}
		}
		break;
		
	case ROLL_DICE:
		if(UserInput == BLUE_KEY1)
		{
			UserInput = 0;
			RollTheDice();
			BlueUserState = GET_DICE;
		}
		break;
	
	case GET_DICE:
		if(ReadDiceValue()>0)
		{
			DiceResult = ReadDiceValue();
			BlueUserState = DICE_VALIDITY_CHECK;
		}
 		break;
		
	case DICE_VALIDITY_CHECK:
		if(IsDiceValueValid(DiceResult,Blue))
		{
			BlueUserState = USER_MOVE_TOKEN;CheckToknFlg = 1;
			if(IsOnlyTokenMovable(BLUE,DiceResult))
      {
			  BlueUserState = USER_TOKEN_PROCESS;CheckToknFlg = 0;
			}
		}
		else
		{BlueUserState = USER_IDLE;GreenUserFlg = 1;SetArrowPointTo(GREEN);}
		break;
		
	case USER_MOVE_TOKEN:
		if((UserInput == BLUE_KEY1)&&(KeyPressedOkFlg == 1))
		{
			CheckToknFlg = 0;KeyPressedOkFlg = 0;
			NewToknValue = MoveToken(Blue[0],DiceResult);
			NToknNumber = 0;BlueUserState = USER_TOKEN_PROCESS;
			
		}
		
		else if((UserInput == BLUE_KEY2)&&(KeyPressedOkFlg == 1))
		{
			CheckToknFlg = 0;KeyPressedOkFlg = 0;
			NewToknValue = MoveToken(Blue[1],DiceResult);
			NToknNumber = 1;BlueUserState = USER_TOKEN_PROCESS;		
		}
		
		else if((UserInput == BLUE_KEY3)&&(KeyPressedOkFlg == 1))
		{
			CheckToknFlg = 0;KeyPressedOkFlg = 0;
			NewToknValue = MoveToken(Blue[2],DiceResult);
			NToknNumber = 2;BlueUserState = USER_TOKEN_PROCESS;
			
		}
		
		else if((UserInput == BLUE_KEY4)&&(KeyPressedOkFlg == 1))
		{
			CheckToknFlg = 0;KeyPressedOkFlg = 0;
			NewToknValue = MoveToken(Blue[3],DiceResult);
			NToknNumber = 3;BlueUserState = USER_TOKEN_PROCESS;		
		}
    break;
		
	case USER_TOKEN_PROCESS:
		if(NewToknValue>0)
		{
			UserInput = 0;
		  TokenAnimator(NToknNumber,BLUE,NewToknValue);
	    BlueUserState = USER_TOKEN_ANIMATING;
		}
		else
		{
		 UserInput = 0;
		 CheckToknFlg = 1;
		 BlueUserState = USER_MOVE_TOKEN;	
		}
		break;
	
	case USER_TOKEN_ANIMATING:
		if(AnimationCmpltFlg)
		{
			if(DiceResult == 6)DiceRollChance +=1;
			if(ConflictFlg){ConflictFlg = 0;DiceRollChance +=1;}
			if(NewToknValue == 57)DiceRollChance +=1;
			BlueUserState = USER_RUN_NEXT_CHANCE;
		}
		break;
	case USER_RUN_NEXT_CHANCE:
		if(DiceRollChance>0)
		{
			if(!IsAllTokenMoved(Blue))
			{
				DiceRollChance--;
			  BlueUserState = ROLL_DICE;
			}
			else
			{
				GreenUserFlg = 1;
				SetArrowPointTo(GREEN);
				DiceRollChance = 0;
			  BlueUserState = USER_IDLE;
			}
		}
		else
		{
			GreenUserFlg = 1;
			SetArrowPointTo(GREEN);
			BlueUserState = USER_IDLE;
		}
		break;
	
  }
}






void GreenUser(void)
{
	switch(GreenUserState)
	{
		
	case USER_INIT:
		GreenUserFlg = 0;
		GreenUserState = USER_IDLE;
		break;
	
	case USER_IDLE:
		if(GreenUserFlg)
		{
			GreenUserFlg = 0;
			if(!IsAllTokenMoved(Green))
			GreenUserState = ROLL_DICE;
			else
			{YellowUserFlg = 1;SetArrowPointTo(YELLOW);}
		}
		break;
		
	case ROLL_DICE:
		if(UserInput == GREEN_KEY1)
		{
			UserInput = 0;
			RollTheDice();
			GreenUserState = GET_DICE;
		}
		break;
	
	case GET_DICE:
		if(ReadDiceValue()>0)
		{
			DiceResult = ReadDiceValue();
			GreenUserState = DICE_VALIDITY_CHECK;
		}
 		break;
		
	case DICE_VALIDITY_CHECK:
		if(IsDiceValueValid(DiceResult,Green))
		{
		  GreenUserState = USER_MOVE_TOKEN;CheckToknFlg = 1;
			if(IsOnlyTokenMovable(GREEN,DiceResult))
      {
			  GreenUserState = USER_TOKEN_PROCESS;CheckToknFlg = 0;
			}
		}
		else
		{GreenUserState = USER_IDLE;YellowUserFlg = 1;SetArrowPointTo(YELLOW);}
		break;
		
	case USER_MOVE_TOKEN:
		if((UserInput == GREEN_KEY1)&&(KeyPressedOkFlg == 1))
		{
			CheckToknFlg = 0;KeyPressedOkFlg = 0;
			NewToknValue = MoveToken(Green[0],DiceResult);
			NToknNumber = 0;GreenUserState = USER_TOKEN_PROCESS;
			
		}
		
		else if((UserInput == GREEN_KEY2)&&(KeyPressedOkFlg == 1))
		{
			CheckToknFlg = 0;KeyPressedOkFlg = 0;
			NewToknValue = MoveToken(Green[1],DiceResult);
			NToknNumber = 1;GreenUserState = USER_TOKEN_PROCESS;		
		}
		
		else if((UserInput == GREEN_KEY3)&&(KeyPressedOkFlg == 1))
		{
			CheckToknFlg = 0;KeyPressedOkFlg = 0;
			NewToknValue = MoveToken(Green[2],DiceResult);
			NToknNumber = 2;GreenUserState = USER_TOKEN_PROCESS;
			
		}
		
		else if((UserInput == GREEN_KEY4)&&(KeyPressedOkFlg == 1))
		{
			CheckToknFlg = 0;KeyPressedOkFlg = 0;
			NewToknValue = MoveToken(Green[3],DiceResult);
			NToknNumber = 3;GreenUserState = USER_TOKEN_PROCESS;		
		}
    break;
		
	case USER_TOKEN_PROCESS:
		if(NewToknValue>0)
		{
			UserInput = 0;
		  TokenAnimator(NToknNumber,GREEN,NewToknValue);
	    GreenUserState = USER_TOKEN_ANIMATING;
		}
		else
		{
		 CheckToknFlg = 1;
		 UserInput = 0;
		 GreenUserState = USER_MOVE_TOKEN;	
		}
		break;
	
	case USER_TOKEN_ANIMATING:
		if(AnimationCmpltFlg)
		{
			if(DiceResult == 6)DiceRollChance +=1;
			if(ConflictFlg){ConflictFlg = 0;DiceRollChance +=1;}
			if(NewToknValue == 57)DiceRollChance +=1;
			GreenUserState = USER_RUN_NEXT_CHANCE;
		}
		break;
		
	case USER_RUN_NEXT_CHANCE:
		if(DiceRollChance>0)
		{
			if(!IsAllTokenMoved(Green))
			{
				DiceRollChance--;
			  GreenUserState = ROLL_DICE;
			}
			else
			{
				YellowUserFlg = 1;SetArrowPointTo(YELLOW);
				DiceRollChance = 0;
			  GreenUserState = USER_IDLE;
			}
		}
		else
		{
			YellowUserFlg = 1;
			SetArrowPointTo(YELLOW);
			GreenUserState = USER_IDLE;
		}
		break;
	
  }
}





void YellowUser(void)
{
	switch(YellowUserState)
	{
		
	case USER_INIT:
		YellowUserFlg = 0;
		YellowUserState = USER_IDLE;
		break;
	
	case USER_IDLE:
		if(YellowUserFlg)
		{
			YellowUserFlg = 0;
			if(!IsAllTokenMoved(Yellow))
			YellowUserState = ROLL_DICE;
			else
			{RedUserFlg = 1;SetArrowPointTo(RED);}
		}
		break;
		
	case ROLL_DICE:
		if(UserInput == YELLOW_KEY4)
		{
			UserInput = 0;
			RollTheDice();
			YellowUserState = GET_DICE;
		}
		break;
	
	case GET_DICE:
		if(ReadDiceValue()>0)
		{
			DiceResult = ReadDiceValue();
			YellowUserState = DICE_VALIDITY_CHECK;
		}
 		break;
		
	case DICE_VALIDITY_CHECK:
		if(IsDiceValueValid(DiceResult,Yellow))
		{
		  YellowUserState = USER_MOVE_TOKEN;CheckToknFlg = 1;
			if(IsOnlyTokenMovable(YELLOW,DiceResult))
      {
			  YellowUserState = USER_TOKEN_PROCESS;CheckToknFlg = 0;
			}
		}
		else
		{YellowUserState = USER_IDLE;RedUserFlg = 1;SetArrowPointTo(RED);}
		break;
		
	case USER_MOVE_TOKEN:
		if((UserInput == YELLOW_KEY1)&&(KeyPressedOkFlg == 1))
		{
			CheckToknFlg = 0;
			NewToknValue = MoveToken(Yellow[0],DiceResult);
			NToknNumber = 0;YellowUserState = USER_TOKEN_PROCESS;
			
		}
		
		else if((UserInput == YELLOW_KEY2)&&(KeyPressedOkFlg == 1))
		{
			CheckToknFlg = 0;
			NewToknValue = MoveToken(Yellow[1],DiceResult);
			NToknNumber = 1;YellowUserState = USER_TOKEN_PROCESS;		
		}
		
		else if((UserInput == YELLOW_KEY3)&&(KeyPressedOkFlg == 1))
		{
			CheckToknFlg = 0;
			NewToknValue = MoveToken(Yellow[2],DiceResult);
			NToknNumber = 2;YellowUserState = USER_TOKEN_PROCESS;
			
		}
		
		else if((UserInput == YELLOW_KEY4)&&(KeyPressedOkFlg == 1))
		{
			CheckToknFlg = 0;
			NewToknValue = MoveToken(Yellow[3],DiceResult);
			NToknNumber = 3;YellowUserState = USER_TOKEN_PROCESS;		
		}
    break;
		
	case USER_TOKEN_PROCESS:
		if(NewToknValue>0)
		{
			UserInput = 0;
		  TokenAnimator(NToknNumber,YELLOW,NewToknValue);
	    YellowUserState = USER_TOKEN_ANIMATING;
		}
		else
		{
		 CheckToknFlg = 1;
		 UserInput = 0;
		 YellowUserState = USER_MOVE_TOKEN;	
		}
		break;
	
	case USER_TOKEN_ANIMATING:
		if(AnimationCmpltFlg)
		{
			if(DiceResult == 6)DiceRollChance +=1;
			if(ConflictFlg){ConflictFlg = 0;DiceRollChance +=1;}
			if(NewToknValue == 57)DiceRollChance +=1;
			YellowUserState = USER_RUN_NEXT_CHANCE;
		}
		break;
		
	case USER_RUN_NEXT_CHANCE:
		if(DiceRollChance>0)
		{
			if(!IsAllTokenMoved(Yellow))
			{
				DiceRollChance--;
			  YellowUserState = ROLL_DICE;
			}
			else
			{
				RedUserFlg = 1;SetArrowPointTo(RED);
				DiceRollChance = 0;
			  YellowUserState = USER_IDLE;
			}
		}
		else
		{
			RedUserFlg = 1;
			SetArrowPointTo(RED);
			YellowUserState = USER_IDLE;
		}
		break;
	
  }
}



bit IsDiceValueValid(uint8_t DiceValue,uint8_t UserBuff[4])
{
	if(DiceValue == 6)
	{
		if(((UserBuff[0]+6)<58)||((UserBuff[1]+6)<58)||((UserBuff[2]+6)<58)||((UserBuff[3]+6)<58))
		{return 1;}
		else
			return 0;
			
	}	

   else
	 {
		 if((UserBuff[0]==0)&&(UserBuff[1]==0)&&(UserBuff[2]==0)&&(UserBuff[3]==0))
		 { return 0;}
		 else if(((UserBuff[0] >0)&&((UserBuff[0]+DiceValue)<58))||
			       ((UserBuff[1] >0)&&((UserBuff[1]+DiceValue)<58))||
		         ((UserBuff[2] >0)&&((UserBuff[2]+DiceValue)<58))||
		         ((UserBuff[3] >0)&&((UserBuff[3]+DiceValue)<58))
		         )
		 {return 1;}
		 else 
			 return 0;
		 
	 }		 
}

bit IsAllTokenMoved(uint8_t Token[4])
{
	if((Token[0]>56)&&(Token[1]>56)&&(Token[2]>56)&&(Token[3]>56))
		return 1;
	else
		return 0;
}

uint8_t MoveToken(uint8_t OldTokenNumber,uint8_t DiceVal)
{
	if((OldTokenNumber == 0)&&(DiceVal == 6))
	{return 1;}
	else if((OldTokenNumber > 0)&&(OldTokenNumber+DiceVal < 58))
	{return OldTokenNumber+DiceVal;}
	else
	{return 0;}
}



bit IsOnlyTokenMovable(uint8_t ToknClr,uint8_t DiceValue)
{
	uint8_t idata TokenVal[4];
  uint8_t idata Counter=0,i=0;
	switch(ToknClr)
	{
		case RED:TokenVal[0] = Red[0];TokenVal[1] = Red[1];TokenVal[2] = Red[2];TokenVal[3] = Red[3];break;
		case BLUE:TokenVal[0] = Blue[0];TokenVal[1] = Blue[1];TokenVal[2] = Blue[2];TokenVal[3] = Blue[3];break;
		case GREEN:TokenVal[0] = Green[0];TokenVal[1] = Green[1];TokenVal[2] = Green[2];TokenVal[3] = Green[3];break;
		case YELLOW:TokenVal[0] = Yellow[0];TokenVal[1] = Yellow[1];TokenVal[2] = Yellow[2];TokenVal[3] = Yellow[3];break;
	}
	if(DiceValue == 6)
	{
		if((TokenVal[0]==0)&&(TokenVal[1]==0)&&(TokenVal[2]==0)&&(TokenVal[3]==0))
		{NewToknValue = 1;NToknNumber = 0;return 1;}
		
		else 
		{
			for(i=0;i<4;i++){if((TokenVal[i] == 0)||((TokenVal[i]>0)&&((TokenVal[i] + 6)<58))){Counter +=1;NToknNumber = i;if(Counter>1)return 0;}}
			if(Counter == 1){if(TokenVal[NToknNumber] == 0){NewToknValue = 1;return 1;}else{NewToknValue = TokenVal[NToknNumber]+6;return 1;}}	
		}
  }
		
	else
	{
		for(i=0;i<4;i++){if((TokenVal[i]>0)&&((TokenVal[i] + DiceValue)<58)){Counter +=1;NToknNumber = i;if(Counter>1)return 0;}}
		if(Counter == 1){NewToknValue = TokenVal[NToknNumber]+DiceValue;return 1;}
	}
	
	return 0;
}












void ManageAnimation(void)
{
	switch(AnimtnState)
	{
		case ANMTN_INIT:
			AnimtnState = ANMTN_IDLE;
			break;
		
		case ANMTN_IDLE:
			if(AnimateCmd)
			{
				AnimationCmpltFlg = 0;
				AnimateCmd = 0;
				AnmtnBusy = 1;
				AnimationTimer = 0;
				ConflictFlg = 0;
				conflictTknClr = 0;
				conflictTknNmbr = 5;
				AnimtnState = ANIMATE_FORWARD1;
			}
			break;
			
		case ANIMATE_FORWARD1:
			if(AnimationTimer == 0)
			{
				switch(AnmtngTokenColour)
				{
					case RED:
						if(Red[AnmtngTokenNumber]< AnmtngTokenValue)
						{Red[AnmtngTokenNumber]++;}
						break;
					case BLUE:
						if(Blue[AnmtngTokenNumber]<AnmtngTokenValue)
						{Blue[AnmtngTokenNumber]++;}
						break;
					case GREEN:
						if(Green[AnmtngTokenNumber]<AnmtngTokenValue)
						{Green[AnmtngTokenNumber]++;}
						break;
					case YELLOW:
						if(Yellow[AnmtngTokenNumber]<AnmtngTokenValue)
						{Yellow[AnmtngTokenNumber]++;}
						break;
				}
				
        StartBeep();
				AnimationTimer = ANIMATE_FORWARD_TIME;
				AnimtnState = ANIMATE_FORWARD2;
				
			}
			break;
			
		case ANIMATE_FORWARD2:
			if(AnimationTimer == 0)
			{
				StopBeep();
				switch(AnmtngTokenColour)
				{
					case RED:
						if(Red[AnmtngTokenNumber] == AnmtngTokenValue)
						{AnimationCmpltFlg = 1;}
						break;
					case BLUE:
						if(Blue[AnmtngTokenNumber] == AnmtngTokenValue)
						{AnimationCmpltFlg = 1;}
						break;
					case GREEN:
						if(Green[AnmtngTokenNumber] == AnmtngTokenValue)
						{AnimationCmpltFlg = 1;}
						break;
					case YELLOW:
						if(Yellow[AnmtngTokenNumber] == AnmtngTokenValue)
						{AnimationCmpltFlg = 1;}
						break;
				}
				
				
				if(AnimationCmpltFlg)
				{
					AnimationCmpltFlg = 0;
					AnimationTimer = 20;//within 20 milli second UserDriverBridge  can decode 
					AnimtnState = ANMTN_CHECK_CONFLICT;
				}
				else
				{
				AnimationTimer = ANIMATE_FORWARD_TIME;
				AnimtnState = ANIMATE_FORWARD1;
				}
				
			}
			break;
			
		case ANMTN_CHECK_CONFLICT:
			if(AnimationTimer == 0)
			{
				CheckAndGetConflictId(AnmtngTokenColour,AnmtngTokenNumber);
				if(conflictTknClr > 0)
				{

					  AnimationTimer = 0;
				    AnimtnState = ANIMATE_BACKWARD1;
				}
				else 
				{
					AnimationCmpltFlg = 1;
					ConflictFlg = 0;
					AnmtnBusy = 0;
				  AnimtnState = ANMTN_IDLE;
				}
			}
			break;
			
		case ANIMATE_BACKWARD1:
			if(AnimationTimer == 0)
			{
				StartBeep();
				switch(conflictTknClr)
				{
					case RED:
						Red[conflictTknNmbr]--;
						break;
					case BLUE:
						Blue[conflictTknNmbr]--;
						break;
					case GREEN:
						Green[conflictTknNmbr]--;
						break;
					case YELLOW:
						Yellow[conflictTknNmbr]--;
						break;
				}
				AnimationTimer = ANIMATE_BACKWARD_TIME;
				AnimtnState = ANIMATE_BACKWARD2;
			}
			break;
			
		case ANIMATE_BACKWARD2:
			if(AnimationTimer == 0)
			{
				StopBeep();
				switch(conflictTknClr)
				{
					case RED:
						if(Red[conflictTknNmbr] == 0)
							AnimationCmpltFlg = 1;
						break;
					case BLUE:
						if(Blue[conflictTknNmbr] == 0)
							AnimationCmpltFlg = 1;
						break;
					case GREEN:
						if(Green[conflictTknNmbr] == 0)
							AnimationCmpltFlg = 1;
						break;
					case YELLOW:
						if(Yellow[conflictTknNmbr] == 0)
							AnimationCmpltFlg = 1;
						break;
				}
				
				if(AnimationCmpltFlg)
				{
					AnimationCmpltFlg = 1;
					ConflictFlg = 1;
					AnmtnBusy = 0;
				  AnimtnState = ANMTN_IDLE;
					
				}
				else
				{
				  AnimationTimer = ANIMATE_BACKWARD_TIME;
				  AnimtnState = ANIMATE_BACKWARD1;
				}
			}
			break;

	}
}



void CheckAndGetConflictId(uint8_t srcColour,uint8_t srcTknNumber)
{
	uint8_t i = 0;
	uint8_t LedPos = 0;
	switch(srcColour)
	{
		case RED:
			for(i= 0;i<4;i++)
      {
        if(DcodedRed[srcTknNumber] == DcodedBlue[i]){conflictTknClr = BLUE;conflictTknNmbr = i;LedPos = DcodedBlue[i];}
				else if(DcodedRed[srcTknNumber] == DcodedGreen[i]){conflictTknClr = GREEN;conflictTknNmbr = i;LedPos = DcodedGreen[i];}
				else if(DcodedRed[srcTknNumber] == DcodedYellow[i]){conflictTknClr = YELLOW;conflictTknNmbr = i;LedPos = DcodedYellow[i];}
			}
			break;
			
		case BLUE:
			for(i= 0;i<4;i++)
      {
        if(DcodedBlue[srcTknNumber] == DcodedRed[i]){conflictTknClr = RED;conflictTknNmbr = i;LedPos = DcodedRed[i];}
				else if(DcodedBlue[srcTknNumber] == DcodedGreen[i]){conflictTknClr = GREEN;conflictTknNmbr = i;LedPos = DcodedGreen[i];}
				else if(DcodedBlue[srcTknNumber] == DcodedYellow[i]){conflictTknClr = YELLOW;conflictTknNmbr = i;LedPos = DcodedYellow[i];}
			}
			break;
			
		case GREEN:
			for(i= 0;i<4;i++)
      {
        if(DcodedGreen[srcTknNumber] == DcodedBlue[i]){conflictTknClr = BLUE;conflictTknNmbr = i;LedPos = DcodedBlue[i];}
				else if(DcodedGreen[srcTknNumber] == DcodedRed[i]){conflictTknClr = RED;conflictTknNmbr = i;LedPos = DcodedRed[i];}
				else if(DcodedGreen[srcTknNumber] == DcodedYellow[i]){conflictTknClr = YELLOW;conflictTknNmbr = i;LedPos = DcodedYellow[i];}
			}
			break;
			
		case YELLOW:
			for(i= 0;i<4;i++)
      {
        if(DcodedYellow[srcTknNumber] == DcodedBlue[i]){conflictTknClr = BLUE;conflictTknNmbr = i;LedPos = DcodedBlue[i];}
				else if(DcodedYellow[srcTknNumber] == DcodedGreen[i]){conflictTknClr = GREEN;conflictTknNmbr = i;LedPos = DcodedGreen[i];}
				else if(DcodedYellow[srcTknNumber] == DcodedRed[i]){conflictTknClr = RED;conflictTknNmbr = i;LedPos = DcodedRed[i];}
			}
			break;	
	}
	if(IsToknOnBaithki(LedPos)>0)
	{
		conflictTknNmbr = 0;
		conflictTknClr = 0;	
	}
}


uint8_t IsToknOnBaithki(uint8_t LedNumber)
{
	uint8_t returnval = 0;
	if(LedNumber>0)
	{
	   switch(LedNumber)
		 {
			 case 1:returnval = 1;break;
			 case 9:returnval = 9;break;
			 case 14:returnval = 14;break;
			 case 22:returnval = 22;break;
			 case 27:returnval = 27;break;
			 case 35:returnval = 35;break;
			 case 40:returnval = 40;break;
			 case 48:returnval = 48;break;		 
		 }
	}
	return returnval;
}


void TokenAnimator(uint8_t TknNmber,uint8_t TknClr,uint8_t NewToknValue)
{
	AnimateCmd = 1;
	AnmtngTokenColour = TknClr;
  AnmtngTokenNumber = TknNmber;
  AnmtngTokenValue = NewToknValue;
	
}



void AnmtnTimer(void)
{
	if(AnimationTimer>0)
		AnimationTimer--;

}