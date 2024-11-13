import numpy as np
import cv2 as cv
 
cap = cv.VideoCapture(0)
 
# Define the codec and create VideoWriter object
fourcc = cv.VideoWriter_fourcc(*'XVID')
out = cv.VideoWriter('output.avi', fourcc, 20.0, (640,  480))
 
while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break
    #frame = cv.flip(frame, 0)

    kernel = np.matrix("-1 -1 -1; -1 8 -1; -1 -1 -1", dtype=np.float32)

    final = cv.filter2D(frame, -1, kernel)
 
    # write the flipped frame
    out.write(final)
 
    cv.imshow('frame', final)
    if cv.waitKey(1) == ord('q'):
        break
 
# Release everything if job is finished
cap.release()
out.release()
cv.destroyAllWindows()