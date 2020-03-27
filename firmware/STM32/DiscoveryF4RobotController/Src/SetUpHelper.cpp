/*
 * SetUpHelper.cpp
 *
 *  Created on: Mar 5, 2020
 *      Author: developer
 */

#include "SetUpHelper.h"

#define PUT_IP(offset, val) \
	memcpy(message_out + (offset), (val), IP_SIZE);

#define PUT_NUM(offset, val) \
	message_out[(offset)] = (val) & 0xFF; \
	message_out[(offset)+1] = (val) >> 8;

#define PUT_TOPIC(offset, str, size) \
		message_out[(offset)] = (size); \
		(offset) += 1; \
		memcpy(message_out + (offset), (str), (size)); \
		(offset) += (size);

#define GET_IP(offset, ip) \
	memcpy((ip), message_out + (offset), IP_SIZE);

#define GET_NUM(offset) \
	(message_out[offset + 1] << 8) | message_out[offset]

#define GET_TOPIC(offset, topic, null_char ) \
	memcpy((topic), message_out + (offset) + 1, message_out[(offset)]); \
	memcpy((topic) + message_out[(offset)], (null_char), sizeof((null_char))); \
	(offset) = (offset) + message_out[(offset)] + 1;

I2C_HandleTypeDef *SetUpHelper::mem_out = nullptr;
SemaphoreHandle_t SetUpHelper::semaphore;
bool SetUpHelper::is_default = false;
bool SetUpHelper::reset_config = false;
SetUpHelper::SetUpHelper() {
	// TODO Auto-generated constructor stub

}

SetUpHelper::~SetUpHelper() {
	// TODO Auto-generated destructor stub
}

void SetUpHelper::memory_init(I2C_HandleTypeDef *main_hi2c1)
{
	SetUpHelper::mem_out = main_hi2c1;
	SetUpHelper::semaphore = xSemaphoreCreateMutex();
	set_default(false);
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
			HAL_GPIO_WritePin(GPIO_EEPROM_LED, PIN_EEPROM_LED, GPIO_PIN_SET);
			break;
		} else
		{
			HAL_GPIO_WritePin(GPIO_EEPROM_LED, PIN_EEPROM_LED, GPIO_PIN_RESET);
		}
		osDelay(10);
	}
}

void SetUpHelper::set_default_network()
{
	uint8_t ip[] = DEFAULT_ROBOT_IP_ADRESS;
	uint8_t mask[] = DEFAULT_NETWORK_MASK;
	uint8_t gw[] = DEFAULT_GATEAWAY;
	uint8_t sn_ip[] = DEFAULT_ROS_SERIALNODE_IP_ADRESS;

	PUT_IP(LOCAL_IP_OFFSET, ip);
	PUT_IP(NETWORK_MASK_OFFSET, mask);
	PUT_IP(GATEAWAY_OFFSET, gw);
	PUT_NUM(ROS_CLIENT_PORT_OFFSET, DEFAULT_ROS_CLIENT_PORT);
	PUT_NUM(SET_UP_SERVER_PORT_OFFSET, DEFAULT_SETUP_SERVER_PORT);
	PUT_IP(SERIALNODE_IP_OFFSET, sn_ip);
	PUT_NUM(SERIALNODE_PORT_OFFSET, DEFAULT_ROS_SERIALNODE_PORT);
}

void SetUpHelper::set_default_robot_geometry()
{
	uint16_t radius = std::round(DEFAULT_RADIUS * 1000.0);
	uint16_t separation = std::round(DEFAULT_WHEEL_SEPARATION * 1000.0);
	uint16_t vel = std::round(DEFAULT_MAX_LIN_SPEED * 1000.0);
	uint16_t max_ang_vel = std::round(DEFAULT_ANG_VEL_MAX * 1000.0);
	uint16_t rad = std::round(DEFAULT_RAD_PER_TICK * 100000.0);
	uint16_t pwd = DEFAULT_MAX_PWD_ALLOWED;

	PUT_NUM(WHEEL_RADIUS_OFFSET, radius);
	PUT_NUM(WHEEL_SEPARATION_OFFSET, separation);
	PUT_NUM(MAX_LIN_VEL_OFFSET, vel);
	PUT_NUM(MAX_ANG_VEL_OFFSET, max_ang_vel);
	PUT_NUM(RAD_PER_TICK_OFFSET, rad);
	PUT_NUM(MAX_PWD_ALLOWED_OFFSET, pwd);
}

