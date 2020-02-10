/*
 * SocketHelper.cpp
 *
 *  Created on: Jan 31, 2020
 *      Author: developer
 */

#include "SocketServer.h"

SocketServer::SocketServer(uint16_t local_port)
{
	// TODO Auto-generated constructor stub

	memset(&localhost, 0, sizeof(struct sockaddr_in));
	localhost.sin_family = AF_INET;
	localhost.sin_port = htons(local_port);
	localhost.sin_addr.s_addr = INADDR_ANY;
	osDelay(100);

}

SocketServer::~SocketServer() {
	// TODO Auto-generated destructor stub
}

void SocketServer::socket_receive(uint8_t *pData, uint16_t size, uint32_t* rdmaInd)
{

	recv_data = recv(remote_sock, pData, size, 0);
	*rdmaInd = (recv_data > 0)? recv_data : 0;


}

void SocketServer::socket_send(uint8_t *pData, uint16_t len)
{
	send_data = write(remote_sock,(void *) pData, len);

}

void SocketServer::start_server()
{
	for(;;)
	{
		if ((local_sock = socket(AF_INET,SOCK_STREAM, 0)) >= 0)
		{
			osDelay(100);
			if (bind(local_sock, (struct sockaddr *)&localhost, sizeof(struct sockaddr_in)) ==  0)
			{
				listen(local_sock, 1);
				break;
			}
		}
	}
}

void SocketServer::SocketServerTask()
{
	uint8_t data_buffer[30] = {};
	uint8_t buff[] = {1,2,3,4,5};
	uint32_t rdmaInd;
	for(;;)
	{
		start_server();

		for(;;)
		{
			remote_sock = accept(local_sock, (struct sockaddr *)&remotehost, (socklen_t *)&sockaddrsize);
			if(remote_sock >= 0)
			{
				socket_receive(data_buffer, 30,&rdmaInd);
				socket_send(buff, 6);
				osDelay(100);
			}
			close(remote_sock);
		}
		close(local_sock);
		osDelay(1000);
	}
}






