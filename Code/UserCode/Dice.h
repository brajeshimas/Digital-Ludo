#ifndef _DICE_H_
#define _DICE_H_


#include "shiftreg.h"
#define BITORDER  MSBFIRST

#if (BITORDER==MSBFIRST)
#define A       BIT7
#define B       BIT6
#define C       BIT1
#define D       BIT2
#define E       BIT3
#define F       BIT4
#define G       BIT5
#define DOT     BIT0
#endif

#if (BITORDER==LSBFIRST)
#define A       BIT0
#define B       BIT1
#define C       BIT6
#define D       BIT5
#define E       BIT4
#define F       BIT3
#define G       BIT2
#define DOT     BIT7 
#endif



#define ZERO    A|B|C|D|E|G
#define ONE     B|C
#define TWO     A|B|F|E|D
#define THREE   A|B|C|F|D
#define FOUR    G|B|F|C
#define FIVE    A|G|F|C|D
#define SIX     A|G|F|C|D|E
#define SEVEN   A|B|C
#define EIGHT   A|B|C|D|E|F|G
#define NINE    A|B|C|D|F|G
#define CLEAR_DICE 10

#define DICE_LATCH_PIN  P31

#define THROUGH_SPI   1  //if true means shiftReg will be run through SPI and CS pin
void DiceManager(void);
void RollTheDice(void);
uint8_t ReadDiceValue(void);
void DiceTimer(void);
void DsplyOnSegment(uint8_t Number, bit DotPrintTrueFalse);
void SetArrowPointTo(uint8_t PlyrClr);
#endif