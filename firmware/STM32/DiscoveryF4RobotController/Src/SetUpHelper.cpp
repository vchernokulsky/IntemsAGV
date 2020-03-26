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

void SetUpHelper::set_default_network()
{
	uint16_t offset = LOCAL_IP_OFFSET;
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

	offset = SERIALNODE_IP_OFFSET;
	uint8_t sn_ip[] = SERVER_IP_ADRESS;
	memcpy(message_out + offset, sn_ip, IP_SIZE);

	offset = SERIALNODE_PORT_OFFSET;
	port = SERVER_PORT;
	message_out[offset] = port & 0xFF;
	message_out[offset+1] = port >> 8;
}

void SetUpHelper::set_default_robot_geometry()
{
	uint16_t offset = WHEEL_RADIUS_OFFSET;
	uint16_t radius = std::round(DEFAULT_RADIUS * 1000.0);
	message_out[offset] = radius & 0xFF;
	message_out[offset+1] = radius >> 8;

	offset = WHEEL_SEPARATION_OFFSET;
	uint16_t separation = std::round(DEFAULT_WHEEL_SEPARATION * 1000.0);
	message_out[offset] = separation & 0xFF;
	message_out[offset+1] = separation >> 8;

	offset = MAX_LIN_VEL_OFFSET;
	uint16_t vel = std::round(DEFAULT_MAX_LIN_SPEED * 1000.0);
	message_out[offset] = vel & 0xFF;
	message_out[offset+1] = vel >> 8;

	offset = MAX_ANG_VEL_OFFSET;
	vel = std::round(DEFAULT_ANG_VEL_MAX * 1000.0);
	message_out[offset] = vel & 0xFF;
	message_out[offset+1] = vel >> 8;

	offset = RAD_PER_TICK_OFFSET;
	uint16_t rad = std::round(DEFAULT_RAD_PER_TICK * 100000.0);
	message_out[offset] = rad & 0xFF;
	message_out[offset+1] = rad >> 8;

	offset = MAX_PWD_ALLOWED_OFFSET;
	uint16_t pwd = DEFAULT_MAX_PWD_ALLOWED;
	message_out[offset] = pwd & 0xFF;
	message_out[offset+1] = pwd >> 8;

}

