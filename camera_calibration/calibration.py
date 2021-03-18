
# Get from https://www.geeksforgeeks.org/camera-calibration-with-python-opencv/
# Modified to get pixel ratio on table

# Import required modules 
import cv2 
import numpy as np 
import os 
import glob 

def normalize(v):
    norm = np.linalg.norm(v)
    if norm == 0: 
       return v
    return v / norm
  
# Define the dimensions of checkerboard 
CHECKERBOARD = (6, 9) 
SQUARE_SIZE = 0.022 # in m

#DISTANCE_CAMERA (to musure in m)
DISTANCE_CAMERA = 0.6
  
# stop the iteration when specified 
# accuracy, epsilon, is reached or 
# specified number of iterations are completed. 
criteria = (cv2.TERM_CRITERIA_EPS + 
            cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001) 
  
  
# Vector for 3D points 
threedpoints = [] 
  
# Vector for 2D points 
twodpoints = [] 

# Size of borad on x axis
sizex = 0

# Size of borad on y axis
sizey = 0
  
#  3D points real world coordinates 
objectp3d = np.zeros((1, CHECKERBOARD[0]  
                      * CHECKERBOARD[1],  
                      3), np.float32) 
objectp3d[0, :, :2] = np.mgrid[0:CHECKERBOARD[0], 
                               0:CHECKERBOARD[1]].T.reshape(-1, 2) * SQUARE_SIZE
prev_img_shape = None

#print(objectp3d)

# Extracting path of individual image stored 
# in ./img_calibration/ folder.
images = glob.glob('./img_calibration/*.jpg') 

# Number of table images
nbimgtables = 0

for filename in images: 
    
    image = cv2.imread(filename) 
    grayColor = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY) 
  
    # Find the chess board corners 
    # If desired number of corners are 
    # found in the image then ret = true 
    ret, corners = cv2.findChessboardCorners( 
                    grayColor, CHECKERBOARD,  
                    cv2.CALIB_CB_ADAPTIVE_THRESH  
                    + cv2.CALIB_CB_FAST_CHECK + 
                    cv2.CALIB_CB_NORMALIZE_IMAGE) 
  
    # If desired number of corners can be detected then, 
    # refine the pixel coordinates and display 
    # them on the images of checker board 
    if ret == True: 
        threedpoints.append(objectp3d) 
  
        # Refining pixel coordinates 
        # for given 2d points. 
        corners2 = cv2.cornerSubPix( 
            grayColor, corners, (11, 11), (-1, -1), criteria) 
  
        twodpoints.append(corners2) 
  
        # Draw and display the corners 
        image = cv2.drawChessboardCorners(image,  
                                          CHECKERBOARD,  
                                          corners2, ret) 
        
        # Calculate real size board 
        if("table" in filename):
            top_left = corners2[0]
            bottom_right = corners2[CHECKERBOARD[0] * CHECKERBOARD[1] -1]
            diag = bottom_right - top_left
            # Verify good rotation
            if(np.dot(np.array([[1,-1]]), diag.flatten()) > 0.5):
                sizex += abs(diag[0, 0])
                sizey += abs(diag[0, 1])
                nbimgtables += 1
            else:
                print(filename + " in wrong direction checkerboard.")
  
    cv2.imshow('img', image) 
    cv2.waitKey(100) 

cv2.destroyAllWindows() 
  
h, w = image.shape[:2] 

# Pixel size board on table
sizex /= nbimgtables
sizey /= nbimgtables

print(" Size X:") 
print(sizex) 

print(" Size Y:") 
print(sizey) 

# Ratio size
ratioX = (SQUARE_SIZE * CHECKERBOARD[1]) / sizex
ratioY = (SQUARE_SIZE * CHECKERBOARD[0]) / sizey

# Perform camera calibration by 
# passing the value of above found out 3D points (threedpoints) 
# and its corresponding pixel coordinates of the 
# detected corners (twodpoints) 
ret, matrix, distortion, r_vecs, t_vecs = cv2.calibrateCamera( 
    threedpoints, twodpoints, grayColor.shape[::-1], None, None) 
  
  
# Displayig required output 
print(" Camera matrix:") 
print(matrix) 

print(" Ratio X:") 
print(ratioX) 
#print(600 * ratioX * 1000) # in m

print(" Ratio Y:") 
print(ratioY) 
#print(200 * ratioY * 1000) # in m

print(" Distance camera:") 
print(DISTANCE_CAMERA) 

#print("\n Distortion coefficient:") 
#print(distortion) 
  
#print("\n Rotation Vectors:") 
#print(r_vecs) 
  
#print("\n Translation Vectors:") 
#print(t_vecs) 


# XML Storage
s = cv2.FileStorage("calibration.xml", cv2.FileStorage_WRITE)
s.write('camera_matrix', matrix)
s.write("ratio_x", ratioX)
s.write("ratio_y", ratioY)
s.write("dist_cam", DISTANCE_CAMERA)
s.release()