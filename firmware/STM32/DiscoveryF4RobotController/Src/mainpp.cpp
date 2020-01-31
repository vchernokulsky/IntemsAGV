#include "mainpp.h"
#include "SocketClient.h"
#include "SocketServer.h"

static __attribute__ ((used,section(".user_heap_stack"))) uint8_t heap_sram1[32*1024];
uint8_t heap_sram2[32*1024];


void StartSocetServerTask(void *arg);
/***** CALL IN 'USER CODE BEGIN(END) 1' *****/
void memory_setup()
{
	  HeapRegion_t xHeapRegions[] =
	  {
	      {  heap_sram1, sizeof(heap_sram1) },
		  {  heap_sram2, sizeof(heap_sram2) },
	      { NULL, 0 }
	  };
	  vPortDefineHeapRegions( xHeapRegions );
}
/********************************************/

/*************** CALL IN 'USER CODE BEGIN(END) 5' ***************/
void threds_setup()
{
	//****** Client Task **********
	sys_thread_new("client_thread", StartSocetClientTask, 0, DEFAULT_THREAD_STACKSIZE * 2, osPriorityNormal);
	sys_thread_new("server_thread", StartSocetServerTask, 0, DEFAULT_THREAD_STACKSIZE * 2, osPriorityNormal);
}
/***************************************************************************/

void StartSocetClientTask(void *arg)
{
	SocketClient client(10888, "192.168.55.10", 11511);
	client.SocketClientTask();
}

void StartSocetServerTask(void *arg)
{
	SocketServer server(11511);
	server.SocketServerTask();
}
