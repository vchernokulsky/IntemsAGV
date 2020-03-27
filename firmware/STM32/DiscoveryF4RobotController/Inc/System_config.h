#pragma once

#define IP_SIZE 4
#define PORT_SIZE 2

/********************UART************************/
#define UART_BUF_SIZE 30
#define UART_QUEUE_LEN 25
#define UART_TRANSMIT_DELAY 5
/************************************************/


/******************* IBT-1 *********************/
#define GPIO_REN1 GPIOA
#define PIN_REN1 GPIO_PIN_15

#define GPIO_LEN1 GPIOA
#define PIN_LEN1 GPIO_PIN_8

#define CHANNEL1 TIM_CHANNEL_3
#define CHANNEL_REV1 TIM_CHANNEL_4
/**********************************************/

/******************* IBT-2 *********************/
#define GPIO_REN2 GPIOD
#define PIN_REN2 GPIO_PIN_12

#define GPIO_LEN2 GPIOD
#define PIN_LEN2 GPIO_PIN_11

#define CHANNEL2 TIM_CHANNEL_2
#define CHANNEL_REV2 TIM_CHANNEL_3
/**********************************************/

/********************LED************************/
/* HardFault LED blinks while everything is ok
 *  and stops when hardFault happens.
 * Run as last Task */
#define GPIO_HARDFAULT_LED GPIOD
#define PIN_HARDFAULT_LED USR_LED_Pin

/* EEPROM LED turns off when the status is not HAL_OK  */
#define GPIO_EEPROM_LED GPIOD
#define PIN_EEPROM_LED GPIO_PIN_10

/* LWIP LED turns on when the connection is successful
 * otherwise it blinks (turns off when the number of errors
 * exceeds the specified threshold)	  */
#define GPIO_LWIP_LED GPIOD
#define PIN_LWIP_LED GPIO_PIN_9
/************************************************/

