#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define LENGTH 3

double getDistance(int channel);

int main(int argc, char **argv) {

	double distanceLeft, distanceRight, distanceCenter;

	// Print out distance measurements from three sensors every second
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

		// Wait 1 second
		usleep(1000000);
	}

	return 0;
}

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

	// Ri is range in inches
	ri = vm / vi;

	return ri;
}
