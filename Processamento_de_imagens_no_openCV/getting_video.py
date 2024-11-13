import numpy as np
import cv2 as cv
 
cap = cv.VideoCapture(0)
if not cap.isOpened():
    print("Cannot open camera")
    exit()
while True:
    # Capture frame-by-frame
    ret, frame = cap.read()
 
    # if frame is read correctly ret is True
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break
    # Our operations on the frame come here
    #gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    
    #match_value = np.array([60, 60, 60], dtype=np.uint8)
    #mask = np.all(frame < match_value, axis=-1)
    #final = np.where(mask[..., np.newaxis], np.array([0, 0, 255], dtype=np.uint8), frame)

    kernel = np.matrix("-1 -1 -1; -1 8 -1; -1 -1 -1", dtype=np.float32)

    final = cv.filter2D(frame, -1, kernel)

    # Display the resulting frame
    cv.imshow('frame', final)
    if cv.waitKey(1) == ord('q'):
        break
 
# When everything done, release the capture
cap.release()
cv.destroyAllWindows()