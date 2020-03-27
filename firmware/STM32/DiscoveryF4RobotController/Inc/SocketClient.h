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

#include "Default_settings.h"
#include "System_config.h"


#ifndef SOCKETCLIENT_H_
#define SOCKETCLIENT_H_

#define OK_STATUS 0
#define WARNING_STATUS 1
#define ERROR_STATUS 2
#define UNKNOWN_STATUS 3

#define MAX_ERROR_COUNT 100



class SocketClient {
private:


	int sock;
	int recv_data;
	int send_data;
	int err_count;

	struct sockaddr_in localhost, remotehost;
	socklen_t sockaddrsize;

	SemaphoreHandle_t send_semaphore;

	uint8_t check_errno();
	uint8_t check_errno(int bytes);
public:
	static SemaphoreHandle_t error_semaphore;
	static bool is_connected;
	SocketClient();
	virtual ~SocketClient();
	void init(uint16_t ros_local_port, uint8_t *remote_ip, uint16_t ros_serialnode_port);
	void socket_receive(uint8_t *pData, uint16_t size, uint32_t* rdmaInd);
	void socket_send(uint8_t *pData, uint16_t len);
	void SocketClientTask();
};

#endif /* SOCKETHELPER_H_ */
