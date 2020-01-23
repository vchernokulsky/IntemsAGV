#pragma once

#define OUTPUT_SIZE 24

#define DEVICE_ADDRESS 0x50 << 1
#define DEFAULT_ADDRESS 0x0100
#define CUSTOM_ADDRESS 0x1000

#define SET_FLAG_OFFSET 0
#define MAC_ADDRESS_OFFSET 4
#define WIZNET_IP_OFFSET 10
#define WIZNET_PORT_OFFSET 14
#define WIZNET_MASK_OFFSET 16
#define WIZET_GATE_AWAY_OFFSET 20

bool is_set(I2C_HandleTypeDef *hi2c1)
{
	const char set_flag[] = "set";
	char rmsg[sizeof(set_flag)];
	HAL_I2C_Mem_Read(hi2c1, DEVICE_ADDRESS, DEFAULT_ADDRESS + SET_FLAG_OFFSET, I2C_MEMADD_SIZE_16BIT, (uint8_t*)rmsg, sizeof(rmsg), HAL_MAX_DELAY);
	return (memcmp(rmsg, set_flag, sizeof(rmsg)) == 0);
}

template<typename A, typename V>
  static void varToArr(A arr, const V var)
  {
    for (size_t i = 0; i < sizeof(V); i++)
      arr[i] = (var >> (8 * i));
  }

void set_default(I2C_HandleTypeDef *hi2c1, bool force){
	uint8_t message_out[OUTPUT_SIZE];
	if(force || !is_set(hi2c1))
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



		HAL_I2C_Mem_Write(hi2c1, DEVICE_ADDRESS, DEFAULT_ADDRESS, I2C_MEMADD_SIZE_16BIT,
			(uint8_t*)message_out, OUTPUT_SIZE, HAL_MAX_DELAY);
	}
}

void read_all_default(I2C_HandleTypeDef *hi2c1, uint8_t *rmsg)
{
	 HAL_StatusTypeDef status;
	for(;;) { // wait...
		status = HAL_I2C_IsDeviceReady(hi2c1, DEVICE_ADDRESS, 1, HAL_MAX_DELAY);
		if(status == HAL_OK)
			break;
	}
	status = HAL_I2C_Mem_Read(hi2c1, DEVICE_ADDRESS, DEFAULT_ADDRESS, I2C_MEMADD_SIZE_16BIT, rmsg, OUTPUT_SIZE, HAL_MAX_DELAY);
}

