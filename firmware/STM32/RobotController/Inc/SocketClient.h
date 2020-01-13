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
#define HTTP_SOCKET     2

#define W5500_CS_Pin GPIO_PIN_6
#define W5500_CS_GPIO_Port GPIOB

#define W5500_RST_Pin GPIO_PIN_7
#define W5500_RST_GPIO_Port GPIOC

#define BUFF_SIZE 20
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "stm32f4xx_hal.h"

#include "socket.h"
#include "UartHelper.h"

class SocketClient {
private:
	static SPI_HandleTypeDef *hspi1;
	uint8_t http_socket;
	UartHelper *uart_helper;

	volatile bool ip_assigned;
	uint8_t addr[4] = {192, 168, 2, 150};
	uint16_t port = 11411;

	void socket_init();


	static void W5500_Select(void);
	static void W5500_Unselect(void);
	static void W5500_ReadBuff(uint8_t* buff, uint16_t len);
	static void W5500_WriteBuff(uint8_t* buff, uint16_t len);
	static uint8_t W5500_ReadByte(void);
	static void W5500_WriteByte(uint8_t byte);
//	void UART_Printf(const char* fmt, ...);
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

#endif /* SOCKETCLIENT_H_ */
