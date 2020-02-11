#include <mainpp.h>
#include <string>
#include "SocketClient.h"
#include "SLMPRequestBuilder.h"
#include "SLMPPacket.h"

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

			const static uint16_t rbuflen = 128;
			uint8_t rbuf[rbuflen];
			uint16_t Size = 5;
			uint32_t* rdmaInd;
			  for(;;)
			  {
				  socket_client.socket_receive(rbuf, Size, rdmaInd);
				  uart_helper.printf(rbuf);
				  socket_client.socket_send(msg, msg_len);
				  osDelay(100);
			  }

//	vTaskDelete( NULL );
}

void setup(UART_HandleTypeDef *main_huart, SPI_HandleTypeDef *main_hspi1,
		TIM_HandleTypeDef *main_htim, TIM_HandleTypeDef *main_htim2,
		TIM_HandleTypeDef *main_encoder_htim1, TIM_HandleTypeDef *main_encoder_htim2)
{

	  huart = main_huart;
	  hspi = main_hspi1;
	  uart_helper.init(huart);

	  socket_client.init(hspi, &uart_helper);
	  HAL_Delay(1000 * 2);
	  socket_client.socket_connect();

	  packet = SLMPPacket();

	  packet.is_serial_no = Field<unsigned short, 2>(0x0050, true);
	  packet.is_serial_no.exist = true;

	  packet.request_dest_net_no = Field<unsigned char, 1>(0, true);
	  packet.request_dest_net_no.exist = true;

	  packet.request_dest_station_no = Field<unsigned char, 1>(0xff, true);
	  packet.request_dest_station_no.exist = true;

	  packet.request_processor = Field<unsigned short, 2>(0x3ff, true);
	  packet.request_processor.exist = true;

	  packet.request_reserved = Field<unsigned char, 1>(0x0, true);
	  packet.request_reserved.exist = true;

	  packet.data_length = Field<unsigned short, 2>(16, true);
	  packet.data_length.exist = true;

	  packet.monitoring_time = Field<unsigned short, 2>(1, true);
	  packet.monitoring_time.exist = true;

	  packet.command = Field<unsigned short, 2>(0x1401, true);
	  packet.command.exist = true;

	  packet.subcommand = Field<unsigned short, 2>(0, true);
	  packet.subcommand.exist = true;

	  packet.head_device_no = Field<unsigned int, 3>(100, true);
	  packet.head_device_no.exist = true;

	  packet.device_code = Field<unsigned char, 1>(0xa8, true);
	  packet.device_code.exist = true;

	  packet.no_of_device_points = Field<unsigned short, 2>(0x2, true);
	  packet.no_of_device_points.exist = true;

	  packet.value = ValueField({5, '\x00', 3, '\x00'});
	  packet.value.exist = true;

	  msg_len = getMsgLen(&packet);

	  //****** UART **********
	  osThreadDef(UartTask, StartUARTTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(UartTask), NULL);
//	  //****** Socket **********
//	  osThreadDef(SocketTask, StartSocketTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(SocketTask), NULL);

	  //========== ROS ===============
//

	  //****** UART Test ***************
	  osThreadDef(SecondTask, StartSecondTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(SecondTask), NULL);

	  //******* Socket Test *************
	  osThreadDef(SocketSendTask, StartSocketSendTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(SocketSendTask), NULL);

}

