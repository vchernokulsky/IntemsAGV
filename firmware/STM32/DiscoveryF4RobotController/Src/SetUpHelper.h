/*
 * SetUpHelper.h
 *
 *  Created on: Mar 5, 2020
 *      Author: developer
 */

#ifndef SETUPHELPER_H_
#define SETUPHELPER_H_

#define DEVICE_ADDRESS 0x50 << 1
#define DEFAULT_ADDRESS 0x0100
#define SETTING_SIZE 26

#define SET_STR "set"
#define SET_FLAG_OFFSET 0
#define LOCAL_IP_OFFSET 4
#define NETWORK_MASK_OFFSET 8
#define GATEAWAY_OFFSET 12
#define ROS_CLIENT_PORT_OFFSET 16
#define SET_UP_SERVER_PORT_OFFSET 18
#define SERIALNODE_IP_OFFSET 20
#define SERIALNODE_PORT_OFFSET 24

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include <string.h>
#include "User_config.h"
#include "System_config.h"


class SetUpHelper {
private:
	static I2C_HandleTypeDef *mem_out;
	uint8_t message_out[SETTING_SIZE];

	void wait_for_readiness();
	void set_default(bool force);
	bool is_set();
	void read_all();
	void extract_variables();
public:
	uint8_t LOCAL_IP_ADDRESS[IP_SIZE];
	uint8_t NETWORK_MASK[IP_SIZE];
	uint8_t GATEAWAY[IP_SIZE];
	uint16_t ROS_CLIENT_PORT;
	uint16_t SET_UP_SERVER_PORT;
	uint8_t SERIALNODE_IP[IP_SIZE];
	uint16_t SERIALNODE_PORT;

	float WHEEL_RADIUS;
	float WHEEL_SEPARATION;
	float MAX_LIN_SPEED;
	float MAX_ANG_VEL;
	uint8_t MAX_PWD_ALLOWED;
	float RAD_PER_TICK;

	SetUpHelper();
	virtual ~SetUpHelper();
	void memory_init(I2C_HandleTypeDef *main_hi2c1);
	void get_curr_memory(uint8_t *buff);
	bool set(uint8_t *buff);
};

#endif /* SETUPHELPER_H_ */
