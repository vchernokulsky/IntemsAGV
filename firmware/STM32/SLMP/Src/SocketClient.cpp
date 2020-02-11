#include "SocketClient.h"

SPI_HandleTypeDef *W5500Chip::hspi;

SocketClient::SocketClient() {}

void SocketClient::init(SPI_HandleTypeDef *main_hspi1, UartHelper *main_uart_helper) {
	uart_helper = main_uart_helper;
	W5500Chip::hspi = main_hspi1;
	chip = new CHIP();

	 HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_RESET);
	 HAL_Delay(100);
	 HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);
	 HAL_Delay(100);

	SocketClient::socket_init();
	(*uart_helper).printf("socket inited\r\n");
}

SocketClient::~SocketClient() {
	(*uart_helper).printf("\r\ndestructor\r\n");
	SocketClient::socket_close();
}
void SocketClient::socket_connect(){
	//uint8_t code = connect(http_socket, addr, port);
	uint8_t code = listen(http_socket);

	if(code < 0) {
    	(*uart_helper).printf("connect() failed, code = %d\r\n", code);
        close(http_socket);
        return;
    }
    (*uart_helper).printf("socket connected\r\n");
}

void SocketClient::socket_send(uint8_t *pData, uint16_t len){

    while(len > 0) {
        //int32_t nbytes = send(http_socket, pData, len);
    	int32_t nbytes = sendto(http_socket, pData, len, addr, port);

    	if(nbytes <= 0) {
        	(*uart_helper).printf("send() failed, %d returned\r\n", nbytes);
            close(http_socket);
            return;
        	HAL_Delay(50);
        } else{
			(*uart_helper).printf("%d bytes sent!\r\n", nbytes);
			len -= nbytes;
        }
    }
}

void SocketClient::socket_send(const char *pData, uint16_t len){

    while(len > 0) {
        //int32_t nbytes = send(http_socket, (uint8_t*)pData, len);
        int32_t nbytes = sendto(http_socket, (uint8_t*)pData, len, addr, port);
        if(nbytes <= 0) {
        	(*uart_helper).printf("send() failed, %d returned\r\n", nbytes);
            close(http_socket);
            return;
        	HAL_Delay(50);
        } else{
			(*uart_helper).printf("%d bytes sent!\r\n", nbytes);
			len -= nbytes;
        }
    }
}

void SocketClient::socket_receive(uint8_t *pData, uint16_t Size, uint32_t* rdmaInd){
		int32_t nbytes = 0;
		uint16_t recvsize = getSn_RX_RSR(http_socket);
		if (recvsize > 0){
			//nbytes = recv(http_socket, pData, Size);
			nbytes = recvfrom(http_socket, pData, Size, addr, &port);
		}
		*rdmaInd = nbytes;


		if(nbytes < 0) {
			(*uart_helper).printf("\r\nrecv() failed, %d returned\r\n", nbytes);
			return;
		}
		if (nbytes > 0){
			(*uart_helper).printf("\r\nrecv() %d returned\r\n", nbytes);
			return;
		} else {
			(*uart_helper).printf("\r\nrecv() socket busy\r\n");
			return;
		}
}


void SocketClient::socket_close(){
	  close(http_socket);
	  (*uart_helper).printf("Closing socket.\r\n");
}

void SocketClient::socket_init(){
	chip->initChip();

    SocketClient::http_socket = HTTP_SOCKET;
    //uint8_t code = socket(SocketClient::http_socket, Sn_MR_TCP, 10888, SF_IO_NONBLOCK );
    uint8_t code = socket(SocketClient::http_socket, Sn_MR_UDP, 10888, SF_IO_NONBLOCK );

    if(code != SocketClient::http_socket) {
    	(*uart_helper).printf("socket() failed, code = %d\r\n", code);
        return;
    }

    (*uart_helper).printf("Socket created, connecting...\r\n");
}
