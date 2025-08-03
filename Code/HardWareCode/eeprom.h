#ifndef __EEPROM_H_
#define __EEPROM_H_


unsigned char EEPROM_Read_Byte(unsigned int code *address);
void EEPROM_Write_nByte(unsigned int u16EPAddr,unsigned char *u8EPData,unsigned char sizeofdata);

void IAP_ErasePage(unsigned char ByteAddr);
void IAP_WriteByte(unsigned int ByteAddr,unsigned char ByteData);
#endif
