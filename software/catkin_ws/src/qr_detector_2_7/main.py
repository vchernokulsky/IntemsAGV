#!/usr/bin/env python
import robot_expansion.qr_code_recogniser as qr_recogn
import rospy
from sensor_msgs.msg import Image
from std_msgs.msg import String
import cv2
from cv_bridge import CvBridge
import json

init_name = 'QR_code_publisher'
topic_to_publish = 'QR_code_meaning'
topic_to_subscribe = '/usb_cam/image_raw'

class QR_detector_node(object):
    def __init__(self):
	    self.value = 'wewvb' 


    def cheaderback(self, header):
        bridge = CvBridge()
        frame = bridge.imgmsg_to_cv2(header, "bgr8")
        samples = qr_recogn.get_qr_code_samples(frame)

        barcode_info = (None, None)
        for sample in samples:
            barcode_info = qr_recogn.get_barcodes_from_img(sample)
            
            if barcode_info != None:
                break

        barcode_dict = {'barcode_type': barcode_info[0], 'barcode_data': barcode_info[1]}
        json_dict = json.dumps(barcode_dict)
        print(json_dict)
        self.publisher.publish(json_dict)

    def run(self):
        rospy.init_node(init_name)
        self.publisher = rospy.Publisher(topic_to_publish, String, queue_size = 10)
        rospy.Subscriber(topic_to_subscribe, Image, self.cheaderback)
	rospy.spin()

qr = QR_detector_node()
qr.run()