void SetUpHelper::set_default_topics_name()
{
	uint16_t offset = TOPICS_OFFSET;

	char cmd_vel[] = DEFAULT_CMDVEL_TOPIC;
	char odom[] = DEFAULT_ODOMETRY_TOPIC;
	char base_frame[] = DEFAULT_BASE_FRAME;
	char odom_frame[] = DEFAULT_ODOMETRY_FRAME;

	uint8_t topic_size = strlen(cmd_vel);
	PUT_TOPIC(offset, cmd_vel, topic_size);

	topic_size = strlen(odom);
	PUT_TOPIC(offset, odom, topic_size);

	topic_size = strlen(base_frame);
	PUT_TOPIC(offset, base_frame, topic_size);

	topic_size = strlen(odom_frame);
	PUT_TOPIC(offset, odom_frame, topic_size);

	 msg_length = offset + 1;
}

void SetUpHelper::calc_checksum() {
  uint16_t c = 0;
  for(uint16_t i = 0; i < (msg_length - 1); i++) {
    c += message_out[i];
    c %= 256;
  }
  message_out[msg_length - 1] = 255 - c;
}
bool SetUpHelper::check_checksum(uint8_t *buff, uint16_t size) {
  uint16_t c = 0;
  for(uint16_t i = 0; i < size; i++) {
    c += buff[i];
    c %= 256;
  }
  return (c == 255);
}

void SetUpHelper::memory_write()
{
	HAL_StatusTypeDef status;
	uint16_t offset = 0;
	uint16_t bytes_left = msg_length - offset;
	uint8_t size = 0;
	while(bytes_left > 0)
	{
		size = (bytes_left > 64)? 64 : bytes_left;
		status = HAL_I2C_Mem_Write(mem_out, DEVICE_ADDRESS, DEFAULT_ADDRESS + offset, I2C_MEMADD_SIZE_16BIT, message_out + offset, size, HAL_MAX_DELAY);
		osDelay(1);
		wait_for_readiness();
		offset += size;
		bytes_left -= size;

	}
}

HAL_StatusTypeDef SetUpHelper::memory_read(uint16_t read_size)
{
	HAL_StatusTypeDef status;
	uint16_t offset = 0;
	uint16_t bytes_left = read_size - offset;
	uint8_t size = 0;
	while(bytes_left > 0)
	{
		size = (bytes_left > 64)? 64 : bytes_left;
		wait_for_readiness();
		status = HAL_I2C_Mem_Read(mem_out, DEVICE_ADDRESS, DEFAULT_ADDRESS + offset, I2C_MEMADD_SIZE_16BIT, message_out + offset, size, HAL_MAX_DELAY);
		if (status != HAL_OK){
			return status;
		}
		offset += size;
		bytes_left -= size;
	}
	return HAL_OK;
}

