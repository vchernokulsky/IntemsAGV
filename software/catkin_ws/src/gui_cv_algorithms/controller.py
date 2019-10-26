#!/usr/bin/env python2

from gui import *
import rospy
from std_msgs.msg import String
import io
import json

init_name = 'CV_calibrator'
topic_to_publish = '/CV_settings'
path_to_settings = './src/gui_cv_algorithms/settings.json'
publisher = rospy.Publisher(topic_to_publish, String, queue_size=10)

with io.open(path_to_settings, 'r', encoding='utf-8') as f:
    settings = json.load(f)


def load_data(algo):
    return settings[algo]


def save_data(algo, data):
    rospy.init_node('save_data', anonymous=True)
    settings[algo] = data

    with open(path_to_settings, 'w') as f:
        json.dump(settings, f)

    publisher.publish(algo)


if __name__ == '__main__':
    app = CalibrationApp()
    app.after(1000, save_data, None, None)
    app.mainloop()
