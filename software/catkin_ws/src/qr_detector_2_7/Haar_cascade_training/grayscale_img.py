import cv2
import os

img = cv2.imread('pattern.jpg', cv2.IMREAD_GRAYSCALE)
cv2.imwrite('pattern.jpg', img)