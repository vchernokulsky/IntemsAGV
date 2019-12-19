#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <std_msgs/Float32.h>


class WheelPublisher
{
private:
	GPIO_TypeDef* port;
	uint16_t pin;

	std_msgs::Float32 float_msg;
	ros::Publisher pub;

	uint16_t tick;

public:
	ros::NodeHandle* nh;


	WheelPublisher(ros::NodeHandle* n,char topic_out[],
			GPIO_TypeDef* gpio_in,uint16_t pin_num_in):pub(topic_out,&float_msg){

		nh = n;
		port = gpio_in;
		pin = pin_num_in;
		(*nh).advertise(pub);
		tick = 0;
	}

	void loop(){

		if (!HAL_GPIO_ReadPin(port, pin)) {	                // checks if PA0 is set
			       tick++;      // switch on LED6
		}
		float_msg.data = tick;

		pub.publish(&float_msg);


	}

};
