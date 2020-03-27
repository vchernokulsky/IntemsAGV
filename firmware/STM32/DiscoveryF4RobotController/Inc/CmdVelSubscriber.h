#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <geometry_msgs/Twist.h>

#include "WheelSubscriber.h"
#include "Default_settings.h"

class CmdVelSubscriber
{
private:
	ros::Subscriber<geometry_msgs::Twist,CmdVelSubscriber> sub;
	float wheel_separation;
	float max_lin_speed;
	float max_ang_vel;

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
    	float ret = constrain(vel, -max_lin_speed, max_lin_speed);
        return ret;
    }
    float check_angular_limit_velocity(float vel){
    	float ret = constrain(vel, -max_ang_vel, max_ang_vel);
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
    	if ((left_linear > max_lin_speed)or(right_linear > max_lin_speed)){
            float factor = max_lin_speed / max(left_linear, right_linear);
            left_linear *= factor;
			right_linear *= factor;
    	}
    	if ((left_linear < -max_lin_speed)or(right_linear < -max_lin_speed)){
			float factor = max_lin_speed / min(left_linear, right_linear);
			left_linear *= factor;
			right_linear *= factor;
		}
    }
    void calculate_speeds(){
		left_linear = (2 * linear + wheel_separation * angular) / 2;
		right_linear = (2 * linear - wheel_separation * angular) / 2;
		adjust_speeds();
    }
public:

	void cmdvel_callback(const geometry_msgs::Twist& msg){
		last_time = HAL_GetTick();
		linear = check_linear_limit_velocity(msg.linear.x);
		angular = check_angular_limit_velocity(msg.angular.z);
		calculate_speeds();
		left_wheel->wheel_callback(left_linear);
		right_wheel->wheel_callback(right_linear);

	}

	void check_timeout(){
		cur_time = HAL_GetTick();
		delta_time = cur_time - last_time;
		if(delta_time >  CMDVEL_TIMEOUT){
			left_wheel->wheel_callback(0.0);
			right_wheel->wheel_callback(0.0);
		}

	}
	CmdVelSubscriber():sub("",&CmdVelSubscriber::cmdvel_callback, this){

	}


	void init(ros::NodeHandle* nh, WheelSubscriber *wheel, WheelSubscriber *wheel2, char *topic_name){
		sub.topic_ = topic_name;
		(*nh).subscribe(sub);
		left_wheel = wheel;
		right_wheel = wheel2;
		cur_time = HAL_GetTick();
		last_time = cur_time;
	}

	void set_robot_params(float separation, float max_lin, float max_ang){
			wheel_separation = separation;
			max_lin_speed = max_lin;
			max_ang_vel = max_ang;
		}





};
