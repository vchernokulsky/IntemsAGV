#!/usr/bin/env python
# Lucas Walter
# June 2016
#
# Subscribe to a cmd_vel Twist message and interpret the linear
# and angular components into a joint state for the virtual steer joint.

import math
import rospy
import tf2_py as tf2
import tf2_ros

from geometry_msgs.msg import PointStamped, Twist
from sensor_msgs.msg import JointState


# from tf import transformations


class CmdVelToJoint:
    def __init__(self):
        self.LIN_MIN = 0.1
        self.LIN_MAX = 0.5
        self.ANG_MIN = 0.2
        self.ANG_MAX = 0.5
        self.rate = rospy.get_param("~rate", 20.0)
        self.period = 1.0 / self.rate

        # angular mode maps angular z directly to steering angle
        # (adjusted appropriately)
        # non-angular mode is somewhat suspect, but it turns
        # a linear y into a command to turn just so that the
        # achieved linear x and y match the desired, though
        # the vehicle has to turn to do so.
        self.angular_mode = rospy.get_param("~angular_mode", True)

        self.tf_buffer = tf2_ros.Buffer()
        self.tf = tf2_ros.TransformListener(self.tf_buffer)

        self.steer_link = rospy.get_param("~steer_link", "lead_steer")
        self.steer_joint = rospy.get_param("~steer_joint", "lead_steer_joint")
        # +/- this angle
        self.min_steer_angle = rospy.get_param("~min_steer_angle", -0.523)
        self.max_steer_angle = rospy.get_param("~max_steer_angle", 0.523)

        self.wheel_joint = rospy.get_param("~wheel_joint", "wheel_lead_axle")
        self.wheel_radius = rospy.get_param("~wheel_radius", 0.051)
        # the spin center is always on the fixed axle y axis of the fixed axle,
        # it is assume zero rotation on the steer_joint puts the steering
        # at zero rotation with respect to fixed axle x axis (or xz plane)
        self.fixed_axle_link = rospy.get_param("~fixed_axle_link", "back_axle")

        self.point_pub = rospy.Publisher("cmd_vel_spin_center", PointStamped, queue_size=1)
        self.steer_pub = rospy.Publisher("steer_joint_states", JointState, queue_size=1)
        # TODO(lucasw) is there a way to get TwistStamped out of standard
        # move_base publishers?
        self.joint_state = JointState()
        self.joint_state.name.append(self.steer_joint)
        self.joint_state.position.append(0.0)
        self.joint_state.velocity.append(0.0)
        self.joint_state.name.append(self.wheel_joint)
        self.joint_state.position.append(0.0)
        self.joint_state.velocity.append(0.0)
        self.cmd_vel = Twist()
        rospy.Subscriber("cmd_vel", Twist, self.cmd_vel_callback, queue_size=2)
        self.timer = rospy.Timer(rospy.Duration(self.period), self.update)

    def cmd_vel_callback(self, msg):
        self.cmd_vel = msg

    def update(self, event):
        try:
            fixed_to_steer = self.tf_buffer.lookup_transform(self.fixed_axle_link,
                                                             self.steer_link,
                                                             rospy.Time(),
                                                             rospy.Duration(4.0))
        except tf2.ExtrapolationException as e:
            rospy.logwarn(e)
            return
        except tf2.LookupException as e:
            rospy.logwarn(e)
            return

        self.joint_state.header.stamp = fixed_to_steer.header.stamp

        # TODO(lucasw) use same time as fixed_to_steer above
        fixed_to_base = self.tf_buffer.lookup_transform(self.fixed_axle_link,
                                                        "base_link",
                                                        rospy.Time(),
                                                        rospy.Duration(4.0))
        # if the cmd_vel is pure linear x, then the joint state is at zero
        # steer angle (no skid steering modelled).
        linear_x = self.cmd_vel.linear.x
        angular_z = self.cmd_vel.angular.z

        self.joint_state.position[0] = 0.0
        self.joint_state.velocity[0] = 0.0

        # handle angular z
        # the proper steer angle is a function of linear.x-
        # linear_x = angular_z * radius

        # ****************************************************
        # if linear_x != 0.0 and abs(linear_x) < self.LIN_MIN:
        #     if linear_x < 0:
        #         linear_x = -self.LIN_MIN
        #     else:
        #         linear_x = self.LIN_MIN
        # elif abs(linear_x) > self.LIN_MAX:
        #     if linear_x < 0:
        #         linear_x = -self.LIN_MAX
        #     else:
        #         linear_x = self.LIN_MAX
        # if angular_z != 0 and abs(angular_z) < self.ANG_MIN:
        #     if angular_z < 0:
        #         angular_z = -self.ANG_MIN
        #     else:
        #         angular_z = self.ANG_MIN
        # elif abs(angular_z) > self.ANG_MAX:
        #     if angular_z < 0:
        #         angular_z = -self.ANG_MAX
        #     else:
        #         angular_z = self.ANG_MAX
        # if angular_z != 0.0 and linear_x == 0.0:
        #     linear_x = self.LIN_MIN
        # ****************************************************
        wheel_angular_velocity = linear_x / self.wheel_radius

        if angular_z != 0.0 and linear_x != 0.0:
            base_turn_radius = abs(linear_x) / abs(angular_z)
            # can't have a turn radius smaller then translation x

            # msg = "linear_x = %f; angular_z = %f" % (linear_x, angular_z)
            # rospy.loginfo(msg)
            # msg = "base_turn_radius = %f" % base_turn_radius
            # rospy.loginfo(msg)
            # msg = "fixed_to_base.transform.translation.x = %f" % fixed_to_base.transform.translation.x
            # rospy.loginfo(msg)
            if base_turn_radius < fixed_to_base.transform.translation.x:
                base_turn_radius = fixed_to_base.transform.translation.x
                # TODO(lucasw) logwarn?
            # base_turn_radius * sin(fixed_to_base_angle) = fixed_to_base.transform.translation.x
            try:
                fixed_to_base_angle = math.asin(fixed_to_base.transform.translation.x / base_turn_radius)
            except:
                # msg = "base_turn_radius = %f; fixed_to_base.transform.translation.x = %f" % (
                #     base_turn_radius, fixed_to_base.transform.translation.x)
                # rospy.logerr(msg)
                return
            # spin center relative to fixed axle
            spin_center_y = base_turn_radius * math.cos(fixed_to_base_angle)
            steer_angle = math.atan2(fixed_to_steer.transform.translation.x,
                                     spin_center_y)
            if angular_z > 0.0:
                spin_center_y = -spin_center_y
                steer_angle = -steer_angle
            if linear_x < 0.0:
                spin_center_y = -spin_center_y
                steer_angle = -steer_angle
            self.joint_state.position[0] = steer_angle

        self.joint_state.position[1] += wheel_angular_velocity * self.period
        self.joint_state.velocity[1] = wheel_angular_velocity

        self.steer_pub.publish(self.joint_state)


if __name__ == '__main__':
    rospy.init_node("cmd_vel_to_joint")
    cmd_vel_to_joint = CmdVelToJoint()
    rospy.spin()
