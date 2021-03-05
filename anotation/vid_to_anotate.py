import cv2 as cv
import sys
import xml.etree.cElementTree as ET

FREQ_RECORD = 20

video = cv.VideoCapture('./test.avi')
if not video.isOpened():
        print("Could not open video")
        sys.exit()

ret, frame = video.read()
if not ret:
        print('Cannot read video file')
        sys.exit()
num_frame = 0

root = ET.Element("root")

while(True):
    ret, frame = video.read()
    if ret==True:
        cv.imshow('Frame to anotate',frame)

        # Defining boundingbox and writing anotation every FREQ_RECORD frames
        if num_frame % FREQ_RECORD == 0:
            bbox = cv.selectROI(frame, False)
            frame = ET.SubElement(root, "frame_number", value=str(num_frame))
            top_left = ET.SubElement(frame, "top_left_coord", value=str(bbox[0]) + ", " + str(bbox[1]))
            bottom_right = ET.SubElement(frame, "bottom_right_coord", value=str(bbox[2]) + ", " + str(bbox[3]))
        if cv.waitKey(1) & 0xFF == ord('q'):
            break
    else:
        break
    num_frame += 1

tree = ET.ElementTree(root)
tree.write("anotation.xml")
video.release()
cv.destroyAllWindows()
