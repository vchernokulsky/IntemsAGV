/*
 * SocketHelper.h
 *
 *  Created on: Jan 31, 2020
 *      Author: developer
 */





#ifndef SOCKETSERVER_H_
#define SOCKETSERVER_H_

#include "stdint.h"
#include "string.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"



#define BUFFER_SIZE 30

class SocketServer {
private:
	static SetUpHelper *settings;

	int local_sock;
	int remote_sock;

	int recv_data;
	int send_data;

	uint8_t send_buffer[BUFFER_SIZE];
	uint8_t recv_buffer[BUFFER_SIZE];


	struct sockaddr_in localhost, remotehost;
	socklen_t sockaddrsize;

	uint8_t check_errno();
	void start_server();
public:
	SocketServer();
	SocketServer(uint16_t local_port, SetUpHelper *main_settings);
	virtual ~SocketServer();

	void init(uint16_t local_port, SetUpHelper *main_settings);
	void socket_receive(uint8_t *pData, uint16_t size, uint32_t* rdmaInd);
	void socket_send(uint8_t *pData, uint16_t len);
	void SocketServerTask();
};

#endif /* SOCKETHELPER_H_ */
