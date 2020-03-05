/*
 * Settings.h
 *
 *  Created on: Mar 2, 2020
 *      Author: developer
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "User_config.h"
#include <string.h>

#define BOOL_TRUE 1
#define BOOL_FALSE 0

#define SETTING_SIZE 8

#define DEVICE_ADDRESS 0x50 << 1
#define DEFAULT_ADDRESS 0x0100

#define SET_FLAG_OFFSET 0
#define LOCAL_IP_OFFSET 4


//#define WIZNET_PORT_OFFSET 14
//#define WIZNET_MASK_OFFSET 16
//#define WIZET_GATE_AWAY_OFFSET 20

#define IP_SIZE 4


static I2C_HandleTypeDef *mem_out;
static uint8_t message_out[SETTING_SIZE];
static uint8_t LOCAL_IP_ADDRESS[IP_SIZE];

static uint8_t LOCAL_IP_ADDRESS_0;
static uint8_t LOCAL_IP_ADDRESS_1;
static uint8_t LOCAL_IP_ADDRESS_2;
static uint8_t LOCAL_IP_ADDRESS_3;


static void wait_for_readiness()
{
	HAL_StatusTypeDef status;
	for(;;)
	{
		status = HAL_I2C_IsDeviceReady(mem_out, DEVICE_ADDRESS, 1, HAL_MAX_DELAY);
		if(status == HAL_OK)
		{
			break;
		}
	}
}

static uint8_t is_set()
{
	const char set_flag[] = "set";
	char rmsg[sizeof(set_flag)];
	wait_for_readiness();
	HAL_I2C_Mem_Read(mem_out, DEVICE_ADDRESS, DEFAULT_ADDRESS + SET_FLAG_OFFSET, I2C_MEMADD_SIZE_16BIT, (uint8_t*)rmsg, sizeof(rmsg), HAL_MAX_DELAY);
	if (memcmp(rmsg, set_flag, sizeof(rmsg)) == 0)
	{
		return BOOL_TRUE;
	} else
	{
		return BOOL_FALSE;
	}
}


static void set_default(uint8_t force){
	if((force == BOOL_TRUE) || !is_set())
	{
		int offset = SET_FLAG_OFFSET;
		const char set_flag[] = "set";
		memcpy(message_out + offset, set_flag, sizeof(set_flag));

		offset = LOCAL_IP_OFFSET;
		uint8_t ip[] = WIZNET_IP_ADRESS;
		memcpy(message_out + offset, ip, sizeof(ip));

//		offset = WIZNET_PORT_OFFSET;
//		uint16_t port = WIZNET_PORT;
//		 message_out[offset] = port & 0xFF;
//		 message_out[offset+1] = port >> 8;
//
//		offset = WIZNET_MASK_OFFSET;
//		uint8_t mask[] = WIZNET_MASK;
//		memcpy(message_out + offset, mask, sizeof(mask));
//
//		offset = WIZET_GATE_AWAY_OFFSET;
//		uint8_t gw[] = WIZNET_GATEAWAY;
//		memcpy(message_out + offset, gw, sizeof(gw));

		HAL_StatusTypeDef status = HAL_I2C_Mem_Write(mem_out, DEVICE_ADDRESS, DEFAULT_ADDRESS, I2C_MEMADD_SIZE_16BIT, message_out, SETTING_SIZE, HAL_MAX_DELAY);
		osDelay(1);
	}
}

static void extract_variables()
{
//	memcpy(LOCAL_IP_ADDRESS, message_out + LOCAL_IP_OFFSET, IP_SIZE);
	LOCAL_IP_ADDRESS_0 = message_out[LOCAL_IP_OFFSET + 0];
	LOCAL_IP_ADDRESS_1 = message_out[LOCAL_IP_OFFSET + 1];
	LOCAL_IP_ADDRESS_2 = message_out[LOCAL_IP_OFFSET + 2];
	LOCAL_IP_ADDRESS_3 = message_out[LOCAL_IP_OFFSET + 3];

//	wiznet_client_port = (message_out[WIZNET_PORT_OFFSET + 1] << 8) | message_out[WIZNET_PORT_OFFSET];
//	memcpy(wiznet_mask, message_out + WIZNET_MASK_OFFSET, IP_SIZE);
//	memcpy(wiznet_gateaway, message_out + WIZET_GATE_AWAY_OFFSET, IP_SIZE);
}

static void read_all()
{
	 wait_for_readiness();
	 HAL_StatusTypeDef status = HAL_I2C_Mem_Read(mem_out, DEVICE_ADDRESS, DEFAULT_ADDRESS, I2C_MEMADD_SIZE_16BIT, message_out, SETTING_SIZE, HAL_MAX_DELAY);
	 if (status == HAL_OK){
		 extract_variables();
	 }
}


static void memory_init(I2C_HandleTypeDef *main_hi2c1)
{
	mem_out = main_hi2c1;
	set_default(BOOL_TRUE);
	osDelay(100);
	read_all();
	osDelay(100);
}

static uint8_t* get_loacal_ip()
{
	return LOCAL_IP_ADDRESS;
}


#endif /* SETTINGS_H_ */
