#!/usr/bin/env python
import robot_expansion.code_recogniser as recogn
import rospy
from sensor_msgs.msg import Image
from std_msgs.msg import String
import message_filters
from cv_bridge import CvBridge
import cv2
import json

init_name = 'QR_code_publisher'
topic_to_publish = 'QR_code_meaning'
topics_to_subscribe = ['/CV_settings','/usb_cam/image_raw']


class QrDetectorNode(object):

    def __init__(self):
        self.publisher = rospy.Publisher(topic_to_publish, String, queue_size=10)

    def read_code(self, image):
        bridge = CvBridge()
        frame = bridge.imgmsg_to_cv2(image, "bgr8")

        # start with qr_codes
        samples = recogn.get_qr_code_samples(frame.copy())
        barcode_info = (None, None)
        for sample in samples:
            barcode_info = recogn.get_codes_from_img(sample)

            if barcode_info != None:
                break

        barcode_dict = {'barcode_type': barcode_info[0], 'barcode_data': barcode_info[1]}
        json_dict = json.dumps(barcode_dict)
        self.publisher.publish(json_dict)

        preprocessed = recogn.get_preprocessed_img_for_barcode(frame.copy())
        barcode_info = recogn.get_codes_from_img(preprocessed)

        barcode_dict = {'barcode_type': barcode_info[0], 'barcode_data': barcode_info[1]}
        json_dict = json.dumps(barcode_dict)
        self.publisher.publish(json_dict)

    @staticmethod
    def check_settings(cv_settings_info):
        if cv_settings_info.data == 'QrCode':
            recogn.update_settings()

    def run(self):
        rospy.init_node(init_name)

        rospy.Subscriber(topics_to_subscribe[0], String, self.check_settings)
        rospy.Subscriber(topics_to_subscribe[1], Image, self.read_code)

        rospy.spin()


qr = QrDetectorNode()
qr.run()

