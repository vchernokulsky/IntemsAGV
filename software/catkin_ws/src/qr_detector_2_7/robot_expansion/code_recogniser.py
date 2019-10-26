from pyzbar import pyzbar
import cv2
import os
import numpy as np
import json

qr_cascade = cv2.CascadeClassifier(
    '/home/data-scientist/IntemsAGV/software/catkin_ws/src/qr_detector_2_7/Haar_cascade_training/data/cascade.xml')
barcode_cascade = cv2.CascadeClassifier(
    '/home/data-scientist/Desktop/IntemsAGV/software/catkin_ws/src/qr_detector_2_7/Haar_cascade_training/data1'
    '/cascade1.xml')
path_to_settings = './src/gui_cv_algorithms/settings.json'

with open(path_to_settings) as f:
    settings = json.load(f)


def update_settings():
    global settings
    with open(path_to_settings) as f:
        settings = json.load(f)


# common functions start


def get_codes_from_img(image):
    barcodes = pyzbar.decode(image)

    if len(barcodes) > 0:
        barcode = barcodes[0]
        barcode_data = barcode.data.decode('utf-8')
        barcode_type = barcode.type
        return barcode_type, barcode_data

    return None, None


def get_centered_sample(frame, x, y):
    half_side = min([x, frame.shape[1] - x, y, frame.shape[0] - y])
    new_sample = frame[y - half_side:y + half_side, x - half_side:x + half_side]
    return new_sample


# common functions end


# unic functions start
def get_qr_code_samples(frame):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    qr_codes = qr_cascade.detectMultiScale(gray, 50, 50)

    samples = []
    for (x, y, w, h) in qr_codes:
        new_sample = get_centered_sample(frame, x, y)
        samples += [new_sample]

    return samples


def get_barcode_samples(frame):
    def sharp_with_gaussian(image):

        kernel = np.array(eval(settings['QrCode']['sharp']['kernel']))

        output = image.copy()
        for _ in range(eval(settings['QrCode']['sharp']['times'])):
            output = cv2.filter2D(output, -1, kernel)

        return output

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    gray = sharp_with_gaussian(gray)

    gray = cv2.erode(gray, np.array(eval(settings['QrCode']['erode']['kernel']), dtype=float),
                     iterations=eval(settings['QrCode']['erode']['times']))
    cv2.imshow('gray', gray)
    cv2.waitKey(1)
    barcodes = barcode_cascade.detectMultiScale(gray, 50, 50)

    samples = []
    for (x, y, w, h) in barcodes:
        new_sample = get_centered_sample(gray, x, y)
        samples += [new_sample]

    return samples
# unic functions end
