#include "ltc2668.h"

#include "main.h"
#include <stdint.h>

/* #define CSB_Pin GPIO_PIN_12 */
/* #define CSB_GPIO_Port GPIOB */

/* #define SCK_Pin GPIO_PIN_13 */
/* #define SCK_GPIO_Port GPIOB */

/* #define MISO_Pin GPIO_PIN_14 */
/* #define MISO_GPIO_Port GPIOB */

/* #define MOSI_Pin GPIO_PIN_15 */
/* #define MOSI_GPIO_Port GPIOB */

// Define a macro to set a GPIO pin to low
#define LOW(x) HAL_GPIO_WritePin(x##_GPIO_Port, x##_Pin, GPIO_PIN_RESET)

// Define a macro to set a GPIO pin to high
#define HIGH(x) HAL_GPIO_WritePin(x##_GPIO_Port, x##_Pin, GPIO_PIN_SET)

void SPI_Delay() {
  for (int i = 0; i < 1000; i++) {
  }
}

void LTC2668_Init() {
  HIGH(CSB);
  // Write code to all, update all (power up)
  LTC2668_Write(0xa0, 0x7fff); // Set to middle scale
}

void LTC2668_WriteChannel(uint8_t channel, uint16_t code) {
  // Write code to n, update n (power up)
  LTC2668_Write((0x30 | channel), code);
}

void LTC2668_SetVoltage(uint8_t channel, float voltage) {
  // Clamp the voltage
  if (voltage > 9.9)voltage = 9.9;
  if (voltage < -9.9)voltage = -9.9;
  uint16_t code = (voltage + 10.0)/20.0 * 65535.0;
  // Write code to n, update n (power up)
  LTC2668_Write((0x30 | channel), code);
}

//Software SPI
void LTC2668_Write(uint8_t addr, uint16_t data) {
  LOW(CSB);

  for (int i = 0; i < 8; i++) {
    LOW(SCK);
    SPI_Delay();
    if (addr & 0x80) {
      HIGH(MOSI);
    } else {
      LOW(MOSI);
    }
    SPI_Delay();
    addr <<= 1;
    HIGH(SCK);
    SPI_Delay();
  }
  for (int i = 0; i < 16; i++) {
    LOW(SCK);
    SPI_Delay();
    if (data & 0x8000) {
      HIGH(MOSI);
    } else {
      LOW(MOSI);
    }
    SPI_Delay();
    data <<= 1;
    HIGH(SCK);
    SPI_Delay();
  }

  HIGH(CSB);
}

/* void LTC2668_Write(uint8_t cmd, uint8_t addr, uint16_t data) */
/* { */
/*     // 构造24bit数据帧（适用于LTC2668-16） */
/*     uint8_t txData[3] = { */
/*         (cmd << 4) | (addr & 0x0F),  // 高字节：命令+地址 */
/*         (data >> 8) & 0xFF,          // 数据高字节 */
/*         data & 0xFF                  // 数据低字节 */
/*     }; */

/*     // 手动拉低CS */
/*     HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET); */

/*     // 传输24bit数据（3个字节） */
/*     HAL_SPI_Transmit(&hspi2, txData, 3, HAL_MAX_DELAY); */

/*     HAL_Delay(1); */
/*     // 拉高CS完成传输 */
/*     HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET); */
/* } */

/* void LTC2668_Write32Bit(uint8_t cmd, uint8_t addr, uint16_t data) */
/* { */
/*     // 构造32bit数据帧（8bit填充 + 24bit有效数据） */
/*     uint8_t txData[4] = { */
/*         0x00,                    // 前导填充字节（8 don't care bits） */
/*         (cmd << 4) | (addr & 0x0F),  // 命令+地址 */
/*         (data >> 8) & 0xFF,       // 数据高字节 */
/*         data & 0xFF               // 数据低字节 */
/*     }; */

/*     // 手动控制CS引脚 */
/*     HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET); */

/*     // 传输32bit数据（4个字节） */
/*     HAL_SPI_Transmit(&hspi2, txData, 4, HAL_MAX_DELAY); */

/*     // CS引脚置高 */
/*     HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET); */
/* } */
