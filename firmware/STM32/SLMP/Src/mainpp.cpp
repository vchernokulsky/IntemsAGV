#include <mainpp.h>
#include <string>
#include "SocketClient.h"
#include "SLMPRequestBuilder.h"
#include "SLMPResponseParser.h"
#include "SLMPPacket.h"
#include "PacketFields.h"
#include "LiquidCristal.h"
#include "overrided.h"

UART_HandleTypeDef *huart;
SPI_HandleTypeDef *hspi;
UartHelper uart_helper;
SocketClient socket_client;
SLMPPacket packet;
unsigned int msg_len = 0;


void StartSecondTask(void const * argument)
{
  for(;;)
  {
	  uart_helper.printf("!!!!!!!!!!!!!!!!\n\0");
	  osDelay(500);
  }
}


void StartUARTTask(void const * argument)
{
	uart_helper.UARTTask();
}

void StartSocketSendTask(void const * argument)
{
		unsigned char msg[msg_len];
		buildRequest(&packet, msg);

			const static uint16_t rbuflen = 30;
			static float a_before = 0;

			uint8_t rbuf[rbuflen];
			uint16_t Size = 30;
			uint32_t* rdmaInd;
			static SLMPPacket packet1;
			  for(;;)
			  {
				  socket_client.socket_send(msg, msg_len);
				  osDelay(1000);
				  socket_client.socket_receive(rbuf, Size, rdmaInd);
				  parseResponse(&packet1, rbuf);
				  std::vector<unsigned char> v = packet1.value.getValue();

				  Field<float, 4> res_val;

				  unsigned char * begin = &packet1.value.getValue().front();
				  res_val.getFromDump(begin);
				  float a = res_val.getValue();

				  if  (a != a_before) {
					  a_before = a;
					  clear();
					  print(float_to_std_string(a).c_str());
				  }
				  osDelay(100);
			  }

//	vTaskDelete( NULL );
}

void prepareSLMPRequest(void) {
	packet = SLMPPacket();

		  packet.is_serial_no = Field<unsigned short, 2>(0x0050);
		  packet.is_serial_no.exist = true;

		  packet.request_dest_net_no = Field<unsigned char, 1>(0x0);
		  packet.request_dest_net_no.exist = true;

		  packet.request_dest_station_no = Field<unsigned char, 1>(0xff);
		  packet.request_dest_station_no.exist = true;

		  packet.request_processor = Field<unsigned short, 2>(0x3ff);
		  packet.request_processor.exist = true;

		  packet.request_reserved = Field<unsigned char, 1>(0x0);
		  packet.request_reserved.exist = true;

		  packet.monitoring_time = Field<unsigned short, 2>(0x1);
		  packet.monitoring_time.exist = true;

		  packet.command = Field<unsigned short, 2>(0x0401);
		  //packet.command = Field<unsigned short, 2>(0x1401);
		  packet.command.exist = true;

		  packet.subcommand = Field<unsigned short, 2>(0x0);
		  packet.subcommand.exist = true;

		  packet.head_device_no = Field<unsigned int, 3>(100);
		  packet.head_device_no.exist = true;

		  packet.device_code = Field<unsigned char, 1>(0xa8);
		  packet.device_code.exist = true;

		  packet.no_of_device_points = Field<unsigned short, 2>(0x2);
		  packet.no_of_device_points.exist = true;

//		  packet.value = ValueField({12, '\x00', 14, '\x00'});
//		  packet.value.exist = true;

		  msg_len = getMsgLen(&packet);
}

void setup(UART_HandleTypeDef *main_huart, SPI_HandleTypeDef *main_hspi1,
		TIM_HandleTypeDef *main_htim, TIM_HandleTypeDef *main_htim2,
		TIM_HandleTypeDef *main_encoder_htim1, TIM_HandleTypeDef *main_encoder_htim2)
{
	LiquidCrystal(GPIOC, GPIO_PIN_0, GPIO_PIN_12, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5);
	  huart = main_huart;
	  hspi = main_hspi1;
	  uart_helper.init(huart);

	  socket_client.init(hspi, &uart_helper);
	  HAL_Delay(1000 * 2);
	  socket_client.socket_connect();

	  prepareSLMPRequest();
	  //****** UART **********
	  osThreadDef(UartTask, StartUARTTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(UartTask), NULL);
//	  //****** Socket **********
//	  osThreadDef(SocketTask, StartSocketTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(SocketTask), NULL);

	  //========== ROS ===============
//

	  //****** UART Test ***************
//	  osThreadDef(SecondTask, StartSecondTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(SecondTask), NULL);

	  //******* Socket Test *************
	  osThreadDef(SocketSendTask, StartSocketSendTask, osPriorityNormal, 1, 2048);
	  osThreadCreate(osThread(SocketSendTask), NULL);
}

//void * operator new( size_t size )
//{
//    return pvPortMalloc( size );
//}
//
//void * operator new[]( size_t size )
//{
//    return pvPortMalloc(size);
//}
//
//void operator delete( void * ptr )
//{
//    vPortFree ( ptr );
//}
//
//void operator delete[]( void * ptr )
//{
//    vPortFree ( ptr );
//}