void SetUpHelper::set_default_topics_name()
{
	uint16_t offset = TOPICS_OFFSET;

	char cmd_vel[] = DEFAULT_CMDVEL_TOPIC;
	uint8_t topic_size = strlen(cmd_vel);
	message_out[offset] = topic_size;
	offset += 1;
	memcpy(message_out + offset, cmd_vel, topic_size);
	offset += topic_size;

	char odom[] = DEFAULT_ODOMETRY_TOPIC;
	topic_size = strlen(odom);
	message_out[offset] = topic_size;
	offset += 1;
	memcpy(message_out + offset, odom, topic_size);
	offset += topic_size;

	char base_frame[] = DEFAULT_BASE_FRAME;
	topic_size = strlen(base_frame);
	message_out[offset] = topic_size;
	offset += 1;
	memcpy(message_out + offset, base_frame, topic_size);
	offset += topic_size;

	char odom_frame[] = DEFAULT_ODOMETRY_FRAME;
	topic_size = strlen(odom_frame);
	message_out[offset] = topic_size;
	offset += 1;
	memcpy(message_out + offset, odom_frame, topic_size);
	offset += topic_size;

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
	if(force || !is_set())
	{
		uint16_t offset = SET_FLAG_OFFSET;
		const char set_flag[] = "set";
		memcpy(message_out + offset, set_flag, sizeof(set_flag));

		set_default_network();
		set_default_robot_geometry();
		set_default_topics_name();

		message_out[MSG_SIZE_OFFSET] = msg_length & 0xFF;
		message_out[MSG_SIZE_OFFSET + 1] = msg_length >> 8;

		calc_checksum();
		memory_write();
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
	uint8_t rmsg[PORT_SIZE];
	wait_for_readiness();
	HAL_I2C_Mem_Read(SetUpHelper::mem_out, DEVICE_ADDRESS, DEFAULT_ADDRESS + MSG_SIZE_OFFSET, I2C_MEMADD_SIZE_16BIT, rmsg, sizeof(rmsg), HAL_MAX_DELAY);
	return (rmsg[1] << 8) | rmsg[0];
}

void SetUpHelper::read_all()
{
	if(!is_set())
	{
		return;
	}
	uint16_t mem_size = read_mem_size();
	 HAL_StatusTypeDef status = memory_read(mem_size);
	 if (status == HAL_OK){
		 extract_variables();
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

	memcpy(LOCAL_IP_ADDRESS, message_out + LOCAL_IP_OFFSET, IP_SIZE);
	memcpy(NETWORK_MASK, message_out + NETWORK_MASK_OFFSET, IP_SIZE);
	memcpy(GATEAWAY, message_out + GATEAWAY_OFFSET, IP_SIZE);

	ROS_CLIENT_PORT = (message_out[ROS_CLIENT_PORT_OFFSET + 1] << 8) | message_out[ROS_CLIENT_PORT_OFFSET];
	SET_UP_SERVER_PORT = (message_out[SET_UP_SERVER_PORT_OFFSET + 1] << 8) | message_out[SET_UP_SERVER_PORT_OFFSET];

	memcpy(SERIALNODE_IP, message_out + SERIALNODE_IP_OFFSET, IP_SIZE);
	SERIALNODE_PORT = (message_out[SERIALNODE_PORT_OFFSET + 1] << 8) | message_out[SERIALNODE_PORT_OFFSET];

	uint16_t radius_mm =  (message_out[WHEEL_RADIUS_OFFSET + 1] << 8) | message_out[WHEEL_RADIUS_OFFSET];
	WHEEL_RADIUS = (float)radius_mm / 1000.0f;

	uint16_t separation_mm =  (message_out[WHEEL_SEPARATION_OFFSET + 1] << 8) | message_out[WHEEL_SEPARATION_OFFSET];
	WHEEL_SEPARATION = (float)separation_mm / 1000.0f;

	uint16_t lin_vel_mm =  (message_out[MAX_LIN_VEL_OFFSET + 1] << 8) | message_out[MAX_LIN_VEL_OFFSET];
	MAX_LIN_SPEED = (float)lin_vel_mm / 1000.0f;

	uint16_t ang_vel_mr =  (message_out[MAX_ANG_VEL_OFFSET + 1] << 8) | message_out[MAX_ANG_VEL_OFFSET];
	MAX_ANG_VEL = (float)ang_vel_mr / 1000.0f;

	uint16_t rad_int =  (message_out[RAD_PER_TICK_OFFSET + 1] << 8) | message_out[RAD_PER_TICK_OFFSET];
	RAD_PER_TICK = (float)rad_int / 100000.0f;

	MAX_PWD_ALLOWED = (message_out[MAX_PWD_ALLOWED_OFFSET + 1] << 8) | message_out[MAX_PWD_ALLOWED_OFFSET];

	offset = TOPICS_OFFSET;
	char null_char[]  = {'\0'};

	uint8_t topic_size = message_out[offset];
	offset += 1;
	memcpy(CMD_VEL_TOPIC, message_out + offset, topic_size);
	memcpy(CMD_VEL_TOPIC + topic_size, null_char, sizeof(null_char));
	offset += topic_size;

	topic_size = message_out[offset];
	offset += 1;
	memcpy(ODOM_TOPIC, message_out + offset, topic_size);
	memcpy(ODOM_TOPIC + topic_size, null_char, sizeof(null_char));
	offset += topic_size;

	topic_size = message_out[offset];
	offset += 1;
	memcpy(BASE_FRAME, message_out + offset, topic_size);
	memcpy(BASE_FRAME + topic_size, null_char, sizeof(null_char));
	offset += topic_size;

	topic_size = message_out[offset];
	offset += 1;
	memcpy(ODOM_FRAME, message_out + offset, topic_size);
	memcpy(ODOM_FRAME + topic_size, null_char, sizeof(null_char));
	offset += topic_size;

}

void SetUpHelper::get_curr_memory(uint8_t *buff)
{
	memcpy(buff, message_out, msg_length);
}

bool SetUpHelper::set(uint8_t *buff){
		uint16_t offset = SET_FLAG_OFFSET;
		const char set_flag[] = "set";
		memcpy(message_out + offset, set_flag, sizeof(set_flag));

		offset = sizeof(set_flag);
		uint16_t buff_size =  (buff[offset + 1] << 8) | buff[offset];
		if(buff_size < MIN_SETIING_SIZE || buff_size > MAX_SETTING_SIZE || !check_checksum(buff, buff_size))
		{
			return false;
		}
		msg_length = buff_size;
		memcpy(message_out + offset, buff + offset, buff_size - offset);
		calc_checksum();

		memory_write();
		return true;

}

