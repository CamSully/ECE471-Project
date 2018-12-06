#
#	ECE 471 Final Project
#	Author: Derek Haas
#	Description: Take a picture with the raspberry pi camera and save
#

from picamera import PiCamera
import time

camera = PiCamera() 
camera.vflip = True  
camera.start_preview()		# Turn on Camera and preview
time.sleep(5)			# Wait 5 seconds to focus
camera.capture('/home/pi/Documents/git-collab/ECE471-Project/camera/pics/test.jpg')	# Where to save pics
camera.stop_preview()		# Stop the preview




