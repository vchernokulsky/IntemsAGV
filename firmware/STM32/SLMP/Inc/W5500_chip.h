#pragma once

#include "wizchip_conf.h"
#include "stm32f4xx_hal.h"

#define W5500_CS_Pin GPIO_PIN_2
#define W5500_CS_GPIO_Port GPIOB

#define W5500_RST_Pin GPIO_PIN_6
#define W5500_RST_GPIO_Port GPIOB

class W5500Chip {
public:
	static SPI_HandleTypeDef *hspi;
public:
	W5500Chip();
	void initChip(void);
	~W5500Chip();
private:
	static void W5500_Select(void);
	static void W5500_Unselect(void);
	static void W5500_ReadBuff(uint8_t* buff, uint16_t len);
	static void W5500_WriteBuff(uint8_t* buff, uint16_t len);
	static uint8_t W5500_ReadByte(void);
	static void W5500_WriteByte(uint8_t byte);
};
