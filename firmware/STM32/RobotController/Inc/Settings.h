#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <string.h>

#include "User_config.h"

#define SETTING_SIZE 24

#define DEVICE_ADDRESS 0x50 << 1
#define DEFAULT_ADDRESS 0x0100
#define CUSTOM_ADDRESS 0x1000

#define SET_FLAG_OFFSET 0
#define MAC_ADDRESS_OFFSET 4
#define WIZNET_IP_OFFSET 10
#define WIZNET_PORT_OFFSET 14
#define WIZNET_MASK_OFFSET 16
#define WIZET_GATE_AWAY_OFFSET 20

#define MAC_ADDRESS_SIZE 6
#define IP_SIZE 4

class Settings {
private:
	I2C_HandleTypeDef *hi2c1;
	uint8_t message_out[SETTING_SIZE];
	bool is_set();
	void wait_for_readiness();
	void extract_variables();
public:
	uint8_t wiznet_mac_address[MAC_ADDRESS_SIZE];
	uint8_t wiznet_ip_address[IP_SIZE];
	uint16_t wiznet_client_port;
	uint16_t wiznet_server_port;
	uint8_t wiznet_mask[IP_SIZE];
	uint8_t wiznet_gateaway[IP_SIZE];
	Settings(I2C_HandleTypeDef *main_hi2c1);
	~Settings();
	void set_default(bool force);
	void read_all();
};



