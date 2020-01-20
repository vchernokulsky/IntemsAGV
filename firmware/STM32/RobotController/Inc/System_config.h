#pragma once

/********************UART************************/
#define UART_BUF_SIZE 20
#define UART_QUEUE_LEN 25
#define UART_TRANSMIT_DELAY 10
/************************************************/

/******************* IBT-1 *********************/
#define GPIO_REN1 GPIOB
#define PIN_REN1 GPIO_PIN_14

#define GPIO_LEN1 GPIOB
#define PIN_LEN1 GPIO_PIN_13

#define CHANNEL1 TIM_CHANNEL_2
#define CHANNEL_REV1 TIM_CHANNEL_1
/**********************************************/

/******************* IBT-2 *********************/
#define GPIO_REN2 GPIOC
#define PIN_REN2 GPIO_PIN_6

#define GPIO_LEN2 GPIOB
#define PIN_LEN2 GPIO_PIN_2

#define CHANNEL2 TIM_CHANNEL_4
#define CHANNEL_REV2 TIM_CHANNEL_3
/**********************************************/
