/*
 * SocketHelper.h
 *
 *  Created on: Jan 31, 2020
 *      Author: developer
 */

#include "stdint.h"
#include "string.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"



#ifndef SOCKETSERVER_H_
#define SOCKETSERVER_H_

class SocketServer {
private:

	int local_sock;
	int remote_sock;

	int recv_data;
	int send_data;


	struct sockaddr_in localhost, remotehost;
	socklen_t sockaddrsize;

	uint8_t check_errno();
	void start_server();
public:
	SocketServer(uint16_t local_port);
	virtual ~SocketServer();

	void socket_receive(uint8_t *pData, uint16_t size, uint32_t* rdmaInd);
	void socket_send(uint8_t *pData, uint16_t len);
	void SocketServerTask();
};

#endif /* SOCKETHELPER_H_ */
