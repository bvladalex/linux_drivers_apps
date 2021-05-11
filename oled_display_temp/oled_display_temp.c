/*
 * oled_display_temp.c
 *
 *  Created on: Apr 12, 2020
 *      Author: Vlad Barbalata
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "oled96.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define BUS_ADDRESS 	"/dev/i2c-2"
#define TC74_ADDRESS	0x4d
#define TC74_TMP_REG	0x00
#define TC74_CTL_REG	0x01
#define OLED_ADDRESS 	0x3c

int main(int argc, char *argv[]){
	int fd;
	char temp,tmp0[3];
	char buf[2]={TC74_TMP_REG,TC74_CTL_REG};
	char msg[]="TC74 temp is: ";

	/*open i2c-2 bus*/
	fd=oledInit(2, OLED_ADDRESS, OLED_128x64, 1, 0);

	oledFill(0);
	/*enter infinte loop*/
	while(1){
		/*set TC74 as slave address*/

		if(ioctl(fd,I2C_SLAVE,TC74_ADDRESS))
			perror("Could not set TC74 dev address\n");

		/*read temp to char*/
		if(write(fd,buf,1)<0)
			perror("Error when trying to reach sensor");

		if(read(fd,&temp,1)<0)
			perror("Error when retrieving temp from sensor\n");

		/*set OLED address as slave*/
		if(ioctl(fd,I2C_SLAVE,OLED_ADDRESS)<0)
			perror("Could no re-set OLED I2C dev address");

		/*write temp on screen*/


		snprintf((msg+14),3,"%d%d",temp/10,temp%10);

		oledWriteString(0,0,msg,FONT_NORMAL);


		usleep(100*1000);
	}
	return 0;
}
