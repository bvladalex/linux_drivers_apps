# Small footprint, linux based, incline angle measuring device
Utility for displaying angle of elevation using a lightweight linux setup: orangepi zero + MPU6050 breakout board + hd44780 lcd display.
Both the sensor board and display are connected on the same bus to make the HW setup as light as possible.
The setup can run on a small battery (like the one in the pictures, 2200mAh) for around 16hrs as it has a small power draw of about 130mA. Optimisations can still be made to reduce power draw.

What follows is an implementation guide presented in broad strokes.

## End result is this:
![Image of full setup](https://i.imgur.com/VW0zwLN.jpg?1)
![Image of full setup](https://i.imgur.com/N08In6l.jpg?1)

## HW setup:
- OrangePi zero SBC
- LCD with HD44780 controller
- MPU6050 accelerometer + gyro
- level shifter for I2C signal: accel uses 3.3V, LCD uses 5V
- jumper wires

## SW setup:
### Kernel and DTB setup
- Linux distribution was compiled with [ARMbian](https://github.com/armbian/build) so not to start from scrath with bootloader + kernel + FS:  
- Kernel and DTB need to be recompiled outside ARMbian: cross toolchain can be found on [Linaros website](https://releases.linaro.org/components/toolchain/binaries/latest-7/arm-linux-gnueabihf/).
- we need to make a shallow clone of mainline kernel and then:
- HW config needs to be implemented in the DTB, so we'll have to declare the sensor, the lcd and the I2C port expander used to drive the LCD GPIOs. The I2C bus to which we connect the devices need to be also "okay"-ed in the DTB. The LCD itself is under root in the DTB, the rest are under the corresponding I2C bus. I picked the second one, i2c-1:
```
/ {
.........
  auxdisplay: auxdisplay {
    		compatible = "hit,hd44780";

    		data-gpios = <&pcf8574 4 GPIO_ACTIVE_HIGH>,
                 		<&pcf8574 5 GPIO_ACTIVE_HIGH>,
                 		<&pcf8574 6 GPIO_ACTIVE_HIGH>,
                 		<&pcf8574 7 GPIO_ACTIVE_HIGH>;
    		rs-gpios = <&pcf8574 0 GPIO_ACTIVE_HIGH>;
    		rw-gpios = <&pcf8574 1 GPIO_ACTIVE_HIGH>;
    		enable-gpios = <&pcf8574 2 GPIO_ACTIVE_HIGH>;
    		backlight-gpios = <&pcf8574 3 GPIO_ACTIVE_LOW>;

    		display-height-chars = <2>;
    		display-width-chars = <16>;
	};
};
```
```
&i2c1 {
	status = "okay";
	
	mpu6050@68 {
		compatible = "invensense,mpu6050";
                reg = <0x68>;
		interrupt-parent = <&pio>;
                //interrupts = <16 1>;
	};
	
	 pcf8574: gpio@27 {
            	compatible = "nxp,pcf8574";
            	reg = <0x27>;
            	gpio-controller;
            	#gpio-cells = <2>;
        };
};
```
- DTB need to be compiled with symbols, so please include DTC_FLAGS="-@" when compiling DTB.
- drivers for internal i2c controller, display, MPU6050 sensor and I2c port expander need to be either compiled in the kernel or as modules. I opted for the built-in version because it is more convenient. To achieve this, enter menuconfig in the root of the cloned kernel and search for each component with /, enable it then compile the kernel. They are all in the kernel source tree, which is quite convenient.
- replace the kernel image and dtb in the /boot folder on the storage media you use for the OrangePi.

### Final touches inside linux terminal
- to be able to run this without your intervention by just booting up the board we still need to do something: use crontab to schedule running the small binary that takes the data from the sensor and displays it. But before that, we also need to give non-root users permissions to run it, which is acomplished by implementing a udev rule.
- to grant permissions add a rule with following text (under /etc/udev/rules.d/):
```
KERNEL=="lcd", SUBSYSTEM=="misc", SYMLINK+="my_k", MODE="0666"
```
-add a crontab job with the simple text:
```
@reboot /path_to_compiled_binary
```
## Possible improvements:
- navigate to cell where incline is being written to and update only that as opoosed to the whole display like in current version
- polling to be done by interrupt based timer flag not by actively sleeping
- state machine to display incline on other axis as well as temperature and maybe also gyro values. Changing between the measurements shown should be done by button(s).

## ***Acknowledgements*** : 
- https://yeah.nah.nz/embedded/linux-chardev-lcd/
