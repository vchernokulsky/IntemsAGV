#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <std_msgs/Float32.h>

#define RAD_PER_TICK 0.0174533


class WheelPublisher
{
private:
	xQueueHandle q;
	ros::NodeHandle* nh;
	std_msgs::Float32 float_msg;
	ros::Publisher pub;

	uint32_t prev_tick;
	uint32_t cur_tick;

	uint8_t previous_code;

	unsigned graydecode(unsigned gray)
	{
	 unsigned bin;
	 for (bin = 0; gray; gray >>= 1)
	    bin ^= gray;
	 return bin;
	}


public:

	WheelPublisher(ros::NodeHandle* n, char topic_out[]):pub(topic_out,&float_msg){
		q = xQueueCreate( 64, sizeof( uint8_t ) );
		nh = n;
		(*nh).advertise(pub);

		prev_tick = 0;
		cur_tick = 0;
		previous_code = 0;
	}

	void publish(){
		float_msg.data = ((float)cur_tick - (float)prev_tick) * RAD_PER_TICK;
//		float_msg.data = (float)cur_tick  * RAD_PER_TICK;
		prev_tick = cur_tick;
		pub.publish(&float_msg);
	}

	void push(uint8_t gray_code){
		uint8_t code = gray_code;
		xQueueSend( q, ( void * ) &code, portMAX_DELAY  );
	}

	void calculate_ang(void){
		uint8_t gray_code;
		xQueueReceive( q, &( gray_code ), portMAX_DELAY );
		unsigned code = graydecode(gray_code);
		if (code == 0)
		{
		   if (previous_code == 3){
//		     Serial.println("->");
			 cur_tick += 1;
		   }
		   else if (previous_code == 1)
//		     Serial.println("<-");
			 cur_tick -= 1;
		 }
		 previous_code = code;
	}

	void calculate_ang(uint8_t gray_code){
			unsigned code = graydecode(gray_code);
			if (code == 0)
			{
			   if (previous_code == 3){
	//		     Serial.println("->");
				 cur_tick += 1;
			   }
			   else if (previous_code == 1)
	//		     Serial.println("<-");
				 cur_tick -= 1;
			 }
			 previous_code = code;
		}



};
