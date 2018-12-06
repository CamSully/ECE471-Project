ECE 471 Final Project Repository

Authors: Derek Haas, Dustin Knight, Cameron Sullivan

Description of Project: 
This final project is an automated security camera system.  For the project, 3 ultrasonic sensors are used to detect people.  These 3 sensors are placed 90 degrees apart from each other (covering angles from 0 to 180 degrees).  When a change in distance is noticed by the sensors, a servo motor rotates to that sensors position.  Attached to this servo is a raspberry pi camera that will (hopefully) take a picture of the person detected.  This image will then be uploaded to a web server.


Code for this project is broken into 3 parts:

1. Code to retrieve distance from the sensors
2. Code to move the servo 0-180 degrees
3. Code to take a picture with a the camera and post to web server


Servo motor code can be found in servo/

Ultrasonic sensor code can be found in ultrasonic/

Camera code can be found in camera/

The master file that will run all code is securityCam.py, written in Python3 because it is easy :)

The code to use the ultrasonic sensors and rotate the motor is written in C to show understanding of low level C code and its uses with embedded busses on the Raspberry Pi.

index.html hold html code that allows us to post the image to the webserver.



TO DO:

"sudo pip3 install flask" for needed python library
Finish detection algorithm for ultrasonic sensors
Write to file what sensor detects a person
Fix image file naming scheme for ease of posting image to web server (Derek)
Test hardware in newly made box
Test securityCam.py and ensure it actually does what its suppose to 
Debug
Run code at boot?  rc.local?  Cron job?  Service?

