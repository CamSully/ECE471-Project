/* ECE 471 - Dustin, Derek, Cam - Final Project - servo.c
* This program is the main servo code for the project: the function rotateServo() is used to rotate the servo to the angle specified in the command-line argument.
* This code assumes you have already completed the prerequisites seen in README.
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int setupServo(void);
int rotateServo(int);

// main() takes an argument for the rotation angle of the servo: either 0, 90, or 180 degrees.
int main(int argc, char **argv) {

	// Initialize servo motor.
	int result = setupServo();
	if (result == 0) {
		fprintf(stderr, "Error setting up servo.\n");
		return 0;
	}

	int angle = atoi(argv[1]);
	printf("Angle: %d", angle);
	rotateServo(angle);

// This loop is for testing code! Replace when finalized.
/* while (1) {
	rotateServo(-90);
	delay(1);
	rotateServo(0);
	delay(1);
	rotateServo(90);
	delay(1);
} */
	return 0;
}

// Initialize the servo to run on RPi GPIO.
int setupServo(void) {
	
	// Open the PWM export file.
	int pwm_file = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
	if (pwm_file < 0) {
		fprintf(stderr, "Error opening export file.\n");
		close(pwm_file);
		return 0;
	}

	// Export PWM channel 0 to allow it to be used.
	pwm_file = write(pwm_file, "0", 1);
	printf("Export write status: %d\n", pwm_file);
	close(pwm_file);
	
	// Open the PWM channel 0 period file.
	pwm_file = open("/sys/class/pwm/pwmchip0/pwm0/period", O_WRONLY);
	if (pwm_file < 0) {
		fprintf(stderr, "Error opening period file.\n");
		close(pwm_file);
		return 0;
	}

	// Set the PWM period to be 20 ms as specified by the SG90 datasheet.
	pwm_file = write(pwm_file, "20000000", 8);
	printf("Period write status: %d\n", pwm_file);
	close(pwm_file);
	
	return 1;
}

int rotateServo(int angle) {

	char* pulseWidth;

	// Calibrate these values!
	// Idea: use linear regresssion to allow for any angle?
	if (angle == 0) {
		pulseWidth = "1000000"; 
	}
	if (angle == 90) {
		pulseWidth = "1500000";
	}
	if (angle == 180) {
		pulseWidth = "2000000";
	}

	// Disable the PWM0 interface to allow for a config change.
	pwm_file = open("/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY);
	if (pwm_file < 0) {
                fprintf(stderr, "Error opening enable file.\n");
                close(pwm_file);
                return 0;
        }
	pwm_file = write(pwm_file, "0", 1);
	printf("Enable write status: %d\n", pwm_file);
	close(pwm_file);
	
	// Do I need this???
	usleep(10000);

	// Set the duty cycle (pulse width in nanoseconds) of the PWM signal.
	pwm_file = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
	if (pwm_file < 0) {
                fprintf(stderr, "Error opening duty cycle file.\n");
                close(pwm_file);
                return 0;
        }
	pwm_file = write(pwm_file, pulseWidth, 7);
	printf("Duty cycle write status: %d\n", pwm_file);
	close(pwm_file);

	// Do I need this???
	usleep(10000);	

	// Enable the PWM0 interface with the new duty cycle value.
	pwm_file = open("/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY);
	if (pwm_file < 0) {
                fprintf(stderr, "Error opening enable file.\n");
                close(pwm_file);
                return 0;
        }
	pwm_file = write(pwm_file, "1", 1);
	printf("Enable write status: %d\n", pwm_file);
	close(pwm_file);

	return 1;
}
