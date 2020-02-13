#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <nav_msgs/Odometry.h>
#include "ros/time.h"
#include "tf/tf.h"
#include "tf/transform_broadcaster.h"
#include "geometry_msgs/TransformStamped.h"

#include "WheelPublisher.h"

#define PI 3.14

class OdometryPublisher
{
private:
	ros::NodeHandle* nh;
	nav_msgs::Odometry odom;
	ros::Publisher pub;
	tf::TransformBroadcaster tf_broadcaster;
	geometry_msgs::TransformStamped transform;


	WheelPublisher *encoder1 = nullptr;
	WheelPublisher *encoder2 = nullptr;

	ros::Time last_time;
	ros::Time cur_time;

	float theta;

public:
	OdometryPublisher():pub(ODOMETRY_TOPIC,&odom){

	}

	OdometryPublisher(ros::NodeHandle* n, WheelPublisher *leftWheel, WheelPublisher *rightWheel):pub(ODOMETRY_TOPIC,&odom){
		nh = n;
		(*nh).advertise(pub);

		encoder1 = leftWheel;
		encoder2 = rightWheel;


		cur_time = nh->now();
		last_time = cur_time;

		theta = 0;

		odom.header.frame_id = ODOMETRY_FRAME;
		odom.child_frame_id = BASE_FRAME;

		tf_broadcaster.init(*nh);
		transform.header.frame_id =ODOMETRY_FRAME;
		transform.child_frame_id = BASE_FRAME;
	}

	void init(ros::NodeHandle* n, WheelPublisher *leftWheel, WheelPublisher *rightWheel){
			nh = n;
			(*nh).advertise(pub);

			encoder1 = leftWheel;
			encoder2 = rightWheel;


			cur_time = nh->now();
			last_time = cur_time;

			theta = 0;

			odom.header.frame_id = ODOMETRY_FRAME;
			odom.child_frame_id = BASE_FRAME;

			tf_broadcaster.init(*nh);
			transform.header.frame_id =ODOMETRY_FRAME ;
			transform.child_frame_id = BASE_FRAME;
		}



	void set_pose(){

		float left_travel = encoder1->get_distance() * RADIUS;
		float right_travel = encoder2->get_distance() * RADIUS;

//		double delta_time = cur_time.toSec() - last_time.toSec();
		double delta_time = (encoder1->get_distance_time() + encoder2->get_distance_time()) / 2000.0;

		float pose_x =   odom.pose.pose.position.x;
		float pose_y =   odom.pose.pose.position.y;

		float x_vel = 0;
		float theta_vel = 0;


        float delta_travel = (right_travel + left_travel) / 2.0;
        float delta_theta = (right_travel - left_travel) / WHEEL_SEPARATION;

        float delta_x = 0;
        float delta_y = 0;

        if (right_travel == left_travel)
        {
            delta_x = left_travel * cos(theta);
            delta_y = left_travel * sin(theta);
        }
        else
        {

            float radius = (delta_theta == 0)? 0 : delta_travel / delta_theta;

            float icc_x = pose_x - radius * sin(theta);
            float icc_y = pose_y + radius * cos(theta);

            delta_x = cos(delta_theta) * (pose_x - icc_x) - sin(delta_theta) * (pose_y - icc_y) + icc_x - pose_x;
            delta_y = sin(delta_theta) * (pose_x - icc_x) + cos(delta_theta) * (pose_y - icc_y) + icc_y - pose_y;
        }
        pose_x += delta_x;

        pose_y += delta_y;
        theta += delta_theta;
        while (theta > 2.0 * PI){
        	theta -= 2.0 * PI;
        }

		if (delta_time > 0){
			x_vel = delta_travel / delta_time;
			theta_vel = delta_theta / delta_time;
		}

		geometry_msgs::Quaternion q = tf::createQuaternionFromYaw(theta);

		transform.header.stamp = cur_time;
		transform.transform.translation.x = pose_x;
		transform.transform.translation.y = pose_y;
		transform.transform.translation.z = 0;
		transform.transform.rotation = q;
		tf_broadcaster.sendTransform(transform);
		if(pose_x == 0 && pose_y == 0){
			tf_broadcaster.sendTransform(transform);
		}


		odom.header.stamp = cur_time;

		 odom.pose.pose.position.x = pose_x;
		 odom.pose.pose.position.y = pose_y;
		 odom.pose.pose.orientation = q;
		 odom.twist.twist.linear.x = x_vel;
		 odom.twist.twist.angular.z = theta_vel;



	}

	void publish(){
		cur_time = nh->now();
		set_pose();
		last_time = cur_time;
		pub.publish(&odom);

	}



};
