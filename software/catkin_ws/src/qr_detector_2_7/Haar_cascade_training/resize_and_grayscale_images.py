import cv2
import os
import PIL
from PIL import Image

def resize_imgs_in_dir(path_to_img_dir, basewidth):
    for path_to_img in os.listdir(path_to_img_dir):
        img = Image.open(os.path.join(path_to_img_dir, path_to_img))
        img = img.resize((basewidth, basewidth), PIL.Image.ANTIALIAS)
        img.save(os.path.join(path_to_img_dir, path_to_img))

def grayscale_imgs_in_dir(path_to_img_dir):
    for path_to_img in os.listdir(path_to_img_dir):
        img = cv2.imread(os.path.join(path_to_img_dir, path_to_img), cv2.IMREAD_GRAYSCALE)
        cv2.imwrite(os.path.join(path_to_img_dir, path_to_img), img)


path_to_img_dir = 'neg'

resize_imgs_in_dir(path_to_img_dir, 500)
grayscale_imgs_in_dir(path_to_img_dir)