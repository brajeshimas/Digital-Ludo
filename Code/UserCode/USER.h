#ifndef _USER_H
#define _USER_H




extern volatile uint8_t idata Red[4];
extern volatile uint8_t idata Blue[4];
extern volatile uint8_t idata Green[4];
extern volatile uint8_t idata Yellow[4];

//extern bit CheckToknFlg;
void RedUser(void);
void GreenUser(void);
void BlueUser(void);
void YellowUser(void);
void ManageAnimation(void);
void AnmtnTimer(void);
void TokenAnimator(uint8_t TknNmber,uint8_t TknClr,uint8_t NewToknValue);
#endif