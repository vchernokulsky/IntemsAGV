#!/usr/bin/python

import json
import math
from math import sin, cos, pi

import socket

import rospy
import tf
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point, Pose, Quaternion, Twist, Vector3

IP = '192.168.22.178'
PORT = 23
BUFFER_SIZE = 1024



class Publisher(object):
    def __init__(self, w_left=0, w_right=0, x=0,y=0):
        self.WIDTH = 0.450
        self.RADIUS = 0.100
        self.odom_pub = rospy.Publisher("odom", Odometry, queue_size=5)
        self.odom_broadcaster = tf.TransformBroadcaster()
        
        self.x = 0.0
        self.y = 0.0
        self.th = 0.0

        self.vx = 0.0
        self.vy = 0.0
        self.vth = 0.0

        self.current_time = rospy.Time.now()
        self.last_time = rospy.Time.now()
        self.odom_quat = tf.transformations.quaternion_from_euler(0, 0, 0)

    def vels(self):
        return "currently:\tlinear vel %s\t angular vel %s " % (self.vx, self.vth)

    def update_velocity(self, omega_left, omega_right):
        v_left = omega_left * self.RADIUS
        v_right = omega_right * self.RADIUS
        self.current_time = rospy.Time.now()  
        self.vx = ((v_right + v_left) / 2)# * 10
        self.vy = 0
        self.vth =((v_right - v_left) / self.WIDTH)# * 10
        rospy.loginfo(self.vels())

        dt = (self.current_time - self.last_time).to_sec()
        delta_x = (self.vx * math.cos(self.th)) * dt
        delta_y = (self.vx * math.sin(self.th)) * dt
        delta_th = self.vth * dt

        self.x += delta_x
        self.y += delta_y
        self.th += delta_th

    def pub_tf(self):
         # since all odometry is 6DOF we'll need a quaternion created from yaw
        self.odom_quat = tf.transformations.quaternion_from_euler(0, 0, self.th)

        # first, we'll publish the transform over tf
        self.odom_broadcaster.sendTransform((self.x, self.y, 0.), self.odom_quat, self.current_time, "base_link", "odom")

    def pub_odom(self):
        # next, we'll publish the odometry message over ROS
        odom = Odometry()
        odom.header.stamp = self.current_time
        odom.header.frame_id = "odom"

        # set the position
        odom.pose.pose = Pose(Point(self.x, self.y, 0.0),  Quaternion(*self.odom_quat))
        

        # set the velocity
        odom.child_frame_id = "base_link"
        odom.twist.twist = Twist(Vector3(self.vx, self.vy, 0), Vector3(0, 0, self.vth))
        # publish the message
        self.odom_pub.publish(odom)


    def publish(self,v_left, v_right):
        self.update_velocity(v_left, v_right)
        self.pub_tf()
        self.pub_odom()
        self.last_time = self.current_time

 


def main():
    rospy.init_node('odometry_publisher')
    pub = Publisher()
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            rospy.loginfo("socket set")
            s.connect((IP, PORT))
            rospy.loginfo("socket connected")
            data = s.recv(BUFFER_SIZE)
            rospy.loginfo("data received")
            rospy.loginfo(data)
            s.close()
            json_dict = json.loads(data.decode('utf-8'))

            l_vel = json_dict['l_vel'] 
            r_vel = json_dict['r_vel']
            pub.publish(l_vel , r_vel )
        except Exception as e:
            rospy.logerr(e)
        finally:
            rate.sleep()
    rospy.spin()


if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
