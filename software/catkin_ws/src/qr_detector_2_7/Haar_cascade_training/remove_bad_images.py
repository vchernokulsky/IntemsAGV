import os
import cv2

for path_to_img in os.listdir(path_to_img_dir):
    img = cv2.imread(os.path.join(path_to_img_dir, path_to_img), cv2.IMREAD_GRAYSCALE)
    cv2.imwrite(os.path.join(path_to_img_dir, path_to_img), img)