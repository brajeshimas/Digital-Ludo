#ifndef _USERDRIVERBRIDGE_H_
#define _USERDRIVERBRIDGE_H_

#define R1       52
#define R2       53
#define R3       54
#define R4       55
#define R5       56
      
#define B1       57
#define B2       58
#define B3       59
#define B4       60
#define B5       61
      
#define G1       62
#define G2       63
#define G3       64
#define G4       65
#define G5       66
      
#define Y1       67
#define Y2       68
#define Y3       69
#define Y4       70
#define Y5       71

#define RH1      72
#define RH2      73
#define RH3      74
#define RH4      75
        
#define GH1      76
#define GH2      77
#define GH3      78
#define GH4      79
      
#define BH1      80
#define BH2      81
#define BH3      82
#define BH4      83
        
#define YH1      84
#define YH2      85
#define YH3      86
#define YH4      87

#define TOKEN_OUT_R1   88
#define TOKEN_OUT_R2   89
#define TOKEN_OUT_R3   90
#define TOKEN_OUT_R4   91

#define TOKEN_OUT_B1   92
#define TOKEN_OUT_B2   93
#define TOKEN_OUT_B3   94
#define TOKEN_OUT_B4   95

#define TOKEN_OUT_G1   96
#define TOKEN_OUT_G2   97
#define TOKEN_OUT_G3   98
#define TOKEN_OUT_G4   99

#define TOKEN_OUT_Y1   100
#define TOKEN_OUT_Y2   101
#define TOKEN_OUT_Y3   102
#define TOKEN_OUT_Y4   103


extern uint8_t DcodedRed[4];
extern uint8_t DcodedBlue[4];
extern uint8_t DcodedGreen[4];
extern uint8_t DcodedYellow[4];

void DecodeManager(void);
void DcodeToknToLedNumber(uint8_t TokenNumber,uint8_t TokenValue,uint8_t ToknColour);
#endif