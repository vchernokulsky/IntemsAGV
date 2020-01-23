/*
 * SocketClient.h
 *
 *  Created on: Nov 26, 2019
 *      Author: developer
 */

#ifndef SOCKETCLIENT_H_
#define SOCKETCLIENT_H_

#define DHCP_SOCKET     0
#define DNS_SOCKET      1
#define HTTP_SOCKET_CLIENT    2
#define HTTP_SOCKET_SERVER    3



#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "stm32f4xx_hal.h"

#include "socket.h"
#include "UartHelper.h"
#include "Settings.h"

#include "System_config.h"
#include "User_config.h"

class SocketClient {
private:
	static bool wiznet_inited;
	static SPI_HandleTypeDef *hspi1;
	uint8_t error_count;
	uint32_t data_exchange_time;
	uint8_t http_socket;
	UartHelper *uart_helper;
	xQueueHandle queue;

	Settings *settings;

	uint8_t addr[4] = SERVER_IP_ADRESS;
	uint16_t port = SERVER_PORT;

	void wiznet_init();


	static void W5500_Select(void);
	static void W5500_Unselect(void);
	static void W5500_ReadBuff(uint8_t* buff, uint16_t len);
	static void W5500_WriteBuff(uint8_t* buff, uint16_t len);
	static uint8_t W5500_ReadByte(void);
	static void W5500_WriteByte(uint8_t byte);

public:
	SocketClient(SPI_HandleTypeDef *main_hspi1, UartHelper *main_uart_helper, Settings *main_settings, uint8_t socket_mode);
	virtual ~SocketClient();
	bool socket_open();
	void socket_connect();
	void socket_send(uint8_t *pData, uint16_t len);
	void socket_send(const char *pData, uint16_t len);
	void socket_receive(uint8_t *pData, uint16_t Size, uint32_t* rdmaInd);
	void socket_close();
	void socket_reset();
	void socket_error();
	void socket_success();
	void SocketStateTask();
	void CheckFreezingTask();
	void socketServerTestTask();
};

#endif /* SOCKETCLIENT_H_ */
