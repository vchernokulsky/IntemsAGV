#pragma once

#define DHCP_SOCKET     0
#define DNS_SOCKET      1
#define HTTP_SOCKET     2

#define BUFF_SIZE 20

#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "socket.h"
#include "uart_helper.h"
#include "W5500_chip.h"

#define CHIP W5500Chip

class SocketClient {
private:
	uint8_t http_socket;
	UartHelper *uart_helper;
	volatile bool ip_assigned;
	uint8_t addr[4] = {192, 168, 88, 2};
	//uint8_t addr[4] = {192, 168, 55, 100};
	uint16_t port = 5004;
	CHIP* chip;

	void socket_init();

public:
	SocketClient();
	virtual ~SocketClient();

	void init(SPI_HandleTypeDef *main_hspi1, UartHelper *main_uart_helper);
	void socket_connect();
	void socket_send(uint8_t *pData, uint16_t len);
	void socket_send(const char *pData, uint16_t len);
	void socket_receive(uint8_t *pData, uint16_t Size, uint32_t* rdmaInd);
	void socket_close();
};
