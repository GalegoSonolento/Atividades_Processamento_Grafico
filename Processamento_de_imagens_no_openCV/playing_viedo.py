import numpy as np
import cv2 as cv
 
cap = cv.VideoCapture('output.avi')
 
while cap.isOpened():
    ret, frame = cap.read()
 
    # if frame is read correctly ret is True
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break
    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
 
    cv.imshow('frame', gray)
    if cv.waitKey(25) == ord('q'): # Isso aqui está em milissegundos
        break
 
cap.release()
cv.destroyAllWindows()
