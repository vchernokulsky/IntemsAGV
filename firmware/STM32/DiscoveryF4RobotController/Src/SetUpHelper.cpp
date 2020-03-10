/*
 * SetUpHelper.cpp
 *
 *  Created on: Mar 5, 2020
 *      Author: developer
 */

#include "SetUpHelper.h"


I2C_HandleTypeDef *SetUpHelper::mem_out = nullptr;

SetUpHelper::SetUpHelper() {
	// TODO Auto-generated constructor stub

}

SetUpHelper::~SetUpHelper() {
	// TODO Auto-generated destructor stub
}

void SetUpHelper::memory_init(I2C_HandleTypeDef *main_hi2c1)
{
	SetUpHelper::mem_out = main_hi2c1;
	set_default(true);
	osDelay(100);
	read_all();
	osDelay(100);
}

void SetUpHelper::wait_for_readiness()
{
	HAL_StatusTypeDef status;
	for(;;)
	{
		status = HAL_I2C_IsDeviceReady(SetUpHelper::mem_out, DEVICE_ADDRESS, 1, HAL_MAX_DELAY);
		if(status == HAL_OK)
		{
			break;
		}
	}
}

void SetUpHelper::set_default(bool force){
	if(force || !is_set())
	{
		int offset = SET_FLAG_OFFSET;
		const char set_flag[] = "set";
		memcpy(message_out + offset, set_flag, sizeof(set_flag));

		offset = LOCAL_IP_OFFSET;
		uint8_t ip[] = WIZNET_IP_ADRESS;
		memcpy(message_out + offset, ip, IP_SIZE);

		offset = NETWORK_MASK_OFFSET;
		uint8_t mask[] = WIZNET_MASK;
		memcpy(message_out + offset, mask, IP_SIZE);

		offset = GATEAWAY_OFFSET;
		uint8_t gw[] = WIZNET_GATEAWAY;
		memcpy(message_out + offset, gw, IP_SIZE);

		offset = ROS_CLIENT_PORT_OFFSET;
		uint16_t port = WIZNET_PORT;
		message_out[offset] = port & 0xFF;
		message_out[offset+1] = port >> 8;

		offset = SET_UP_SERVER_PORT_OFFSET;
		port = DEFAULT_SETUP_SERVER_PORT;
		message_out[offset] = port & 0xFF;
		message_out[offset+1] = port >> 8;

		HAL_StatusTypeDef status = HAL_I2C_Mem_Write(mem_out, DEVICE_ADDRESS, DEFAULT_ADDRESS, I2C_MEMADD_SIZE_16BIT, message_out, SETTING_SIZE, HAL_MAX_DELAY);
		osDelay(1);
	}
}

bool SetUpHelper::is_set()
{
	const char set_flag[] = SET_STR;
	char rmsg[sizeof(set_flag)];
	wait_for_readiness();
	HAL_I2C_Mem_Read(SetUpHelper::mem_out, DEVICE_ADDRESS, DEFAULT_ADDRESS + SET_FLAG_OFFSET, I2C_MEMADD_SIZE_16BIT, (uint8_t*)rmsg, sizeof(rmsg), HAL_MAX_DELAY);
	return memcmp(rmsg, set_flag, sizeof(rmsg)) == 0;
}

void SetUpHelper::read_all()
{
	 wait_for_readiness();
	 HAL_StatusTypeDef status = HAL_I2C_Mem_Read(mem_out, DEVICE_ADDRESS, DEFAULT_ADDRESS, I2C_MEMADD_SIZE_16BIT, message_out, SETTING_SIZE, HAL_MAX_DELAY);
	 if (status == HAL_OK){
		 extract_variables();
	 }
}

void SetUpHelper::extract_variables()
{
	memcpy(LOCAL_IP_ADDRESS, message_out + LOCAL_IP_OFFSET, IP_SIZE);
	memcpy(NETWORK_MASK, message_out + NETWORK_MASK_OFFSET, IP_SIZE);
	memcpy(GATEAWAY, message_out + GATEAWAY_OFFSET, IP_SIZE);

	ROS_CLIENT_PORT = (message_out[ROS_CLIENT_PORT_OFFSET + 1] << 8) | message_out[ROS_CLIENT_PORT_OFFSET];
	SET_UP_SERVER_PORT = (message_out[SET_UP_SERVER_PORT_OFFSET + 1] << 8) | message_out[SET_UP_SERVER_PORT_OFFSET];

}

void SetUpHelper::get_curr_memory(uint8_t *buff)
{
	memcpy(buff, message_out, SETTING_SIZE);
}

bool SetUpHelper::set(uint8_t *buff){
		int offset = SET_FLAG_OFFSET;
		const char set_flag[] = "set";
		memcpy(message_out + offset, set_flag, sizeof(set_flag));

		offset = LOCAL_IP_OFFSET;
		memcpy(message_out + offset, buff + offset, IP_SIZE);

		offset = NETWORK_MASK_OFFSET;
		memcpy(message_out + offset, buff + offset, IP_SIZE);

		offset = GATEAWAY_OFFSET;
		memcpy(message_out + offset, buff + offset, IP_SIZE);

		offset = ROS_CLIENT_PORT_OFFSET;
		memcpy(message_out + offset, buff + offset, PORT_SIZE);

		offset = SET_UP_SERVER_PORT_OFFSET;
		memcpy(message_out + offset, buff + offset, PORT_SIZE);

//		offset = WIZNET_PORT_OFFSET;
//		uint16_t port = WIZNET_PORT;
//		 message_out[offset] = port & 0xFF;
//		 message_out[offset+1] = port >> 8;

		HAL_StatusTypeDef status = HAL_I2C_Mem_Write(mem_out, DEVICE_ADDRESS, DEFAULT_ADDRESS, I2C_MEMADD_SIZE_16BIT, message_out, SETTING_SIZE, HAL_MAX_DELAY);
		osDelay(1);
		return status == HAL_OK;

}

