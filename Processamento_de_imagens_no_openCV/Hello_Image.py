import numpy as np
import cv2 as cv

img = cv.imread('jojo_teste.jpg') #original

cv.imshow("Hello OpenCV", img)

k = cv.waitKey(0)