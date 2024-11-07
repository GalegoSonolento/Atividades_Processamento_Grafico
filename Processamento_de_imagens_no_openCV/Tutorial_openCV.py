import cv2 as cv
import sys
 
img = cv.imread('jojo_teste.jpg') # original
 
if img is None:
    sys.exit("Could not read the image.")
 
cv.imshow("Display window", img)
k = cv.waitKey(0)
 
# Esse comando permite salvar a imagem colocada no OpenCV
if k == ord("s"):
    cv.imwrite("starry_night.png", img)