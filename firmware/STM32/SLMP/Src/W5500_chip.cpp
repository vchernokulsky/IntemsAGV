#include "W5500_chip.h"

W5500Chip::W5500Chip() {}

void W5500Chip::initChip(void) {
    reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);
    reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);
    reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);
    uint8_t rx_tx_buff_sizes[] = {2, 2, 2, 2};
    wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);
    wiz_NetInfo net_info = {
    	.mac = {0x00, 0x08, 0xdc, 0xab, 0x1d, 0xe8},
        .ip = {192, 168, 55, 114},
        .sn = {255, 255, 255, 0},
        .gw = {192, 168,55, 1}};
    wizchip_setnetinfo(&net_info);
    wizchip_getnetinfo(&net_info);
}

void W5500Chip::W5500_Select(void) {
    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET);
}

void W5500Chip::W5500_Unselect(void) {
    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_SET);
}

void W5500Chip::W5500_ReadBuff(uint8_t* buff, uint16_t len) {
	uint32_t begin = HAL_GetTick();
	HAL_SPI_Receive(hspi, buff, len, 100);
	uint32_t end = HAL_GetTick();
	if(end - begin > 110) {
		HAL_Delay(50);
	}
}

void W5500Chip::W5500_WriteBuff(uint8_t* buff, uint16_t len) {
	uint32_t begin = HAL_GetTick();
	HAL_SPI_Transmit(hspi, buff, len, 100);
	uint32_t end = HAL_GetTick();
	if(end - begin > 110) {
		HAL_Delay(50);
	}
}

uint8_t W5500Chip::W5500_ReadByte(void) {
    uint8_t byte;
    W5500_ReadBuff(&byte, sizeof(byte));
    return byte;
}

void W5500Chip::W5500_WriteByte(uint8_t byte) {
    W5500_WriteBuff(&byte, sizeof(byte));
}
