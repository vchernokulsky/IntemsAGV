#!/usr/bin/python
import rospy
from geometry_msgs.msg import Twist
import requests
import json

RADIUS = 0.225
MAX_SPEED = 3.832
MAX_VALUE = 1

URL = "http://192.168.55.177:8080"
HEADERS = {'Content-type': 'application/json', 'Accept': 'text/plain'}
TIMEOUT = 0.1


def clip(value, minimum, maximum):
    """Ensure value is between minimum and maximum."""
    if value != 0:
        if 0 > value > -0.15:
            return -0.15
        if 0.15 > value > 0:
            return 0.15
        if value < minimum:
            return minimum
        elif value > maximum:
            return maximum
    return value


def send_speeds(left_speed, right_speed):
    data = {'l_vel': round(left_speed, 3), 'r_vel': round(right_speed, 3)}
    try:
        rospy.loginfo("msg: " + str(data))
        r = requests.post(URL, data=json.dumps(data), headers=HEADERS, timeout=TIMEOUT)
        if r.status_code != 200:
            rospy.logwarn("status = " + str(r.status_code))
    except requests.exceptions.RequestException as e:
        rospy.logerr("network error")
        rospy.logerr(e)
    except Exception as e:
        rospy.logerr(e)


def twist_callback(message):
    # Extract linear and angular velocities from the message
    linear = message.linear.x
    angular = message.angular.z

    # Calculate wheel speeds in m/s
    left_speed = linear - angular * RADIUS
    right_speed = linear + angular * RADIUS

    # Ideally we'd now use the desired wheel speeds along
    # with data from wheel speed sensors to come up with the
    # power we need to apply to the wheels, but we don't have
    # wheel speed sensors. Instead,  we'll simply convert m/s
    # into percent of maximum wheel speed, which gives us a
    # duty cycle that we can apply to each motor.
    left_speed_percent = clip((left_speed / MAX_SPEED) * MAX_VALUE, -MAX_VALUE, MAX_VALUE)
    right_speed_percent = clip((right_speed / MAX_SPEED) * MAX_VALUE, -MAX_VALUE, MAX_VALUE)
    rospy.loginfo("left_speed_percent=%s", left_speed_percent)
    rospy.loginfo("right_speed_percent=%s", right_speed_percent)

    send_speeds(left_speed_percent, right_speed_percent)


if __name__ == '__main__':
    try:
        rospy.init_node('cmd_vel_sub')
        sub = rospy.Subscriber('/cmd_vel', Twist, twist_callback)
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
