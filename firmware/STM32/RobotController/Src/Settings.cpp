#include "Settings.h"

Settings::Settings(I2C_HandleTypeDef *main_hi2c1){
	hi2c1 = main_hi2c1;
	set_default(false);
	HAL_Delay(100);
	read_all();
}
Settings::~Settings(){

}

void Settings::wait_for_readiness()
{
	HAL_StatusTypeDef status;
	for(;;)
	{
		status = HAL_I2C_IsDeviceReady(hi2c1, DEVICE_ADDRESS, 1, HAL_MAX_DELAY);
		if(status == HAL_OK)
		{
			break;
		}
	}
}

void Settings::extract_variables()
{
	memcpy(wiznet_mac_address, message_out + MAC_ADDRESS_OFFSET, MAC_ADDRESS_SIZE);
	memcpy(wiznet_ip_address, message_out + WIZNET_IP_OFFSET, IP_SIZE);
	wiznet_port = (message_out[WIZNET_PORT_OFFSET + 1] << 8) | message_out[WIZNET_PORT_OFFSET];
	memcpy(wiznet_mask, message_out + WIZNET_MASK_OFFSET, IP_SIZE);
	memcpy(wiznet_gateaway, message_out + WIZET_GATE_AWAY_OFFSET, IP_SIZE);
}

bool Settings::is_set()
{
	const char set_flag[] = "set";
	char rmsg[sizeof(set_flag)];
	wait_for_readiness();
	HAL_I2C_Mem_Read(hi2c1, DEVICE_ADDRESS, DEFAULT_ADDRESS + SET_FLAG_OFFSET, I2C_MEMADD_SIZE_16BIT, (uint8_t*)rmsg, sizeof(rmsg), HAL_MAX_DELAY);
	return (memcmp(rmsg, set_flag, sizeof(rmsg)) == 0);
}



void Settings::set_default(bool force){
	if(force || !is_set())
	{
		int offset = SET_FLAG_OFFSET;
		const char set_flag[] = "set";
		memcpy(message_out + offset, set_flag, sizeof(set_flag));

		offset = MAC_ADDRESS_OFFSET;
		uint8_t mac[] = {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef};
		memcpy(message_out + offset, mac, sizeof(mac));

		offset = WIZNET_IP_OFFSET;
		uint8_t ip[] = {192, 168, 2, 114};
		memcpy(message_out + offset, ip, sizeof(ip));

		offset = WIZNET_PORT_OFFSET;
		uint16_t port = 10888;
		 message_out[offset] = port & 0xFF;
		 message_out[offset+1] = port >> 8;

		offset = WIZNET_MASK_OFFSET;
		uint8_t mask[] = {255, 255, 255, 0};
		memcpy(message_out + offset, mask, sizeof(mask));

		offset = WIZET_GATE_AWAY_OFFSET;
		uint8_t gw[] = {192, 168, 2, 1};
		memcpy(message_out + offset, gw, sizeof(gw));

		HAL_I2C_Mem_Write(hi2c1, DEVICE_ADDRESS, DEFAULT_ADDRESS, I2C_MEMADD_SIZE_16BIT, message_out, OUTPUT_SIZE, HAL_MAX_DELAY);
	}
}

void Settings::read_all()
{
	 wait_for_readiness();
	 HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c1, DEVICE_ADDRESS, DEFAULT_ADDRESS, I2C_MEMADD_SIZE_16BIT, message_out, OUTPUT_SIZE, HAL_MAX_DELAY);
	 if (status == HAL_OK){
		 extract_variables();
	 }
}


