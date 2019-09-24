#!/usr/bin/env python
# -*- coding: utf-8 -*-
from alexnet import AlexNet

import rospy
from sensor_msgs.msg import Image
from std_msgs.msg import String
import tensorflow as tf
import numpy as np
import cv2
from cv_bridge import CvBridge
import PIL

init_name = 'NN_class_recognizer'
topic_to_publish = 'image_class_on_frame'
topic_to_subscribe = 'usb_cam/image_raw'
classes = ['европалет', 'человек', 'стеллаж']
path_to_checkpoints = 'checkpoints/model_epoch500.ckpt'
num_classes = len(classes)

class NN_class_recognizer(object):
    def __init__(self):
        self.x = tf.compat.v1.placeholder(tf.float32, [1, 227, 227, 3])
        self.keep_prob = tf.compat.v1.placeholder (tf.float32)
        self.model = AlexNet(self.x, self.keep_prob, num_classes, [])
        self.softmax = tf.nn.softmax(self.model.fc8)

    def cheaderback(self, header):
        bridge = CvBridge()
        frame = bridge.imgmsg_to_cv2(header, "bgr8")
        frame = frame.resize((227, 227), PIL.Image.ANTIALIAS)
        prediction = self.classify(frame)

        self.publisher.publish(prediction)

    def classify(self, frame):
        with tf.Session() as sess:
            saver = tf.train.Saver();
            saver.restore(sess, path_to_checkpoints)

            frame = frame.astype('float32')
            frame = np.reshape(frame, (1,227,227,3))

            probs = sess.run(self.softmax, feed_dict={self.x: self.frame, self.keep_prob: 1})
            prediction = classes[np.argmax(probs)]

            return prediction


    def run(self):
        rospy.init_node(init_name)
        self.publisher = rospy.Publisher(topic_to_publish, String, queue_size = 10)
        rospy.Subscriber(topic_to_subscribe, Image, self.cheaderback)

node = NN_class_recognizer()
node.run()
