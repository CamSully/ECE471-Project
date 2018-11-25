#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define LENGTH 3

int main(int argc, char **argv) {

        int spi_fd;
	int result, value;
	int mode = SPI_MODE_0;
	struct spi_ioc_transfer spi;
	double vin;

	/* Open SPI device */
	spi_fd=open("/dev/spidev0.0",O_RDWR);

	/* Set SPI Mode_0 */
	result = ioctl(spi_fd,SPI_IOC_WR_MODE,&mode);

	if(result < 0){
		printf("Error setting SPI Mode_0");
	}

	/* Loop forever printing the CH0 and CH1 Voltages 	*/
	/* Once per second.					*/

	// First byte is start bit at 0x00000001
	// Second byte has first 4 bits being single/diff followed by 3 bits of channel you want
	// Third byte is all 0's for padding
	unsigned char data_out[LENGTH]={0x1,0x0,0x0};
	unsigned char data_in[LENGTH];
	while(1){

		// Wait 1 second
		usleep(1000000);

	// Channel 0

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

		// Reset data_out back to Channel 0
		data_out[1] = 0x0;

		// Transmit three bytes and recieve for Channel 0 data
		spi.tx_buf	= (unsigned long)&data_out;
		spi.rx_buf	= (unsigned long)&data_in;

		// Run one full-duplex transaction
		result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);

		// Check for error
		if(result < 0){
			printf("Error running full-duplex transaction");
		}


		// Print values of the bytes output by Channel 0
		//printf("Byte 1: %d\nByte 2: %d\nByte 3:%d\n\n",data_in[0],data_in[1],data_in[2]);

		// Value of the output voltage is from the first two bits in the second byte and all of the third byte
		value = ((data_in[1] & 0x03) << 8) | data_in[2];

		// Output voltage as calculated using datasheet equation V_IN = (value * V_REF)/1024
		vin = ((float)value*3.3)/1024;
		printf("Channel 0 Raw Value: %d\n",value);
		printf("Channel 0 Voltage: %lf\n",vin);


	// Channel 1

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

		// Set Output to Channel 1
		data_out[1] = 0x10;

		// Transmit three bytes and recieve for Channel 0 data
		spi.tx_buf	= (unsigned long)&data_out;
		spi.rx_buf	= (unsigned long)&data_in;

		// Run one full-duplex transaction
		result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);

		// Check for error
		if(result < 0){
			printf("Error running full-duplex transaction");
		}

		// Print values of the bytes output by Channel 1
		//printf("Byte 1: %d\nByte 2: %d\nByte 3:%d\n\n",data_in[0],data_in[1],data_in[2]);

		// Value of the output voltage is from the first two bits in the second byte and all of the third byte
		value = ((data_in[1] & 0x03) << 8) | data_in[2];

		// Output voltage as calculated using datasheet equation V_IN = (value * V_REF)/1024
		vin = ((float)value*3.3)/1024;
		printf("Channel 1 Raw Value: %d\n",value);
		printf("Channel 1 Value: %lf\n",vin);

	// Channel 2

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

		// Set Output to Channel 2
		data_out[1] = 0x11;

		// Transmit three bytes and recieve for Channel 0 data
		spi.tx_buf	= (unsigned long)&data_out;
		spi.rx_buf	= (unsigned long)&data_in;

		// Run one full-duplex transaction
		result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);

		// Check for error
		if(result < 0){
			printf("Error running full-duplex transaction");
		}

		// Print values of the bytes output by Channel 1
		//printf("Byte 1: %d\nByte 2: %d\nByte 3:%d\n\n",data_in[0],data_in[1],data_in[2]);

		// Value of the output voltage is from the first two bits in the second byte and all of the third byte
		value = ((data_in[1] & 0x03) << 8) | data_in[2];

		// Output voltage as calculated using datasheet equation V_IN = (value * V_REF)/1024
		vin = ((float)value*3.3)/1024;
		printf("Channel 2 Raw Value: %d\n",value);
		printf("Channel 2 Value: %lf\n\n",vin);


	}
	/* Use the SPI_IOC_MESSAGE(1) ioctl() as described in the class notes */

	return 0;
}
