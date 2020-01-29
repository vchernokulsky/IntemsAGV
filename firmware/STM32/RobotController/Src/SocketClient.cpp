/*
 * SocketClient.cpp
 *
 *  Created on: Nov 26, 2019
 *      Author: developer
 */

#include "SocketClient.h"
SPI_HandleTypeDef *SocketClient::hspi1;
uint8_t SocketClient::error_count = 0;

SemaphoreHandle_t SocketClient::error_semaphore = xSemaphoreCreateMutex() ;

SocketClient::SocketClient(SPI_HandleTypeDef *main_hspi1, UartHelper *main_uart_helper) {
	hspi1 = main_hspi1;
	uart_helper = main_uart_helper;
	SocketClient::socket_reset();
}

SocketClient::~SocketClient() {
	(*uart_helper).printf("\r\ndestructor\r\n");
	SocketClient::socket_close();
}
void SocketClient::socket_connect(){
	int8_t code = connect(http_socket, addr, port);
    if(code > 0)
    {
    	(*uart_helper).printf("socket connected\r\n");
    }
    else
    {
    	(*uart_helper).printf("\r\nconnect() failed\r\n");
    	(*uart_helper).printf("\r\ncode = %d\r\n", code);
    	if (code < 0){
    		close(http_socket);
    	}
    }
}

void SocketClient::socket_reset()
{
	if (SocketClient::socket_init())
	{
		HAL_Delay(500);
		SocketClient::socket_connect();
	}
}

void SocketClient::socket_send(uint8_t *pData, uint16_t len){

    while(len > 0) {
        int32_t nbytes = send(http_socket, pData, len);
        if(nbytes <= 0) {
        	socket_error();
        	(*uart_helper).printf("send() failed, %d returned\r\n", nbytes);;
        	osDelay(50);
        } else{
        	socket_success();
//			(*uart_helper).printf("%d bytes sent!\r\n", nbytes);
			len -= nbytes;
        }
    }
}

void SocketClient::socket_send(const char *pData, uint16_t len){

    while(len > 0) {
        int32_t nbytes = send(http_socket, (uint8_t*)pData, len);
        if(nbytes <= 0) {
        	socket_error();
        	(*uart_helper).printf("send() failed, %d returned\r\n", nbytes);
        	osDelay(50);
        } else{
        	socket_success();
//			(*uart_helper).printf("%d bytes sent!\r\n", nbytes);
			len -= nbytes;
        }
    }
}

void SocketClient::socket_receive(uint8_t *pData, uint16_t Size, uint32_t* rdmaInd){

		int32_t nbytes = 0;
		uint16_t recvsize = getSn_RX_RSR(http_socket);
		if (recvsize > 0){
			nbytes = recv(http_socket, pData, Size);
		}
		*rdmaInd = nbytes;

		if(nbytes < 0) {
			socket_error();
			(*uart_helper).printf("\r\nrecv() failed, %d returned\r\n", nbytes);
			return;
		}
		if (nbytes > 0){
			socket_success();
//			(*uart_helper).printf("\r\nrecv() %d returned\r\n", nbytes);
			return;
		} else {
//			(*uart_helper).printf("\r\nrecv() socket busy\r\n");
			return;
		}
}


void SocketClient::socket_close(){
	  close(http_socket);
	  (*uart_helper).printf("Closing socket.\r\n");
}

bool SocketClient::socket_init(){
	/******* RESET WIZNET**********/
	 HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_RESET);
	 HAL_Delay(100);
	 HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);
	 HAL_Delay(100);
	/****** REGISTER SOCKET CALLBACKS ******/
    reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);
    reg_wizchip_spi_cbfunc(SocketClient::W5500_ReadByte, SocketClient::W5500_WriteByte);
    reg_wizchip_spiburst_cbfunc(SocketClient::W5500_ReadBuff, SocketClient::W5500_WriteBuff);
    /****** INIT SOCKET ******/
    uint8_t rx_tx_buff_sizes[] = {2, 2, 2, 2};
    wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);
    wiz_NetInfo net_info = {
    	.mac = WIZNET_MAC_ADRESS, // MAC адрес
        .ip = WIZNET_IP_ADRESS, // IP адрес
        .sn = WIZNET_MASK, // маска сети
        .gw = WIZNET_GATEAWAY}; // адрес шлюза
    wizchip_setnetinfo(&net_info);
    wizchip_getnetinfo(&net_info);
    SocketClient::error_count = 0;
    data_exchange_time = HAL_GetTick();
    /***** OPEN SOCKET *****/
    SocketClient::http_socket = HTTP_SOCKET;
    uint8_t code = socket(SocketClient::http_socket, Sn_MR_TCP, WIZNET_PORT, SF_IO_NONBLOCK );
    if(code != SocketClient::http_socket) {
    	(*uart_helper).printf("socket open failed, code = %d\r\n", code);
        return false;
    }
    (*uart_helper).printf("\r\nSocket opened\r\n");
    return true;
}

void SocketClient::socket_error()
{
	if( xSemaphoreTake( SocketClient::error_semaphore, portMAX_DELAY) == pdTRUE )
	{
		++SocketClient::error_count;
		data_exchange_time = HAL_GetTick();
		xSemaphoreGive( SocketClient::error_semaphore );
	}
}
void SocketClient::socket_success()
{
	if( xSemaphoreTake( SocketClient::error_semaphore, portMAX_DELAY) == pdTRUE )
	{
		if(SocketClient::error_count > 0)
		{
			--SocketClient::error_count;
		}
		data_exchange_time = HAL_GetTick();
		xSemaphoreGive( SocketClient::error_semaphore );
	}

}

void SocketClient::CheckFreezingTask()
{
	for(;;)
	{
		if( xSemaphoreTake( SocketClient::error_semaphore, portMAX_DELAY) == pdTRUE )
		{
			if(SocketClient::error_count > WIZNET_MAX_ERROR_COUNT || (uint32_t)(HAL_GetTick() - data_exchange_time) > WIZNET_FREEZE_TIMEOUT)
			{
				xSemaphoreGive( SocketClient::error_semaphore );
				(*uart_helper).printf("\r\nSocket Reseting\r\n");
				socket_reset();
			} else {
				xSemaphoreGive( SocketClient::error_semaphore );
			}

		}
		osDelay(WIZNET_CHECK_FREEZING_DELAY);
	}
}

void SocketClient::W5500_Select(void) {
    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET);
}

void SocketClient::W5500_Unselect(void) {
    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_SET);
}

void SocketClient::W5500_ReadBuff(uint8_t* buff, uint16_t len) {
	uint32_t begin = HAL_GetTick();
	HAL_SPI_Receive(SocketClient::hspi1, buff, len, 100);
	uint32_t end = HAL_GetTick();
	if(end - begin > 110) {
		osDelay(50);
	}
}

void SocketClient::W5500_WriteBuff(uint8_t* buff, uint16_t len) {
	uint32_t begin = HAL_GetTick();
	HAL_SPI_Transmit(SocketClient::hspi1, buff, len, 100);
	uint32_t end = HAL_GetTick();
	if(end - begin > 110) {
		osDelay(50);
	}
}

uint8_t SocketClient::W5500_ReadByte(void) {
    uint8_t byte;
    W5500_ReadBuff(&byte, sizeof(byte));
    return byte;
}

void SocketClient::W5500_WriteByte(uint8_t byte) {
    W5500_WriteBuff(&byte, sizeof(byte));
}

