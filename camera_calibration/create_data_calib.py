import cv2 as cv
import sys

FOLDER = "./img_calibration/"
SIZE_X = 256
SIZE_Y = 256


video = cv.VideoCapture(0,cv.CAP_DSHOW)
if not video.isOpened():
        print("Could not open webcam")
        sys.exit(1)

video.set(cv.CAP_PROP_FRAME_WIDTH, SIZE_X)
video.set(cv.CAP_PROP_FRAME_HEIGHT, SIZE_Y)

print(" Width :")
print(video.get(cv.CAP_PROP_FRAME_WIDTH))

print("Height")
print(video.get(cv.CAP_PROP_FRAME_HEIGHT))

num_frame = 0
while(True):
    ret, frame = video.read()
    if ret==True:
        cv.imshow("Frame", frame)

        touch = cv.waitKey(2000)
        if touch & 0xFF == ord('s'):
            cv.imwrite(FOLDER + "calib_"+str(num_frame)+".jpg", frame)
        elif touch & 0xFF == ord('q') or touch & 0xFF == 27:
            break
    num_frame += 1