import numpy as np
import cv2 as cv

img = cv.imread('jojo_teste.jpg') #original

kernel = np.matrix("-1 -1 -1; -1 8 -1; -1 -1 -1", dtype=np.float32)

final = cv.filter2D(img, -1, kernel)

cv.imshow("Hello OpenCV", final)

k = cv.waitKey(0)