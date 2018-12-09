#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define LENGTH 3
#define CONSECUTIVE_SAMPLES_LIMIT 10


double getDistance(int channel);
void getBaseline();
void getNoise();
int initializeChaining();

double leftBaseline, rightBaseline, centerBaseline;
double leftNoise, rightNoise, centerNoise;

int main(int argc, char **argv) {

	double distanceLeft, distanceRight, distanceCenter;
	int fd_detection;
	int leftCounter, rightCounter, centerCounter;
	int result;
		
	// Delay for LV-EZ0 start-up
	usleep(1000000);

	// Start chain 
	result = initializeChaining();
	
	// Error check
	if (result == 0){
		return 0;
	}
		
	printf("Getting Baseline...\n");
	getBaseline();
	printf("Got Baseline, Getting Noise...\n");
	getNoise();
	printf("Got Noise");

	// Print out distance measurements from three sensors every 100ms
	while(1){

		// Get distance from ultrasonic sensors
		distanceLeft = getDistance(0);
		distanceRight = getDistance(1);

		distanceCenter = getDistance(2);

		// An error occurred if getDistance() returns 7879
		if((distanceLeft || distanceRight || distanceCenter) == 7879){
			return 0;
		}

		// Print distances from each sensor
		printf("Distance from left sensor: %lf \n", distanceLeft);
		printf("Distance from right sensor: %lf \n", distanceRight);
		printf("Distance from center sensor: %lf \n\n", distanceCenter);


		// Counting number of time distance is less than (baseline - noise)
		if (distanceLeft < (leftBaseline - leftNoise)) {
			leftCounter++;
		}
		else {
			leftCounter = 0;
		}
		if (distanceRight < (rightBaseline - rightNoise)) {
			rightCounter++;
		}
		else {
			rightCounter = 0;
		}
		if (distanceCenter < (centerBaseline - centerNoise)) {
			centerCounter++;
		}
		else 
			centerCounter = 0;

		// If counter is greater than a certain number of samples, then person detected
		if(leftCounter >= CONSECUTIVE_SAMPLES_LIMIT) {

			fd_detection = open("/home/pi/ECE471-Project/detection.txt", O_WRONLY);
			if(fd_detection < 0) {
				printf("ERROR OPENING DETECTION FILE DESCRIPTOR!\n");
				return 0;
			}
			write(fd_detection, "1", 1);
			close(fd_detection);
			usleep(50000);

		}
		if(rightCounter >= CONSECUTIVE_SAMPLES_LIMIT) {

			fd_detection = open("/home/pi/ECE471-Project/detection.txt", O_WRONLY);
			if(fd_detection < 0) {
				printf("ERROR OPENING DETECTION FILE DESCRIPTOR!\n");
				return 0;
			}
			write(fd_detection, "3", 1);
			close(fd_detection);
			usleep(50000);
		}
		if(centerCounter >= CONSECUTIVE_SAMPLES_LIMIT) {

			fd_detection = open("/home/pi/ECE471-Project/detection.txt", O_WRONLY);\
			if(fd_detection < 0) {
				printf("ERROR OPENING DETECTION FILE DESCRIPTOR!\n");
				return 0;
			}
			write(fd_detection, "2", 1);
			close(fd_detection);
			usleep(50000);
		}
		if((leftCounter < CONSECUTIVE_SAMPLES_LIMIT) && (rightCounter < CONSECUTIVE_SAMPLES_LIMIT) && (centerCounter < CONSECUTIVE_SAMPLES_LIMIT)) {
			
			fd_detection = open("/home/pi/ECE471-Project/detection.txt", O_WRONLY);\
			if(fd_detection < 0) {
				printf("ERROR OPENING DETECTION FILE DESCRIPTOR!\n");
				return 0;
			}
			write(fd_detection, "0", 1);
			close(fd_detection);
			usleep(50000);
		}
			 

		// Wait 1 second
		// Minimum sample rate: 50 ms
		// Displays distance every 100ms
		usleep(50000);
	}
	return 0;
}

