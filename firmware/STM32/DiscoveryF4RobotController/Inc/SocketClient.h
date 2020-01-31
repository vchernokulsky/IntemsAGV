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



#ifndef SOCKETCLIENT_H_
#define SOCKETCLIENT_H_

#define OK_STATUS 0
#define WARNING_STATUS 1
#define ERROR_STATUS 2
#define UNKNOWN_STATUS 3

#define MAX_ERROR_COUNT 10



class SocketClient {
private:

	int sock;
	int recv_data;
	int send_data;
	int err_count;

	struct sockaddr_in localhost, remotehost;
	socklen_t sockaddrsize;

	uint8_t check_errno();
public:
	SocketClient(uint16_t local_port, const char *remote_ip,  uint16_t remote_port);
	virtual ~SocketClient();

	void socket_receive(uint8_t *pData, uint16_t size, uint32_t* rdmaInd);
	void socket_send(uint8_t *pData, uint16_t len);
	void SocketClientTask();
};

#endif /* SOCKETHELPER_H_ */