void SetUpHelper::set_default(bool force)
{
	if(!SetUpHelper::mem_out){
		return;
	}
	if( xSemaphoreTake( SetUpHelper::semaphore, portMAX_DELAY) == pdTRUE )
	{
		if((force || !is_set())&&!is_default)
		{
			uint16_t offset = SET_FLAG_OFFSET;
			const char set_flag[] = "set";
			memcpy(message_out + offset, set_flag, sizeof(set_flag));

			set_default_network();
			set_default_robot_geometry();
			set_default_topics_name();

			PUT_NUM(MSG_SIZE_OFFSET, msg_length);

			calc_checksum();
			memory_write();
			is_default = true;
		}
		xSemaphoreGive( SetUpHelper::semaphore );
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

uint16_t SetUpHelper::read_mem_size()
{
	uint8_t rmsg[NUM_SIZE];
	wait_for_readiness();
	HAL_I2C_Mem_Read(SetUpHelper::mem_out, DEVICE_ADDRESS, DEFAULT_ADDRESS + MSG_SIZE_OFFSET, I2C_MEMADD_SIZE_16BIT, rmsg, sizeof(rmsg), HAL_MAX_DELAY);
	return (rmsg[1] << 8) | rmsg[0];
}

void SetUpHelper::read_all()
{
	if( xSemaphoreTake( SetUpHelper::semaphore, portMAX_DELAY) == pdTRUE )
	{
		if(is_set())
		{
			uint16_t mem_size = read_mem_size();
			HAL_StatusTypeDef status = memory_read(mem_size);
			if (status == HAL_OK){
				extract_variables();
			}
		}
		xSemaphoreGive( SetUpHelper::semaphore );
	}
}

void SetUpHelper::extract_variables()
{
	uint16_t offset = MSG_SIZE_OFFSET;
	uint16_t msg_size =  (message_out[offset + 1] << 8) | message_out[offset];
	if(msg_size < MIN_SETIING_SIZE || msg_size > MAX_SETTING_SIZE || !check_checksum(message_out, msg_size))
	{
		return;
	}
	msg_length = msg_size;

	GET_IP(LOCAL_IP_OFFSET, LOCAL_IP_ADDRESS);
	GET_IP(NETWORK_MASK_OFFSET, NETWORK_MASK);
	GET_IP(GATEAWAY_OFFSET, GATEAWAY);
	ROS_CLIENT_PORT = GET_NUM(ROS_CLIENT_PORT_OFFSET);
	SET_UP_SERVER_PORT = GET_NUM(SET_UP_SERVER_PORT_OFFSET);

	GET_IP(SERIALNODE_IP_OFFSET, SERIALNODE_IP);
	SERIALNODE_PORT = GET_NUM(SERIALNODE_PORT_OFFSET);

	uint16_t radius_mm =  GET_NUM(WHEEL_RADIUS_OFFSET);
	WHEEL_RADIUS = (float)radius_mm / 1000.0f;

	uint16_t separation_mm =  GET_NUM(WHEEL_SEPARATION_OFFSET);
	WHEEL_SEPARATION = (float)separation_mm / 1000.0f;

	uint16_t lin_vel_mm =  GET_NUM(MAX_LIN_VEL_OFFSET);
	MAX_LIN_SPEED = (float)lin_vel_mm / 1000.0f;

	uint16_t ang_vel_mr =  GET_NUM(MAX_ANG_VEL_OFFSET);
	MAX_ANG_VEL = (float)ang_vel_mr / 1000.0f;

	uint16_t rad_int = GET_NUM(RAD_PER_TICK_OFFSET);
	RAD_PER_TICK = (float)rad_int / 100000.0f;

	MAX_PWD_ALLOWED = GET_NUM(MAX_PWD_ALLOWED_OFFSET);

	offset = TOPICS_OFFSET;
	char null_char[]  = {'\0'};

	GET_TOPIC(offset, CMD_VEL_TOPIC, null_char );
	GET_TOPIC(offset, ODOM_TOPIC, null_char );
	GET_TOPIC(offset, BASE_FRAME, null_char );
	GET_TOPIC(offset, ODOM_FRAME, null_char );

}

void SetUpHelper::get_curr_memory(uint8_t *buff)
{
	if( xSemaphoreTake( SetUpHelper::semaphore, portMAX_DELAY) == pdTRUE )
		{
			memcpy(buff, message_out, msg_length);
			xSemaphoreGive( SetUpHelper::semaphore );
		}
}

bool SetUpHelper::set(uint8_t *buff){
	bool ret = false;
	if( xSemaphoreTake( SetUpHelper::semaphore, portMAX_DELAY) == pdTRUE )
	{
		uint16_t offset = SET_FLAG_OFFSET;
		const char set_flag[] = "set";
		memcpy(message_out + offset, set_flag, sizeof(set_flag));

		offset = sizeof(set_flag);
		uint16_t buff_size =  (buff[offset + 1] << 8) | buff[offset];
		if((buff_size >= MIN_SETIING_SIZE) && (buff_size <= MAX_SETTING_SIZE) && check_checksum(buff, buff_size))
		{
			msg_length = buff_size;
			memcpy(message_out + offset, buff + offset, buff_size - offset);
			calc_checksum();
			memory_write();
			is_default = false;
			ret = true;
		}

		xSemaphoreGive( SetUpHelper::semaphore );
	}
	return ret;
}

void SetUpHelper::set_default_task()
{
	for(;;)
	{
		if(reset_config)
		{
			set_default(true);
			reset_config = false;
		}
		osDelay(500);
	}
}

