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


init_name = 'NN_class_recognizer'
topic_to_publish = 'image_class_on_frame'
topic_to_subscribe = 'usb_cam/image_raw'
classes = ['europallet', 'human', 'shelving']
path_to_checkpoints = './src/nn_class_recognizer/checkpoints/model_epoch500.ckpt'
num_classes = len(classes)


def classify(header):
    frame = bridge.imgmsg_to_cv2(header, "bgr8")
    frame = cv2.resize(frame, (227, 227))
    frame = np.reshape(frame, (1, 227, 227, 3))

    prediction = None

    probs = sess.run(softmax, feed_dict={x: frame, keep_prob: 1})
    prediction = classes[np.argmax(probs)]

    publisher.publish(prediction)


def run():
    rospy.init_node(init_name)
    rospy.Subscriber(topic_to_subscribe, Image, classify)
    rospy.spin()


if __name__ == '__main__':
    bridge = CvBridge()
    publisher = rospy.Publisher(topic_to_publish, String, queue_size=10)

    x = tf.placeholder(tf.float32, [1, 227, 227, 3])
    keep_prob = tf.placeholder(tf.float32)
    model = AlexNet(x, keep_prob, num_classes, [])
    softmax = tf.nn.softmax(model.fc8)

    sess = tf.Session()
    saver = tf.train.Saver()
    saver.restore(sess, path_to_checkpoints)

    run()
