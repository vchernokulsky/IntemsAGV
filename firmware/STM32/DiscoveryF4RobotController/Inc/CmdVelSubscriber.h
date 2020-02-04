#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <geometry_msgs/Twist.h>

#include "WheelSubscriber.h"
#include "User_config.h"

class CmdVelSubscriber
{
private:
	ros::Subscriber<geometry_msgs::Twist,CmdVelSubscriber> sub;

	float linear;
	float angular;

	float left_linear;
	float right_linear;

	uint32_t cur_time;
	uint32_t last_time;
	uint32_t delta_time;

	WheelSubscriber *left_wheel;
	WheelSubscriber *right_wheel;

    float constrain(float vel, float low, float high)
    {
    	float ret = vel;
        if (ret < low){
        	ret = low;
        }
        if (ret > high){
        	ret = high;
        }
        return ret;
    }
    float check_linear_limit_velocity(float vel)
    {
    	float ret = constrain(vel, MIN_LIN_SPEED, MAX_LIN_SPEED);
        return ret;
    }
    float check_angular_limit_velocity(float vel){
    	float ret = constrain(vel, ANG_VEL_MIN, ANG_VEL_MAX);
        return ret;
    }

    float max(float a, float b){
    	if (a > b){
    		return a;
    	} else {
    		return b;
    	}
    }
    float min(float a, float b){
    	if (a < b){
    		return a;
    	} else {
    		return b;
    	}
    }
    void adjust_speeds()
    {
    	if ((left_linear > MAX_LIN_SPEED)or(right_linear > MAX_LIN_SPEED)){
            float factor = MAX_LIN_SPEED / max(left_linear, right_linear);
            left_linear *= factor;
			right_linear *= factor;
    	}
    	if ((left_linear < -MAX_LIN_SPEED)or(right_linear < -MAX_LIN_SPEED)){
			float factor = MAX_LIN_SPEED / min(left_linear, right_linear);
			left_linear *= factor;
			right_linear *= factor;
		}
    }
    void calculate_speeds(){
    	cur_time = HAL_GetTick();
    	delta_time = cur_time - last_time;
    	if(delta_time <  CMDVEL_TIMEOUT)
    	{
			left_linear = (2 * linear + WHEEL_SEPARATION * angular) / 2;
			right_linear = (2 * linear - WHEEL_SEPARATION * angular) / 2;
			adjust_speeds();

    	}
    	else
    	{
    		left_linear = 0;
    		right_linear = 0;
    	}
    	last_time = cur_time;
    }
public:

	void cmdvel_callback(const geometry_msgs::Twist& msg){
		linear = check_linear_limit_velocity(msg.linear.x);
		angular = check_angular_limit_velocity(msg.angular.z);
		calculate_speeds();
		left_wheel->wheel_callback(left_linear);
		right_wheel->wheel_callback(right_linear);

	}
	CmdVelSubscriber():sub(CMDVEL_TOPIC,&CmdVelSubscriber::cmdvel_callback, this){

	}

	CmdVelSubscriber(ros::NodeHandle* nh, WheelSubscriber *wheel, WheelSubscriber *wheel2):sub(CMDVEL_TOPIC,&CmdVelSubscriber::cmdvel_callback, this){
		(*nh).subscribe(sub);
		left_wheel = wheel;
		right_wheel = wheel2;
		cur_time = HAL_GetTick();
		last_time = cur_time;
	}

	void init(ros::NodeHandle* nh, WheelSubscriber *wheel, WheelSubscriber *wheel2){
		(*nh).subscribe(sub);
		left_wheel = wheel;
		right_wheel = wheel2;
		cur_time = HAL_GetTick();
		last_time = cur_time;
	}





};