// Run ultrasonic sensors and calculate distance output from MCP3008 ADC
double getDistance(int channel){

        int spi_fd;
	int result, value;
	int mode = SPI_MODE_0;
	struct spi_ioc_transfer spi;
	double vm, ri, vi;

	// Open SPI device
	spi_fd=open("/dev/spidev0.0",O_RDWR);
	if(spi_fd < 0){
		printf("Error opening SPI device");
		return 7879;
	}

	// Set SPI Mode_0
	result = ioctl(spi_fd,SPI_IOC_WR_MODE,&mode);
	if(result < 0){
		printf("Error setting SPI Mode_0");
		close(spi_fd);
		return 7879;
	}

	// First byte is start bit at 0x00000001
	// Second byte has first 4 bits being single/diff followed by 3 bits of channel you want
	// Third byte is all 0's for padding

	// Second byte: 0x80 is single-ended mode, OR single-ended mode with the channel left-shift 4 to obtain the data output byte.
	unsigned char data_out[LENGTH]={0x1, (0x80 | (channel << 4)), 0x0};
	unsigned char data_in[LENGTH];

	// Clear out transmit buffer with zeros
	memset(&spi,0,sizeof(struct spi_ioc_transfer));

	// Length set to 3
	spi.len			= LENGTH;
	spi.delay_usecs		= 0;
	// Clock speed set to 100kHz
	spi.speed_hz		= 100000;
	// Bits per word set to 8 bits
	spi.bits_per_word	= 8;
	spi.cs_change		= 0;

	// Transmit three bytes and recieve for Channel 0 data
	spi.tx_buf	= (unsigned long)&data_out;
	spi.rx_buf	= (unsigned long)&data_in;

	// Run one full-duplex transaction
	result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);

	// Check for error
	if (result < 0) {
		printf("Error running full-duplex transaction");
		close(spi_fd);
		return 7879;
	}

	/*		Calculating Distance		*/

	// Value of the output voltage is from the first two bits in the second byte and all of the third byte
	value = ((data_in[1] & 0x03) << 8) | data_in[2];

	// Measured voltage as calculated using MCP3008 datasheet equation Vm = (value * V_REF)/1024
	vm = ((float)value * 3.3) / 1024;

	// Analog output of ultrasonic sensor
	// Vi is volts per inch
	// Vi = Vcc(V)/512(inch)
	vi = 5.0 / 512.0;
/*	if (channel == 0) {
		printf("Left voltage: %lf\n", vm);
	}
	if (channel == 1) {
		printf("Right voltage; %lf\n", vm);
	}
	if (channel == 2) {
		printf("Center voltage; %lf\n", vm);
	}
*/
	// Ri is range in inches
	ri = vm / vi;

	// Close file descriptor
	close(spi_fd);

	return ri;
}

// Get the average initial distance reading from the ultrasonic sensors
void getBaseline() {

	double distanceLeft, distanceRight, distanceCenter;
	double leftTotal, rightTotal, centerTotal;
	int i;	

	for(i = 0; i < 100; i++){
		distanceLeft = getDistance(0);
		distanceRight = getDistance(1);
		distanceCenter = getDistance(2);

		leftTotal += distanceLeft;
		rightTotal += distanceRight;
		centerTotal += distanceCenter;		

		usleep(50000);
	}

	leftBaseline = leftTotal / 100.0;
	printf("Left baseline: %lf\n", leftBaseline);
	rightBaseline = rightTotal / 100.0;
	printf("Right baseline: %lf\n", rightBaseline);
	centerBaseline = centerTotal / 100.0;
	printf("Center baseline: %lf\n", centerBaseline);
	
}

// Get average noise for each ultrasonic sensor
void getNoise() {
	
	double distanceLeft, distanceRight, distanceCenter;
	double leftTotal, rightTotal, centerTotal;
	int i;	

	// Sum distances for left, right, and center ultrasonic sensors
	for(i = 0; i < 100; i++){
		distanceLeft = getDistance(0);
		distanceRight = getDistance(1);
		distanceCenter = getDistance(2);

		leftTotal += abs(leftBaseline - distanceLeft);
		rightTotal += abs(rightBaseline - distanceRight);
		centerTotal += abs(centerBaseline - distanceCenter);		

		usleep(50000);
	}

	// Divide by number of samples to get average noise for each ultrasonic sensor
	leftNoise = leftTotal / 100.0;
	printf("Left noise: %lf\n", leftNoise);
	rightNoise = rightTotal / 100.0;
	printf("Left noise: %lf\n", rightNoise);
	centerNoise = centerTotal / 100.0;
	printf("Left noise: %lf\n", centerNoise);

}

// Function to initialize the chaining of the ultrasonic sensors
void initializeChaining(){
	
	int gpio_file = open("/sys/class/gpio/export",O_WRONLY);
	
	// Error check
	if (gpio_file < 0){
		printf("Error opening GPIO file!");
		return 0;
	}
	
	// Enable GPIO21
	write(gpio_file,"21",2);
	// Close gpio_file
	close(gpio_file);
	
	// Set direction to output
	gpio_file = open("/sys/class/gpio/gpio21/direction",O_WRONLY);
	
	// Error check
	if (gpio_file < 0){
		printf("Error setting direction of GPIO 21 file");
		return 0;
	}

	// Write output
	write(gpio_file,"out",3);
	close(gpio_file);

	// Set GPIO21 high
	gpio_file = open("/sys/class/gpio/gpio21/value",O_WRONLY);
	
	// Error check
	if (gpio_file < 0) {
		printf("Error setting GPIO 21 high");
		return 0;
	}
	
	// Write "1" to set GPIO21 high
	write(gpio_file,"1",1);
	close(gpio_file);

	// This delay accounts for the time constant of the input RC circuit. T = 500 us, so 5T = 2.5 ms.
	// This delay must be greater than 2.5 ms to charge RX pin to full voltage. T = tau.
	usleep(5000);

	// Set GPIO21 low
	gpio_file = open("/sys/class/gpio/gpio21/value",O_WRONLY);
	
	// Error check
	if (gpio_file < 0) {
		printf("Error setting GPIO21 low!");
		return 0;
	}

	// Set GPIO21 low
	write(gpio_file,"0",1);
	close(gpio_file);
}





