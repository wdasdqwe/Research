#ifndef _LTC2668_H_
#define _LTC2668_H_

#include <stdint.h>

void LTC2668_Init();
void LTC2668_WriteChannel(uint8_t channel, uint16_t code);


void LTC2668_Write(uint8_t addr, uint16_t data);

#endif
