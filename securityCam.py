#
#	ECE 471 Final Project
#	Team Members: Derek Haas, Dustin Knight, Cameron Sullivan
#	Description: Security camera system that uses a ultrasonic sensor to detect people
#		     Then turn a camera using a servo motor and take a picture.
#

from picamera import PiCamera
from time import sleep
from subprocess import call
import os.path


def takePic():

	pictureLocation = "/home/pi/Documents/git-collab/ECE471-Project/camera/pics/"
	
	fileNum = 0

	# Check if previous image exists
	while(os.path.exists(pictureLocation + "picture%d.jpg" % fileNum):
		fileNum += 1		


	camera = PiCamera()   
	camera.start_preview()						# Turn on Camera and preview
	time.sleep(1)							# Wait 5 seconds to focus
	camera.capture(pictureLocation + 'picture%d.jpg' % fileNum))	# Where to save pics
	camera.stop_preview()						# Stop the preview

	return


def main():
	
	# Start the ultrasonic sensors
	call(["sudo", "ultrasonic/distance"])

	# Infinite loop to detect a person and take a picture of them
	while(1):
	
		# Open file containing what sensor detected a person
		detected = open("filename.txt", "r")
		
		# Busy loop to wait until someone is detected
		while(int(detected.read()) == -1):
			print("No one Detected\n")

		# If the value in the file is 0, move camera to left sensor
		if(int(detected.read()) == 0):
		
			call(["sudo", "servo/servo 90"])

		# If the value in the file is 1, move camera to middle sensor
		if(int(detected.read()) == 1):
	
			call(["sudo", "servo/servo 0"])		

		# If the value in the file is 2, move camera to right sensor
		if(int(detected.read()) == 2):	  

			call(["sudo", "servo/servo 180"])

		# Once camera is in the correct position, take a picture
		takePic()

		# Close the file containing what sensor detected a person
		detected.close()	
	
	return

# Call the main function		
main()
