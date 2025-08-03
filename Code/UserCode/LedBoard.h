#ifndef _LEDBOARD_H_
#define _LEDBOARD_H_

#define LED_LTCH_PIN       P17

#define RED      82   //82 ASCII CODE OF 'R'
#define BLUE     66   //66 ASCII CODE OF 'B'
#define GREEN    71   //71 ASCII CODE OF 'G'
#define YELLOW   89   //89 ASCII CODE OF 'Y'



//---------LED_MANAGER_STATES-------------
#define LED_INIT              0
#define LED_IDLE              1
#define LED_SET_ROW_COLM      2  
#define LED_UPDATE            3
//----------------------------------------


extern uint8_t ActiveLedColm;
void LedTimer(void);
void LedManager(void);
#endif