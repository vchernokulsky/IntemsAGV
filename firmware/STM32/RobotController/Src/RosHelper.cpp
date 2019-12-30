/*
 * RosHelper.cpp
 *
 *  Created on: Dec 17, 2019
 *      Author: developer
 */

#include "RosHelper.h"

RosHelper::RosHelper():chatter("chatter",&str_msg) {
	// TODO Auto-generated constructor stub

}

RosHelper::~RosHelper() {
	// TODO Auto-generated destructor stub
}

void RosHelper::setupRos(UartHelper *uart_helper, TIM_HandleTypeDef *main_htim,  TIM_HandleTypeDef *main_htim2)
{
  nh.initNode();
  nh.advertise(chatter);
  nh.setUARTHelper(uart_helper);

  	wheel = new WheelSubscriber("my_robot/left_wheel_vel");
	wheel->set_ports(GPIOA, GPIOB, GPIOA, GPIOA);
	wheel->set_pins(GPIO_PIN_4, GPIO_PIN_0, GPIO_PIN_0, GPIO_PIN_1);
	wheel->subscribe(&nh, main_htim, TIM_CHANNEL_1, TIM_CHANNEL_2);

  wheel2 = new WheelSubscriber("my_robot/right_wheel_vel");
  wheel2->set_ports(GPIOA, GPIOB, GPIOB, GPIOB);
  wheel2->set_pins(GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_4, GPIO_PIN_5);
  wheel2->subscribe(&nh, main_htim2, TIM_CHANNEL_1, TIM_CHANNEL_2);

  encoder = new WheelPublisher(&nh, "/my_robot/left_wheel_angle");
  encoder2 = new WheelPublisher(&nh, "/my_robot/right_wheel_angle");

}


void RosHelper::rosLoop(void)
{
  str_msg.data = "Hello world!";
  chatter.publish(&str_msg);
  encoder->publish();
  encoder2->publish();
  nh.spinOnce();
  osDelay(500);
}

void RosHelper::RosTask(void){

	for(;;){
		rosLoop();
	}
}

void RosHelper::setSpeedTask(void){
	for(;;){
		wheel->set_speed();
		osDelay(5);
	}
}
void RosHelper::setSpeedTask2(void){
	for(;;){
		wheel2->set_speed();
		osDelay(5);
	}
}
void RosHelper::readSpeedTask(void){
	for(;;){
		encoder->calculate_ang();
		osDelay(5);
	}
}
void RosHelper::readSpeedTask2(void){
	for(;;){
		encoder2->calculate_ang();
		osDelay(5);
	}
}

void RosHelper::exti_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_4 || GPIO_Pin == GPIO_PIN_10)
	{
	  uint8_t gray_code = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4) | HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10) << 1;
//	  encoder->push(gray_code);
	  encoder->calculate_ang(gray_code);
	  return;
	}
  if (GPIO_Pin == GPIO_PIN_3 || GPIO_Pin == GPIO_PIN_5)
	{
	  uint8_t gray_code = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) | HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5) << 1;
//	  encoder2->push(gray_code);
	  encoder2->calculate_ang(gray_code);
	  return;
	}
  else
	{
	  __NOP();

	}
}

