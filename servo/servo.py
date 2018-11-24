# ECE 471 - Dustin, Derek, Cam - Final Project - servo.py
# This code works! It's in Python though... we would get a better grade for low-level C?
# Interesting feature of using this Python library: you don't need to be root to run the servo code.

import RPi.GPIO as IO
import time

try:
	IO.setmode(IO.BCM)
	IO.setup(18,IO.OUT)
	p = IO.PWM(18,100)
	p.start(0)

	while 1:
	    for x in range (50):
	        p.ChangeDutyCycle(x)
	        time.sleep(0.01)
    
	    for x in range (50):
	        p.ChangeDutyCycle(50-x)
	        time.sleep(0.01)

except:
	print "Error"

finally:
	IO.cleanup()
