import cv2
from picamera2 import Picamera2
import serial
from time import sleep

face_detector = cv2.CascadeClassifier("/home/saksham/Downloads/haarcascade_frontalface_default.xml")
cv2.startWindowThread()

picam2 = Picamera2()
picam2.configure(picam2.create_preview_configuration(main={"format": 'XRGB8888', "size": (640, 480)}))
picam2.start()

ArduinoSerial = serial.Serial("/dev/ttyACM0", 9600)
# Wait for the connection to establish
sleep(2)

while True:
    im = picam2.capture_array()
    im = cv2.flip(im,flipCode=1)
    gray = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)
    faces = face_detector.detectMultiScale(gray, 1.1, 5)  # detect the face
    for x, y, w, h in faces:
        string = 'X{0:d}Y{1:d}'.format((x + w // 2), (y + h // 2))
        ArduinoSerial.write(string.encode('utf-8'))
        cv2.circle(im, (x + w // 2, y + h // 2), 2, (0, 255, 0), 2)
        cv2.rectangle(im, (x, y), (x + w, y + h), (0, 0, 255), 3)

    cv2.rectangle(im, (640 // 2 - 30, 480 // 2 - 30),
                  (640 // 2 + 30, 480 // 2 + 30),
                  (255, 255, 255), 3)
    cv2.imshow('Frame', im)
    cv2.waitKey(1)

# Ensure to release resources properly
cv2.destroyAllWindows()

