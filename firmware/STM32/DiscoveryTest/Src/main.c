/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"
#include "string.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
osThreadId defaultTaskHandle;
/* USER CODE BEGIN PV */
static __attribute__ ((used,section(".user_heap_stack"))) uint8_t heap_sram1[32*1024];

osThreadId ThreadRecvHandle;

typedef struct struct_sock_t {
  uint16_t y_pos;
  uint16_t port;
} struct_sock;
struct_sock sock01;

typedef struct struct_recv_socket_t {
  int sock;
  uint16_t y_pos;
} struct_recv_socket;
struct_recv_socket recv_socket01;

typedef struct struct_client_socket_t {
  struct sockaddr_in remotehost;
  socklen_t sockaddrsize;
  int accept_sock;
  uint16_t y_pos;
} struct_client_socket;
struct_client_socket client_socket01;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartDefaultTask(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	  HeapRegion_t xHeapRegions[] =
	  {
	      {  heap_sram1, sizeof(heap_sram1) },
	      { NULL, 0 }
	  };
	  vPortDefineHeapRegions( xHeapRegions );
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Audio_RST_GPIO_Port, Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Audio_RST_Pin */
  GPIO_InitStruct.Pin = Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Audio_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
//---------------------------------------------------------------
static void recv_thread(void *arg)
{
//  struct_out *qstruct;
  struct_recv_socket *arg_recv_socket;
  arg_recv_socket = (struct_recv_socket*) arg;
  int recv_data;
  char data_buffer[30] = {};
  for(;;)
  {
    recv_data = recv(arg_recv_socket->sock,data_buffer,sizeof(data_buffer),0);
    if(recv_data > 0)
    {
    	//====DATA RECEIVED====
      data_buffer[recv_data-1] = 0;
//      qstruct = osMailAlloc(strout_Queue, osWaitForever);
//      sprintf(qstruct->str,"%-20s", (char*)data_buffer);
//      qstruct->y_pos = arg_recv_socket->y_pos;
//      qstruct->x_pos = 10;
//      qstruct->sfont = Font24;
//      osMailPut(strout_Queue, qstruct);
    }
  }
}

static void recv_func_c(int sock)
{
//  struct_out *qstruct;

	int recv_data;
	char data_buffer[30] = {};

	recv_data = recv(sock,data_buffer,sizeof(data_buffer),0);
	if(recv_data > 0)
	{
		//====DATA RECEIVED====
	  data_buffer[recv_data-1] = 0;
	}

}
static void recv_func_s(struct_client_socket *arg_client_socket)
{
	char out_buffer[30] = {};
	  int buflen = 30;
	  int ret, accept_sock;
	  struct sockaddr_in remotehost;
	  socklen_t sockaddrsize;
	  remotehost = arg_client_socket->remotehost;
	  sockaddrsize  = arg_client_socket->sockaddrsize;
	  accept_sock = arg_client_socket->accept_sock;

	    ret = recvfrom( accept_sock,out_buffer, buflen, 0, (struct sockaddr *)&remotehost, &sockaddrsize);

	    if(ret > 0)
	    	{
	    		//====DATA RECEIVED====
	    	out_buffer[ret-1] = 0;
	    	}
}
//---------------------------------------------------------------
static void client_thread(void *arg)
{

  int sock;
  int ret;
  struct sockaddr_in localhost, remotehost;
  uint32_t syscnt = 0;
  char buf[] = "12345678901234567890123456789012345678901234567890"
		  "12345678901234567890123456789012345678901234567890"
		  "12345678901234567890123456789012345678901234567890"
		  "12345678901234567890123456789012345678901234567890"
		  "12345678901234567890123456789012345678901234567890"
		  "12345678901234567890123456789012345678901234567890";

  memset(&localhost, 0, sizeof(struct sockaddr_in));
  localhost.sin_family = AF_INET;
  localhost.sin_port = htons(11411);
  localhost.sin_addr.s_addr = INADDR_ANY;

  memset(&remotehost, 0, sizeof(struct sockaddr_in));
  remotehost.sin_family = AF_INET;
  remotehost.sin_port = htons(11511);

  ip4addr_aton("192.168.55.10",(ip4_addr_t*)&remotehost.sin_addr);

  osDelay(100);
  for(;;){
  if ((sock = socket(AF_INET,SOCK_STREAM, 0)) >= 0)
  {

	  osDelay(100);
//    if (bind(sock, (struct sockaddr *)&localhost, sizeof(struct sockaddr_in)) ==  0)
//    {
//
//    	osDelay(100);
		  if (connect(sock, (struct sockaddr *)&remotehost,sizeof(struct sockaddr_in)) >= 0)
		  {

			for(;;){
			recv_func_c(sock);

			ret = write(sock,(void *) buf,strlen(buf));
			osDelay(500);
			}
//			close(sock);
//			osDelay(1000);

		  }

//    }

    }
  close(sock);
  osDelay(1000);
  }
}
//---------------------------------------------------------------
//---------------------------------------------------------------
static void server_thread(void *arg)
{

  int sock, accept_sock;
  struct sockaddr_in localhost, remotehost;
  socklen_t sockaddrsize;

  char buf[] = "12345678901234567890123456789012345678901234567890"
		  "12345678901234567890123456789012345678901234567890"
		  "12345678901234567890123456789012345678901234567890"
		  "12345678901234567890123456789012345678901234567890"
		  "12345678901234567890123456789012345678901234567890"
		  "12345678901234567890123456789012345678901234567890";

  memset(&localhost, 0, sizeof(struct sockaddr_in));
  localhost.sin_family = AF_INET;
  localhost.sin_port = htons(11511);
  localhost.sin_addr.s_addr = INADDR_ANY;

  osDelay(100);

	  if ((sock = socket(AF_INET,SOCK_STREAM, 0)) >= 0)
	  {
		osDelay(100);
		if (bind(sock, (struct sockaddr *)&localhost, sizeof(struct sockaddr_in)) ==  0)
		{

			listen(sock, 5);
			for(;;)
			{
				accept_sock = accept(sock, (struct sockaddr *)&remotehost, (socklen_t *)&sockaddrsize);
				if(accept_sock >= 0)
				{
					recv_func_c(accept_sock);
					write(accept_sock,(void *) buf,strlen(buf));
					osDelay(500);
				}
				int ret = close(accept_sock);
			}


		}
	  int ret = close(accept_sock);
	  osDelay(1000);
	  }
}
//---------------------------------------------------------------


/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
    
    
                 
  /* init code for LWIP */
  MX_LWIP_Init();

  /* USER CODE BEGIN 5 */
  sys_thread_new("client_thread", client_thread, 0, DEFAULT_THREAD_STACKSIZE * 2, osPriorityNormal);
  sys_thread_new("server_thread", server_thread, 0, DEFAULT_THREAD_STACKSIZE * 2, osPriorityNormal);
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */ 
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
