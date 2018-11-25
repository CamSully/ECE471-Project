Getting Distance With Ultrasonic Sensors:

Files:

test.c: 
	What I Did:	Tried to make a function to return the distance output of an ultrasonic sensor when you input the channel it's connected to.

	What Happened:	So far, nothing.

distance.c: 
	What I Did:	Similar to the code in homework 7, but with an additional channel added. The voltage input of the ADC is used to calculate the distance.
			On the datasheet for the sensor it had the analog output as: (Vcc/512)/inches. I assumed that it equaled Vin, as it didn't say it outright. 
			Solved for inches(assumed that was a variable and not units). Got distance = (Vin*512)/Vcc. I then set Vcc = 3.3V.

	Issues:		Lotta assumptions concerning distance equation.

	What Happened:	Prints out beautifully, but all the values are 0. I'm pretty sure something's wrong with my ADC.

