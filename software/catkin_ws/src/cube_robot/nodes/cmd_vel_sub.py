#!/usr/bin/python
import rospy
from geometry_msgs.msg import Twist
import socket

RADIUS = 0.225
WIDTH = 0.450
MAX_SPEED = 3.832
MAX_VALUE = 255


MIN_VEL = 27
MAX_VEL = 200

URL = "http://192.168.22.177:8080"
HEADERS = {'Content-type': 'application/json', 'Accept': 'text/plain'}
TIMEOUT = 0.1

STOP = 0
FORWARD = 1
BACKWARD = 2

IP = '192.168.22.177'
PORT = 23
BUFFER_SIZE = 1024

def get_direction(value):
    global STOP
    global FORWARD
    global BACKWARD
    if value < 0:
        return BACKWARD
    if value > 0:
        return FORWARD
    return STOP



def clip(speed):
    """Ensure value is between minimum and maximum."""
    global MIN_VEL
    global MAX_VEL
    value = round(abs((speed / MAX_SPEED) * MAX_VALUE))
    if MIN_VEL > value > 0:
        return MIN_VEL
    elif value > MAX_VEL:
        return MAX_VEL
    return int(value)


def send_speeds(left_dir, left_speed, right_dir,right_speed):
    data_str = str(left_dir) + ";" + str(left_speed) + ";" + str(right_dir) + ";" + str(right_speed)
    rospy.loginfo(data_str)
    data = bytearray([left_dir, left_speed, right_dir,right_speed])
    rospy.loginfo (data)
    rospy.loginfo(len(data))
    try:
        s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        rospy.loginfo("socket set")
        s.connect((IP, PORT))
        rospy.loginfo("socket connected")
        s.sendall(data)
        rospy.loginfo("data sent")
        #ans = s.recv(BUFFER_SIZE)
        #rospy.loginfo('Received', repr(ans))
    except Exception as e:
        rospy.loginfo("!!!")
        rospy.loginfo(e)

 

def vels(left, right):
        return "linear vel:\left  %s\t right  %s " % (left, right)


def twist_callback(message):
    # Extract linear and angular velocities from the message
    linear = message.linear.x
    angular = message.angular.z
    rospy.loginfo("linear_speed_m/s=%s", linear)
    rospy.loginfo("angular_speed_r/s=%s", angular)

    right_speed = (2*linear + WIDTH*angular)/2
    left_speed = (2*linear - WIDTH*angular)/2

    rospy.loginfo(vels(left_speed,right_speed))
    
    left_speed_percent = clip(left_speed)
    right_speed_percent = clip(right_speed)
  
    send_speeds(get_direction(left_speed),clip(left_speed), get_direction(right_speed),clip(right_speed))
    #rospy.sleep(0.1)


if __name__ == '__main__':
    try:
        rospy.init_node('cmd_vel_sub')
        sub = rospy.Subscriber('/cmd_vel', Twist, twist_callback)
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
