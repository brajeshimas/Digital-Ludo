#ifndef __KEYPAD_H__
#define __KEYPAD_H__


//HARDWARE KEYS
#define KEY1     1
#define KEY2     2
#define KEY3     3
#define KEY4     4
#define KEY5     5
#define KEY6     6
#define KEY7     7

 
//USER Keys
#define NULL_KEY											0
#define ISWITCH_KEY										1
#define ISWITCH_KEY_05SEC_LONG_PRESS	2
#define ISWITCH_KEY_10SEC_LONG_PRESS	3
#define ISWITCH_KEY_15SEC_LONG_PRESS	4
#define ISWITCH_KEY_20SEC_LONG_PRESS	5
#define ISWITCH_KEY_25SEC_LONG_PRESS	6
#define ISWITCH_KEY_30SEC_LONG_PRESS	7

#define ISWITCH_KEY_LONG_PRESSED			10		


//Keypad Timing Controls
#define KEY_REPEAT_TIME				3	//3*1 sec=1sec
#define KEY_DEBOUNCE_TIME			2	//2*50msec=100msec

//Keypad State Machine - Variables
#define KEYPRESSEDFORLESSTHAN1SEC		0
#define KEYPRESSEDFOR1SEC						1
#define KEYPRESSEDFOR2SEC						2
#define KEYPRESSEDFOR3SEC						3
#define KEYPRESSEDFOR4SEC						4
#define KEYPRESSEDFOR5SEC						5
#define KEYPRESSEDFOR6SEC						6
#define KEYPRESSEDFOR7SEC						7
#define KEYPRESSEDFOR8SEC						8
#define KEYPRESSEDFOR9SEC						9
#define KEYPRESSEDFOR10SEC					10
#define KEYPRESSEDFOR11SEC					11
#define KEYPRESSEDFOR12SEC					12
#define KEYPRESSEDFOR13SEC					13
#define KEYPRESSEDFOR14SEC					14
#define KEYPRESSEDFOR15SEC					15
#define KEYPRESSEDFOR16SEC					16
#define KEYPRESSEDFOR17SEC					17
#define KEYPRESSEDFOR18SEC					18
#define KEYPRESSEDFOR19SEC					19
#define KEYPRESSEDFOR20SEC					20
#define KEYPRESSEDFOR21SEC					21
#define KEYPRESSEDFOR22SEC					22
#define KEYPRESSEDFOR23SEC					23
#define KEYPRESSEDFOR24SEC					24



//Keypad State Machine - States
#define KEYPAD_STATE_INIT							0
#define KEYPAD_STATE_IDLE							1
#define KEYPAD_STATE_KEYDEBOUNCE			2
#define KEYPAD_STATE_WAITFORRELEASE		3

extern volatile unsigned char 	KeyPressTimer;
extern volatile signed char   	KeyPressCounter;
extern volatile signed char		LongKeyPressTimer;
extern volatile unsigned char KeyValue;

void KeypadScan(void);
char GetKeyPressed(void);
void KeypadPortINIT(void);

#endif