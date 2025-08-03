

#ifndef _USERINPUT_H_
#define _USERINPUT_H_

#define RED_KEY1                  13*1
#define RED_KEY2                  13*2
#define RED_KEY3                  13*3
#define RED_KEY4                  13*4

#define BLUE_KEY1									14*1
#define BLUE_KEY2									14*2
#define BLUE_KEY3									14*3
#define BLUE_KEY4									14*4

#define GREEN_KEY1								15*1
#define GREEN_KEY2								15*2
#define GREEN_KEY3								15*3
#define GREEN_KEY4								15*4

#define YELLOW_KEY1								16*1
#define YELLOW_KEY2								16*2
#define YELLOW_KEY3								16*3
#define YELLOW_KEY4								16*4


extern uint8_t KeyPressedRowValue;
extern uint8_t KeyColmValue;
extern uint8_t LongPressedCounter;
extern bit KeyPressedFlg;
extern uint8_t UserInput;
extern bit CheckToknFlg;
extern bit KeyPressedOkFlg;
void InputScanTimer(void);
void ScanInput(void);
uint8_t GetKeyPressed(void);
void ToknBlinkTimer(void);
void CheckKeyVsTokn(void);
#endif