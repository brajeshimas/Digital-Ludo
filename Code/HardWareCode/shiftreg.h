
#ifndef _SHIFTREG_H
#define _SHIFTREG_H

#define MSBFIRST    1
#define LSBFIRST    0

void shiftOut(bit DataPin , bit ClkPin,bit BitOrder,uint8_t Data );
#endif