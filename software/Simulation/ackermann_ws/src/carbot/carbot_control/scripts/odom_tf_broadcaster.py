#!/usr/bin/env python
# Lucas Walter
# June 2016
#
# Subscribe to a cmd_vel Twist message and interpret the linear
# and angular components into a joint state for the virtual steer joint.


import rospy

from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point, PointStamped, TransformStamped, Twist
import tf2_ros

# from tf import transformations


class OdomTfBroadcaster:
    def __init__(self):
        self.br = tf2_ros.TransformBroadcaster()
        self.ts = TransformStamped()
        self.ts.header.frame_id = "odom"
        self.ts.child_frame_id = "base_link"
        self.ts.transform.rotation.w = 1.0

        rospy.Subscriber("odom", Odometry, self.odom_callback, queue_size=2)
        # self.timer = rospy.Timer(rospy.Duration(self.period), self.update)

    def odom_callback(self, msg):
        self.ts.header.stamp = msg.header.stamp
        self.ts.transform.translation.x = msg.pose.pose.position.x
        self.ts.transform.translation.y = msg.pose.pose.position.y
        self.ts.transform.translation.z = msg.pose.pose.position.z
        self.ts.transform.rotation = msg.pose.pose.orientation
        self.br.sendTransform(self.ts)


if __name__ == '__main__':
    rospy.init_node("odom_tf_broadcaster")
    odom_tf_broadcaster = OdomTfBroadcaster()
    rospy.spin()
