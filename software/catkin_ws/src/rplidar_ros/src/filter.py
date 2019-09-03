#!/usr/bin/env python

import roslib
import rospy
from sensor_msgs.msg import LaserScan
import math

topic_from = "/scan"
topic_to = "/transform"
min_angle = - math.pi / 4.0
max_angle = math.pi / 4.0
publisher = rospy.Publisher(topic_to, LaserScan, queue_size=10)


def transform(data):
    print("transform")
    min_i = int(math.ceil((min_angle - data.angle_min + data.angle_increment) / data.angle_increment))
    max_i = len(data.ranges) - int(math.floor((data.angle_max - max_angle) / data.angle_increment))
    ranges = list(data.ranges)
    ranges[:min_i] = [float('inf')] * min_i
    ranges[max_i:] = [float('inf')] * (len(data.ranges) - max_i)

    intens = list(data.ranges)
    intens[:min_i] = [float('inf')] * min_i
    intens[max_i:] = [float('inf')] * (len(data.ranges) - max_i)
  
    data.ranges = tuple(ranges)
    data.intensities = tuple(intens)
    publisher.publish(data)



def main():
    rospy.init_node('laser_transform', anonymous=True)
    rospy.Subscriber(topic_from, LaserScan, transform, queue_size=1, buff_size=2 ** 24)
    try:
        rospy.loginfo("Enetering ROS Spin")
        rospy.spin()
    except KeyboardInterrupt:
        print("Shutting down")


if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
