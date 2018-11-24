// ECE 471 - Dustin, Derek, Cam - Final Project - servo.c

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
	// Prerequisites:
	// - Edit /boot/config.txt and add: "dtoverllay=pwm"
	// - Reboot

	int pwm_file = open("/sys/class/pwm/pwmchip0", O_WRONLY);
	if (pwm_file < 0) {
		fprintf(stderr, "Error opening export file.\n");
		close(pwm_file);
		return 0;
	}

	write(pwm_file, "0", 1);
	close(pwm_file);

	pwm_file = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
	if (pwm_file < 0) {
                fprintf(stderr, "Error opening duty cycle file.\n");
                close(pwm_file);
                return 0;
        }
	
	// Write an arbitrary value to the duty cycle file for testing.
	write(pwm_file, "3", 1);
	close(pwm_file);
}
