#include "ltc2668.h"

#include "spi.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_spi.h"
#include <stdint.h>

#define CS_PIN GPIO_PIN_12
#define CS_PORT GPIOB

void LTC2668_Init() {
  HAL_SPI_Init(&hspi2);
  //Write code to all, update all (power up)
  LTC2668_Write(0x0a, 0x0, 0x0); 
}

void LTC2668_WriteChannel(uint8_t channel, uint16_t code) {
  //Write code to n, update n (power up)
  LTC2668_Write(0x03, channel, code);
}

void LTC2668_Write(uint8_t cmd, uint8_t addr, uint16_t data)
{
    // 构造24bit数据帧（适用于LTC2668-16）
    uint8_t txData[3] = {
        (cmd << 4) | (addr & 0x0F),  // 高字节：命令+地址
        (data >> 8) & 0xFF,          // 数据高字节
        data & 0xFF                  // 数据低字节
    };
    
    // 如果是LTC2668-12需要左移4位：
    // uint16_t shifted_data = data << 4;
    
    // 手动拉低CS
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
    
    // 传输24bit数据（3个字节）
    HAL_SPI_Transmit(&hspi2, txData, 3, HAL_MAX_DELAY);
    
    // 拉高CS完成传输
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

void LTC2668_Write32Bit(uint8_t cmd, uint8_t addr, uint16_t data)
{
    // 构造32bit数据帧（8bit填充 + 24bit有效数据）
    uint8_t txData[4] = {
        0x00,                    // 前导填充字节（8 don't care bits）
        (cmd << 4) | (addr & 0x0F),  // 命令+地址
        (data >> 8) & 0xFF,       // 数据高字节
        data & 0xFF               // 数据低字节
    };
    
    // 手动控制CS引脚
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
    
    // 传输32bit数据（4个字节）
    HAL_SPI_Transmit(&hspi2, txData, 4, HAL_MAX_DELAY);
    
    // CS引脚置高
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}
