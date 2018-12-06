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
from flask import Flask, render_template


def takePic():

	pictureLocation = "/home/pi/ECE471-Project/camera/pics/"
	
	fileNum = 0
	exists = True

	# Check if previous image exists
	if(os.path.exists(pictureLocation + "detected.jpg") == 0):
		detectionPicture = "detection.jpg"
		exists = False		
	
	while(exists):
		if(os.path.exists(pictureLocation + "previous%d.jpg", % fileNum):
			fileNum += 1
		else:
			detectionPicture = "previous%d.jpg", % fileNum
		
		call(["mv", "detected.jpg", detectionPicture])
		detectionPicture = "detected.jpg"


	camera = PiCamera()
	camera.vflip = True   
	camera.start_preview()						# Turn on Camera and preview
	time.sleep(1)							# Wait 5 seconds to focus
	camera.capture(pictureLocation + detectionPicture)	# Where to save pics
	camera.stop_preview()						# Stop the preview

	return


def webServer():

	app = Flask(__name__)

	@app.route('/')

	def index():

		return render_template('index.html')

	
	app.run(debug = True, host = 0.0.0.0)	
	
	return
	
def main():
	
	print("Starting Web Server...\n")
	
	# Start the web server
	webServer()
	
	print("Web Server Started\n")

	print("Starting Ultrasonic Sensors\n")

	# Start the ultrasonic sensors
	call(["sudo", "ultrasonic/distance&"]

	print("Ultrasonic Sensors Started\n")

	# Infinite loop to detect a person and take a picture of them
	while(1):
	
		# Open file containing what sensor detected a person
		detected = open("detection.txt", "r")
		
		# Busy loop to wait until someone is detected
		while(detected.read() == 0):
			print("No one Detected\n")

		# If the value in the file is left, move camera to left sensor
		if(detected.read() == str(left)):
		
			print("DETECTED LEFT\n")
			call(["sudo", "servo/servo", "180"])

		# If the value in the file is center, move camera to center sensor
		if(detected.read() == str(center)):
	
			print("DETECTED CENTER\n")
			call(["sudo", "servo/servo", "90"])		

		# If the value in the file is right, move camera to right sensor
		if(detected.read() == str(right)):	  

			print("DETECTED RIGHT\n")
			call(["sudo", "servo/servo", "0"])

		print("Taking Picture...")
		
		# Once camera is in the correct position, take a picture
		takePic()
	
		print("Picture Taken\n")
		
		# Close the file containing what sensor detected a person
		detected.close()	
	
	return

# Call the main function
if __name__ == "__main__":
	main()


