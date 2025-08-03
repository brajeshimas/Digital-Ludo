#ifndef _BUZZER_H_
#define _BUZZER_H_



#define BZR_PIN  P30
//State Names**********************
#define BZR_INIT             0
#define BZR_IDLE             1
#define BZR_LD_TIMER         2
#define BZR_ON_WAIT          3
#define BZR_OFF_WAIT         4
#define BZR_ONE_BEEP         5
//********************************


//******BEEP_INDICATION_TYPE********
#define SHORT_BEEP               0
#define LONG_BEEP                1
#define ULTRA_LONG_BEEP          2
//**********************************


//Convenient COnstants**************
#define ON_TIME             1
#define OFF_TIME            0

#define BZR_ON        1
#define BZR_OFF       0  
//**********************************



//SET BEEP ON TIME AND OFF TIME**************
#define SHORT_BEEP_ON_TIME               200        
#define SHORT_BEEP_OFF_TIME              400
#define LONG_BEEP_ON_TIME                400
#define LONG_BEEP_OFF_TIME               800
#define ULTRA_LONG_BEEP_ON_TIME          800
#define ULTRA_LONG_BEEP_OFF_TIME         1600
//*******************************************




extern bit BuzzerBeepcmd;
extern volatile int BuzzerTimer;

void SetBeepIndication(unsigned char beepType ,unsigned char Beepcount);
void BuzzerManager(void);
void BuzzerTimerReload(unsigned char beepType,bit onOroffTime);
void BeepTimer(void);
void StopBeep(void);
void StartBeep(void);
#endif