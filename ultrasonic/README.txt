Ultrasonic Sensor Code

Ultrasonic sensor: LV-EZ0 Maxbotix Ultrasonic Rangefinder
Datasheet: https://www.matbotix.com/documents/LV-MaxSonar-EZ_Datasheet.pdf
Distance Calculation Source: https://www.matbotix.com/tutorials6/032-using-analog-voltage-pin-3.htm

File Descriptors:
getDistance.c: Takes digital output of three ultrasonic sensors (using MCP3008) and outputs distance in inches

Up Next On Ultrasonic Sensor Code:
-> Determine distance change that will trigger servo/camera
	-> bool change;
	-> if change, output string of "left", "right", or "center"
-> Algorithm to determine when change occurs (not noise)
	-> Average distance over a period of time to find actual change
		-> Brainstorming possible number of samples to average
			-> Minimum sample rate of LV-EZ0 at 50ms
			-> Average walking pace 4.6 millifeet/millisecond (1.4 meters/second from Wiki, converted to millifeet/second)
			-> Width of detection cone ~4 feet (from LV-EZ0 datasheet)
			-> Time to go 4ft at average walking pace(feet traveled / average walking pace)(time spent walking by sensor) ~870 ms
			-> About 17 samples in 870 ms (takes 17 samples in the time it takes to walk by sensor)
			-> Depending on how long takes to turn and take picture (from 180 degrees away? 90? Insignificant?), average every 5 samples? 9?
