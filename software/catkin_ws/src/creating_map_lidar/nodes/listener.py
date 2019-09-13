#!/usr/bin/env python

import socket
import time
import cv2
import rospy
from sensor_msgs.msg import LaserScan

import numpy as np
import math

POINT_RADIUS = 1
POINT_COLOR = [0, 0, 255]
RANGE_SCALE = 100

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
s.bind(('192.168.55.100', 5000))
s.listen(5)

class Photographer(object):
    def __init__(self, angle_min, angle_max, angle_increment, range_min, range_max, shape):
        self.angle_min = angle_min
        self.angle_max = angle_max
        self.angle_increment = angle_increment
        self.range_min = range_min
        self.range_max = range_max
        self.snapshots = []
        self.snapshot_shape = shape
        self.center = (shape[0]//2, shape[1]//2)

    def is_point_in_frame(self, x, y,):
        if (POINT_RADIUS < x < self.snapshot_shape[1] - POINT_RADIUS and
            POINT_RADIUS < y < self.snapshot_shape[0] - POINT_RADIUS):
            return True
        return False

    def take_photo(self, ranges):
        photo = 200 * np.ones(shape=self.snapshot_shape, dtype=np.uint8)

        i = 1
        for range_ in ranges:
            if range_ != float('inf'):
                x = self.center[1] + int(range_ * RANGE_SCALE * math.cos(self.angle_increment * i))
                y = self.center[0] + int(range_ * RANGE_SCALE * math.sin(self.angle_increment * i))
                if self.is_point_in_frame(x, y):
                    photo[y - POINT_RADIUS: y + POINT_RADIUS, x - POINT_RADIUS: x + POINT_RADIUS] = POINT_COLOR

            i += 1

        return photo

def cheaderback(header):
    photographer = Photographer(header.angle_min,
                                        header.angle_max,
                                        header.angle_increment,
                                        header.range_min,
                                        header.range_max,
                                        shape=[1000, 1000, 3])

    frame = photographer.take_photo(header.ranges)

    data = cv2.imencode('.jpg', frame)[1].tostring()
    

    conn, addr = s.accept()
    
    conn.sendall(str(len(data)))
    conn.sendall(data)

    conn.close()
            
def listener():
    rospy.init_node('listener', anonymous=True)
    
    rospy.Subscriber("scan", LaserScan, cheaderback)
    rospy.spin()

if __name__ == '__main__':
    listener()
    s.close()
