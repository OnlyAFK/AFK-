#ifndef __BLUEBOOTH_H_
#define __BLUEBOOTH_H_

#include "sys.h"
void Bluetooth_USART_Init(void);
void Bluetooth_Init(void);
void Bluetooth_USART_SendByte(uint8_t Data);
void Bluetooth_USART_SendArray(uint16_t *Array,uint16_t Length);
void Bluetooth_USART_SendString(char *String);
uint32_t Bluetooth_Pow(uint32_t X,uint32_t Y);
void Bluetooth_SendNumber(int32_t Number,uint8_t Length);
uint8_t Bluetooth_GetRxFlag(void);
uint8_t Bluetooth_GetRxData(void);

void Bluooth_Detecting(void);


#endif
