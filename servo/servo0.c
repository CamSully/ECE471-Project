// ECE 471 - Dustin, Derek, Cam - Final Project - servo.c

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
	// Prerequisites:
	// - Edit /boot/config.txt and add: "dtoverllay=pwm"
	// - Reboot

	int pwm_file = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
	if (pwm_file < 0) {
		fprintf(stderr, "Error opening export file.\n");
		close(pwm_file);
		return 0;
	}

	pwm_file = write(pwm_file, "0", 1);
	printf("Export write status: %d\n", pwm_file);
	close(pwm_file);
	
	// Set PERIOD
	pwm_file = open("/sys/class/pwm/pwmchip0/pwm0/period", O_WRONLY);
	if (pwm_file < 0) {
		fprintf(stderr, "Error opening period file.\n");
		close(pwm_file);
		return 0;
	}
	// 20 ms PWM period as given in datasheet.
	pwm_file = write(pwm_file, "20000000", 8);
	printf("Period write status: %d\n", pwm_file);
	close(pwm_file);

	// Set DUTY CYCLE
	pwm_file = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
	if (pwm_file < 0) {
                fprintf(stderr, "Error opening duty cycle file.\n");
                close(pwm_file);
                return 0;
        }
	// Datasheet: pw = ~1.5 ms for 0 degrees.
	pwm_file = write(pwm_file, "1500000", 7);
	printf("Duty cycle write status: %d\n", pwm_file);
	close(pwm_file);

	// Enable 
	pwm_file = open("/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY);
	if (pwm_file < 0) {
                fprintf(stderr, "Error opening enable file.\n");
                close(pwm_file);
                return 0;
        }
	pwm_file = write(pwm_file, "1", 1);
	printf("Enable write status: %d\n", pwm_file);
	close(pwm_file);

}
