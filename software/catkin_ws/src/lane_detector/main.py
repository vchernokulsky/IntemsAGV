#!/usr/bin/env python
import rospy
from sensor_msgs.msg import Image
from std_msgs.msg import String
import cv2
from cv_bridge import CvBridge
from lane_detector import *
import json

init_name = 'lane_detector'
topic_to_publish = 'lane'
topics_to_subscribe = ['/CV_settings','/usb_cam/image_raw']


class LaneDetectorNode(object):

    def __init__(self):
        self.publisher = rospy.Publisher(topic_to_publish, String, queue_size=10)

    def cheaderback(self, header):
        bridge = CvBridge()
        frame = bridge.imgmsg_to_cv2(header, "bgr8")

        frame = increase_img_quality(frame)

        frame_hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        mask = get_mask_from_color(frame_hsv) * 255

        w, h = mask.shape[:2]
        begin_mask = mask[h / 3 * 2:, w / 3: w / 3 * 2]
        straight_mask = mask[:h / 3, w / 3:w / 3 * 2]
        left_mask = mask[:, :w / 3]
        right_mask = mask[:, w * 2 / 3:]

        roads = [0] * 4  # straight, right, begin, left

        w, h = mask.shape[:2]
        left_highest_line = [[[0, 0, w / 2, 0]]]
        right_highest_line = [[[w / 2, 0, w, 0]]]
        left_lowest_line = [[[0, h, w / 2, h]]]
        right_lowest_line = [[[w / 2, h, w, h]]]
        center = w / 2, h / 2

        begin_edges = cv2.Canny(begin_mask, 0, 255)
        begin_lines = cv2.HoughLinesP(begin_edges, 1, np.pi / 180, 0, np.array([]), 20, 20)
        roads[2] = get_road(begin_lines, center, (w / 3, h / 3 * 2), begin_mask, 'begin')

        straight_edges = cv2.Canny(straight_mask, 0, 255)
        straight_lines = cv2.HoughLinesP(straight_edges, 1, np.pi / 180, 0, np.array([]), 20, 20)
        roads[0] = get_road(straight_lines, center, (w / 3, 0), straight_mask, 'straight')

        left_edges = cv2.Canny(left_mask, 0, 255)
        left_lines = cv2.HoughLinesP(left_edges, 1, np.pi / 180, 0, np.array([]), 20, 20)

        if left_lines is not None:
            left_lines = np.vstack([left_lines, left_lowest_line])
            left_lines = np.vstack([left_lines, left_highest_line])

        roads[3] = get_road(left_lines, center, (0, 0), left_mask, 'left')

        right_edges = cv2.Canny(right_mask, 0, 255)
        right_lines = cv2.HoughLinesP(right_edges, 1, np.pi / 180, 0, np.array([]), 20, 20)

        if right_lines is not None:
            right_lines = np.vstack([right_lines, right_highest_line])
            right_lines = np.vstack([right_lines, right_lowest_line])

        roads[1] = get_road(right_lines, center, (w * 2 / 3, 0), right_mask, 'right')

        correct_roads(roads, mask)
        print('cheaderback')

        roads_dict = {'straight':  roads[0],
                      'right':  roads[1],
                      'begin':  roads[2],
                      'left':  roads[3]
                      }

        json_dict = json.dumps(roads_dict)

        self.publisher.publish(json_dict)

        if roads[0] is not None:
            p0, p1 = roads[0]
            cv2.line(frame, p0, p1, (255, 0, 0), 2)

        if roads[1] is not None:
            p0, p1 = roads[1]
            cv2.line(frame, p0, p1, (0, 255, 0), 2)

        if roads[2] is not None:
            p0, p1 = roads[2]
            cv2.line(frame, p0, p1, (0, 0, 255), 2)

        if roads[3] is not None:
            p0, p1 = roads[3]
            cv2.line(frame, p0, p1, (255, 255, 0), 2)

        cv2.circle(frame, (w / 2, h / 2), 2, (255, 0, 255), 3)

    @staticmethod
    def check_settings(cv_settings_info):
        if cv_settings_info.data == 'LaneRecognition':
            update_settings()

    def run(self):
        rospy.init_node(init_name)
        rospy.Subscriber(topics_to_subscribe[0], String, self.check_settings)
        rospy.Subscriber(topics_to_subscribe[1], Image, self.cheaderback)
        rospy.spin()


if __name__ == '__main__':
    node = LaneDetectorNode()
    node.run()

