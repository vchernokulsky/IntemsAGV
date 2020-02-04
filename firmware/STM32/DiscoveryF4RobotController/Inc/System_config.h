#pragma once

/********************UART************************/
#define UART_BUF_SIZE 30
#define UART_QUEUE_LEN 25
#define UART_TRANSMIT_DELAY 5
/************************************************/


/******************* IBT-1 *********************/
#define GPIO_REN1 GPIOD
#define PIN_REN1 GPIO_PIN_11

#define GPIO_LEN1 GPIOD
#define PIN_LEN1 GPIO_PIN_10

#define CHANNEL1 TIM_CHANNEL_3
#define CHANNEL_REV1 TIM_CHANNEL_4
/**********************************************/

/******************* IBT-2 *********************/
#define GPIO_REN2 GPIOA
#define PIN_REN2 GPIO_PIN_8

#define GPIO_LEN2 GPIOA
#define PIN_LEN2 GPIO_PIN_15

#define CHANNEL2 TIM_CHANNEL_3
#define CHANNEL_REV2 TIM_CHANNEL_4
/**********************************************/
