#
#	ECE 471 Final Project
#	Team Members: Derek Haas, Dustin Knight, Cameron Sullivan
#	Description: Security camera system that uses a ultrasonic sensor to detect people
#		     Then turn a camera using a servo motor and take a picture.
#

from picamera import PiCamera
import time
from subprocess import call
import os.path
from flask import Flask, render_template


def takePic():

	pictureLocation = "/home/pi/ECE471-Project/webServer/static/"

	fileNum = 0
	exists = True


	# Check if previous image exists
	if(os.path.exists(pictureLocation + "detected.jpg") == 0):
		detectionPicture = "detection.jpg"
		exists = False

	while(exists):
		if(os.path.exists(pictureLocation + "previous%d.jpg" % fileNum)):
			fileNum += 1
		else:
			detectionPicture = "previous%d.jpg" % fileNum

		call(["mv", "detected.jpg", detectionPicture])
		detectionPicture = "detected.jpg"

	camera = PiCamera()
	camera.vflip = True
	camera.start_preview()						# Turn on Camera and preview
	time.sleep(1)							# Wait 5 seconds to focus
	camera.capture(pictureLocation + detectionPicture)	# Where to save pics
	camera.stop_preview()						# Stop the preview
	camera.close()

	return


def main():

	counter = 0

	print("Starting Ultrasonic Sensors...")

	# Start the ultrasonic sensors
	call("sudo ultrasonic/distance &", shell = True)

	time.sleep(23)

	# Indicate baseline and noise set
	call(["sudo", "servo/servo", "0"])
	time.sleep(0.5)
	call(["sudo", "servo/servo", "180"])
	time.sleep(0.5)
	call(["sudo", "servo/servo", "90"])

	detected = open("detection.txt", "w")
	detected.write("0")
	detected.close()

	print("SYSTEM READY")

	# Infinite loop to detect a person and take a picture of them
	while(1):
		
		# Open file containing what sensor detected a person
		detected = open("detection.txt", "r")

		print("No one Detected")

		# Busy loop to wait until someone is detected
		while(int(detected.read()) == 0):
			detected.close()
			detected = open("detection.txt", "r")

		detected.close()
		detected = open("detection.txt", "r")

		# If the value in the file is left, move camera to left sensor
		if(int(detected.read()) == 1):

			print("DETECTED LEFT")
			call(["sudo", "servo/servo", "0"])

		detected.close()
		detected = open("detection.txt", "r")

		# If the value in the file is center, move camera to center sensor
		if(int(detected.read()) == 2):

			print("DETECTED CENTER")
			call(["sudo", "servo/servo", "90"])

		detected.close()
		detected = open("detection.txt", "r")

		# If the value in the file is right, move camera to right sensor
		if(int(detected.read()) == 3):

			print("DETECTED RIGHT")
			call(["sudo", "servo/servo", "180"])

		print("Taking Picture...")


		# Once camera is in the correct position, take a picture
		takePic()
		time.sleep(1)
		call("pkill \"python3 webServer/web.py\" -f", shell = True)
		call("python3 webServer/web.py &", shell = True)
		
		print("Picture Taken!\n")

		# Close the file containing what sensor detected a person
		detected.close()

	return

# Call the main function
if __name__ == "__main__":
	main()


