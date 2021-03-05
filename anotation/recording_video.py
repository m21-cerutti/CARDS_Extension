import cv2 as cv

webcam = cv.VideoCapture(0)
fourcc = cv.VideoWriter_fourcc(*'MJPG')
fichier = cv.VideoWriter('./test.avi',fourcc,20,(640,480))

while(True):
    ret, img = webcam.read()
    if ret==True:
        fichier.write(img)
        cv.imshow('Ma Webcam',img)
        if cv.waitKey(1) & 0xFF == ord('q'):
            break
    else:
        break
    
webcam.release()
fichier.release()
cv.destroyAllWindows()





