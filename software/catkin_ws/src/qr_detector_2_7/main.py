#!/usr/bin/env python
import rospy
from sensor_msgs.msg import Image
from std_msgs.msg import String
import cv2
import numpy as np
from cv_bridge import CvBridge

init_name = 'QR_code_publisher'
topic_to_publish = 'QR_code_meaning'
topic_to_subscribe = '/usb_cam/image_raw'

class QR_detector_node(object):
    def __init__(self):
	self.value = 'wewvb' 


    def cheaderback(self, header):
	bridge = CvBridge()	
	frame = bridge.imgmsg_to_cv2(header, "bgr8")
	cv2.imshow('frame', frame)
	cv2.waitkey(0)

    def run(self):
        rospy.init_node(init_name)
        self.pub = rospy.Publisher(topic_to_publish, String, queue_size = 10)
        rospy.Subscriber(topic_to_subscribe, Image, self.cheaderback)
	rospy.spin()

qr = QR_detector_node()
qr.run()

