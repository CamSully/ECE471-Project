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
