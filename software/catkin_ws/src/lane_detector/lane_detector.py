import cv2
import numpy as np
import json

path_to_settings = './src/gui_cv_algorithms/settings.json'

with open(path_to_settings) as f:
    settings = json.load(f)


def update_settings():
    global settings
    with open(path_to_settings) as f:
        settings = json.load(f)


def increase_img_quality(img):
    img_blured = cv2.GaussianBlur(img, (5, 5), 0)

    img_sharped = img_blured
    for i in range(eval(settings['LaneRecognition']['sharp']['times'])):
        img_sharped = sharp_with_gaussian(img_sharped)

    return img_sharped


def get_mask_from_color(frame_hsv):
    h, s, v = eval(settings['LaneRecognition']['laneColor'])
    mask = ((frame_hsv[:, :, 0] >= h[0]) & (frame_hsv[:, :, 0] <= h[1]) &
            (frame_hsv[:, :, 1] >= s[0]) & (frame_hsv[:, :, 1] <= s[1]) &
            (frame_hsv[:, :, 2] >= v[0]) & (frame_hsv[:, :, 2] <= v[1])).astype(np.uint8)

    return mask


def get_road(lines, center, start_point, mask, way):
    def is_ax_line(a, b):
        if abs(a - b) <= eval(settings['LaneRecognition']['widthLineRange']):
            return True

        return False

    def is_road(a, b):
        lane_width = eval(settings['LaneRecognition']['laneWidth'])
        lane_width_eps = eval(settings['LaneRecognition']['laneWidthEps'])

        if lane_width - lane_width_eps <= abs(a - b) <= lane_width + lane_width_eps:
            road = mask[bot_y:top_y, bot_x:top_x]

            if road.shape[0] > 0 and road.shape[1] > 0:
                if road.mean() > eval(settings['LaneRecognition']['laneTreshold']):
                    return True

        return False

    if lines is not None:
        if way == 'begin' or way == 'straight':
            lines = list(filter(lambda l: is_ax_line(l[0][0], l[0][2]), lines))
        elif way == 'left' or way == 'right':
            lines = list(filter(lambda l: is_ax_line(l[0][1], l[0][3]), lines))
        else:
            raise Exception('way must be <begin>, <left>, <right> or <straight>')

        for i in reversed(range(len(lines))):
            for j in range(len(lines)):
                bot_x = min(lines[i][0][0], lines[i][0][2], lines[j][0][0], lines[j][0][2])
                bot_y = min(lines[i][0][1], lines[i][0][3], lines[j][0][1], lines[j][0][3])
                top_x = max(lines[i][0][0], lines[i][0][2], lines[j][0][0], lines[j][0][2])
                top_y = max(lines[i][0][1], lines[i][0][3], lines[j][0][1], lines[j][0][3])

                road_exist = False

                if way == 'begin' or way == 'straight':
                    if is_road(bot_x, top_x):
                        road_exist = True

                if way == 'left' or way == 'right':
                    if is_road(bot_y, top_y):
                        road_exist = True

                if road_exist:
                    x0 = x1 = y0 = y1 = 0

                    if way == 'straight':
                        x0 = x1 = center[0]  # not change
                        y1 = start_point[1] + bot_y
                        y0 = start_point[1] + top_y

                    if way == 'begin':
                        x0 = x1 = center[0]  # not change
                        y1 = start_point[1] + bot_y
                        y0 = center[1] * 2

                    if way == 'right':
                        y0 = y1 = start_point[1] + (top_y + bot_y) / 2
                        x1 = start_point[0] + top_x
                        x0 = center[0]  # not change

                    if way == 'left':
                        y0 = y1 = start_point[1] + (top_y + bot_y) / 2
                        x1 = start_point[0] + bot_x
                        x0 = center[0]  # not change

                    return [(x0, y0), (x1, y1)]

            del lines[i]

    return None


def sharp_with_gaussian(image):
    kernel = np.array(eval(settings['LaneRecognition']['sharp']['kernel'])) / eval(settings['LaneRecognition']['sharp']['kernelDivider'])

    output = cv2.filter2D(image, -1, kernel)

    return output


def correct_roads(roads, mask):
    w, h = mask.shape[:2]
    center = w / 2, h / 2

    if roads[2]:
        if roads[1] or roads[3]:
            roads[2][1:] = roads[1][:1] if roads[1] else roads[3][:1]

            if roads[0]:
                roads[0][:1] = roads[1][:1] if roads[1] else roads[3][:1]

        elif not roads[1] and not roads[3]:
            middle_mask = mask[h / 3: h / 3 * 2, w / 3: w / 3 * 2]
            middle_edges = cv2.Canny(middle_mask, 0, 255)
            middle_lines = cv2.HoughLinesP(middle_edges, 1, np.pi / 180, 0, np.array([]), 20, 20)
            middle_road = get_road(middle_lines, center, (h / 3, w / 3), middle_mask, 'straight')

            if middle_road is not None:
                roads[0][:1] = roads[2][1:]
            else:
                roads[0] = None

        if roads[0]:
            if abs(roads[0][0][1] - roads[0][1][1]) < 30:
                roads[2][1:] = roads[0][:1]
                roads[0] = None

    return roads
