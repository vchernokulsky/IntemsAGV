#include "mainpp.h"
#include "SocketClient.h"
#include "SocketServer.h"
#include "UartHelper.h"

UartHelper uart_helper;

static __attribute__ ((used,section(".user_heap_stack"))) uint8_t heap_sram1[32*1024];
uint8_t heap_sram2[32*1024];
uint8_t heap_sram3[32*1024];

void StartSocetClientTask(void *arg);
void StartSocetServerTask(void *arg);
void StartUARTTask(void *arg);
void StartSecondTask(void *arg);

/***** CALL IN 'USER CODE BEGIN(END) 1' *****/
void memory_setup()
{
	  HeapRegion_t xHeapRegions[] =
	  {
	      {  heap_sram1, sizeof(heap_sram1) },
		  {  heap_sram2, sizeof(heap_sram2) },
		  {  heap_sram3, sizeof(heap_sram3) },
	      { NULL, 0 }
	  };
	  vPortDefineHeapRegions( xHeapRegions );
}
/********************************************/

/*************** CALL IN 'USER CODE BEGIN(END) 5' ***************/
void threds_setup(UART_HandleTypeDef *main_huart)
{

	memset(&uart_helper, 0, sizeof(struct UartHelper));
//	uart_helper = new UartHelper();
	uart_helper.init(main_huart);
	//****** Client Task **********
	sys_thread_new("client_thread", StartSocetClientTask, 0, DEFAULT_THREAD_STACKSIZE * 2, osPriorityNormal);
	sys_thread_new("server_thread", StartSocetServerTask, 0, DEFAULT_THREAD_STACKSIZE * 2, osPriorityNormal);
	sys_thread_new("uart_thread", StartUARTTask, 0, DEFAULT_THREAD_STACKSIZE * 2, osPriorityNormal);
	sys_thread_new("uart_test_thread", StartSecondTask, 0, 256, osPriorityNormal);
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
void StartUARTTask(void *arg)
{
	uart_helper.UARTTask();
}

void StartSecondTask(void *arg)
{
  for(;;)
  {
	  uint8_t buff[] = "!!!!!!!!!!!!!!!!\n\0";
	  uart_helper.printf(buff);
	  osDelay(500);
  }
}
