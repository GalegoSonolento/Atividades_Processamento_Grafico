import numpy as np
import cv2 as cv
 
# Create a black image
img = np.zeros((512,512,3), np.uint8) + 255
#img.fill(255) # or img[:] = 255 -> também funciona para deixar toda a imagem com uma cor só

# Drawing a circle
cv.circle(img,(180,300), 80, (0,0,255), -1)
cv.circle(img,(180,300), 30, (255,255,255), -1)

cv.imshow('sus', img)
k = cv.waitKey(0)