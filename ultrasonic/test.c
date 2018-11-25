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
		// Distance of left ultrasonic sensor
		distanceLeft = getDistance(0);
		printf("Distance from left sensor",distanceLeft);

		// Distance of right ultrasonic sensor
		distanceRight = getDistance(1);
		printf("Distance from right sensor",distanceRight);

		// Distance of center ultrasonic sensor
		distanceRight = getDistance(2);
		printf("Distance from center sensor",distanceCenter);

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
	double vin, distance;

	/* Open SPI device */
	spi_fd=open("/dev/spidev0.0",O_RDWR);

	/* Set SPI Mode_0 */
	result = ioctl(spi_fd,SPI_IOC_WR_MODE,&mode);

	if(result < 0){
		printf("Error setting SPI Mode_0");
	}

	// First byte is start bit at 0x00000001
	// Second byte has first 4 bits being single/diff followed by 3 bits of channel you want
	// Third byte is all 0's for padding
	unsigned char data_out[LENGTH]={0x1,channel,0x0};
	unsigned char data_in[LENGTH];

	// Clear out transmit buffer with zeros
	memset(&spi,0,sizeof(struct spi_ioc_transfer));

	// Length set to 3
	spi.len			= LENGTH;
	// Delay
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
	if(result < 0){
		printf("Error running full-duplex transaction");
	}

	// Value of the output voltage is from the first two bits in the second byte and all of the third byte
	value = ((data_in[1] & 0x03) << 8) | data_in[2];

	// Output voltage as calculated using datasheet equation V_IN = (value * V_REF)/1024
	vin = ((float)value*3.3)/1024;

	// Analog output of ultrasonic sensor: (Vcc/512)/inches
	// Distance: distance = (Vin*512)/Vcc
	distance = (vin*512)/3.3;

	return distance;
}
