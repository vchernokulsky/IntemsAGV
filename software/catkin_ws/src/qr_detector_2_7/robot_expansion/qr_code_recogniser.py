from pyzbar import pyzbar
import cv2
import os
qr_cascade = cv2.CascadeClassifier('../Haar_cascade_training/data/cascade.xml')

def get_barcodes_from_img(image):
    barcodes = pyzbar.decode(image)

    if len(barcodes) > 0:
        barcode = barcodes[0]
        barcode_data = barcode.data.decode('utf-8')
        barcode_type = barcode.type
        return barcode_type, barcode_data

    return None


def get_centered_sample(frame, x, y):
    half_side = min([x, frame.shape[1] - x, y, frame.shape[0] - y])
    new_sample = frame[y - half_side:y + half_side, x - half_side:x + half_side]
    return new_sample


def get_qr_code_samples(frame):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    qr_codes = qr_cascade.detectMultiScale(gray, 50, 50)

    samples = []
    for (x, y, w, h) in qr_codes:
        new_sample = get_centered_sample(frame, x, y)
        samples += [new_sample]
        cv2.imshow('sample', new_sample)
        cv2.waitKey(0)
    return samples

if __name__ == "__main__":
    cap = cv2.VideoCapture(0)

    while True:
        ret, frame = cap.read()
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        cv2.imshow('frame', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

    samples = get_qr_code_samples(frame)

    for sample in samples:
        barcode_info = get_barcodes_from_img(sample)

        if barcode_info != None:
            break

    if barcode_info != None:
        barcode_type, barcode_data = barcode_info
        print("Information : \n Found Type : {} Barcode : {}".format(barcode_type, barcode_data))