#
#	ECE 471 Final Project
#	Author: Derek Haas, Dustin K, Cam S
#	Description: Take a picture with the raspberry pi camera and save
#

# NOTE THIS CODE IS FOR TESTING AND IS NOT USED IN THE ACTUAL PROJECT

from picamera import PiCamera
import time

camera = PiCamera() 
camera.vflip = True  
camera.start_preview()		# Turn on Camera and preview
time.sleep(1)			# Wait 1 seconds to focus
camera.capture('/home/pi/Documents/git-collab/ECE471-Project/camera/pics/test.jpg')	# Where to save pics
camera.stop_preview()		# Stop the preview




