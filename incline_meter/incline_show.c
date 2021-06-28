/*
 * incline_show.c
 *
 *  Created on: Jun 6, 2021
 *      Author: bvlad
 */

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
//#include <linux/delay.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

//paths for raw acceleration value files as created by the mpu6050 linux driver
#define ACCEL_FILE_ACC_X "/sys/bus/iio/devices/iio:device0/in_accel_x_raw"
#define ACCEL_FILE_ACC_Y "/sys/bus/iio/devices/iio:device0/in_accel_y_raw"
#define ACCEL_FILE_ACC_Z "/sys/bus/iio/devices/iio:device0/in_accel_z_raw"

//path to file created by the lcd linux driver
#define LCD_FILE "/dev/lcd"

//some constants
#define ACC_FS_SENSITIVITY 16384
#define FULL_SCALE_G_VALUE 2
#define PI 				   3.1415926


double x_val,y_val,z_val;
int x_incline,y_incline,z_incline;
char x_to_print[10], y_to_print[10], z_to_print[10];

//declare some special char used to wipe/navigate display
char special_chars[]="\f\n";
char msg[20]="\fIncl in degrees:\n";
//from datasheet of HD44780 controller, this is the hex value that when sent to display it'll display degree char
int deg_char=0xDF;

double get_acc_val(char *fd);
int compute_incline(double axis1, double axis2);
int ret;



double get_acc_val(char *fd){
	int c;
	char Vo[10];
	char *ret_char_ptr;

	FILE *proc = fopen (fd, "r");
	if (!proc) {
		fprintf (stderr, "error: process open failed.\n");
		return 1;
	}
	int i=0;
	//only way I got this to work was to read char by char until EOF
	while(1) {
		c = fgetc(proc);
		if( feof(proc) ) {
			break ;
		}
		if (c != '\0'){
			Vo[i] = c;
			++i;
		}
		Vo[i+1]= '\0';
	}
	fclose(proc);
	return (strtod(Vo,&ret_char_ptr)/ACC_FS_SENSITIVITY);
}

int compute_incline(double axis1, double axis2){
	int incl;
	//derived from composition of forces formula
	incl=(asin(axis1/sqrt(pow(axis1,2)+pow(axis2,2))))*180/PI;

	return incl;
}

int main(){


	char deg_char_c=(char)deg_char;
	while (1){
	//x_val=get_acc_val(ACCEL_FILE_ACC_X);
	//printf("Retrieved accel value for X axis is: %lf.\n", x_val);
	y_val=get_acc_val(ACCEL_FILE_ACC_Y);
	//printf("Retrieved accel value for Y axis is: %lf.\n", y_val);
	z_val=get_acc_val(ACCEL_FILE_ACC_Z);
	//printf("Retrieved accel value for Z axis is: %lf.\n", z_val);
	y_incline=compute_incline(y_val,z_val);
	printf("Incline in degrees on y axis is: %i.\n", y_incline);


	int my_dev=open(LCD_FILE, O_WRONLY);
			if(my_dev<0){
				perror("Fail to open device file: /dev/lcd");
				}else{
					//convert from double to char to concatanate string that we'll display
					gcvt(y_incline,2,y_to_print);
					//concatanate mesage to print + current read value + special char (degree symbol)
					snprintf((msg+18),4,"%s%c",y_to_print,deg_char_c);
					//send a clear screen char
					ret=write(my_dev,special_chars,1);
					if(ret<0){
						perror("Was not able send clear display command.\n");
							}
					//actual write of message
					ret=write(my_dev,msg,21);
					if(ret<0){
						perror("Was not able to transfer all info.\n");
							}
					close(my_dev);

				}
	sleep(1);
	}


	return 0;
}
