#
#

import cv2 as cv
import sys
import xml.etree.cElementTree as ET

FREQ_RECORD = 20
IDS_TO_USE = range(0, 5)

video = cv.VideoCapture('./test.avi')
if not video.isOpened():
        print("Could not open video")
        sys.exit()

ret, frame = video.read()
if not ret:
        print('Cannot read video file')
        sys.exit()
num_frame = 0

root = ET.Element("Video_annotations")

while(True):
    ret, frame = video.read()
    if ret==True:
        cv.imshow('Frame to anotate',frame)

        # Defining boundingbox and writing anotation every FREQ_RECORD frames
        if num_frame % FREQ_RECORD == 0:
            select = True
            xmlframe = ET.SubElement(root, "frame", 
                number=str(num_frame))

            for id in IDS_TO_USE:
                print("Id" + str(id))
                x, y, width, height = cv.selectROI(frame, True)
                if width==0 and height ==0:
                    ET.SubElement(xmlframe, "Object",
                        id=str(id), 
                        state="Lost") 
                else:
                    ET.SubElement(xmlframe, "Object",
                        id=str(id), 
                        state="Live", 
                        x=str(x), 
                        y=str(y), 
                        width=str(width), 
                        height=str(height))
        if cv.waitKey(1) & 0xFF == ord('q'):
            break
    else:
        break
    num_frame += 1

tree = ET.ElementTree(root)
tree.write("anotation.xml")
video.release()
cv.destroyAllWindows()
