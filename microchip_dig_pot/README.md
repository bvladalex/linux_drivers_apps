This is a modified version of the microchip digital potentiometer driver written by Slawomir Stepien that can be found at: https://github.com/torvalds/linux/blob/master/drivers/iio/potentiometer/mcp4131.c.

Initially, the driver made it possbile to write custom wiper position and also read current wiper position. These are 16bit SPI transfers. 
I needed the option of incrementing or decrementing the wiper position without first reading current value and adding or substracting 1 and then performing the 16biy SPI transfer.
My spin of the driver implementation makes use of the built in capability of the MCP dig pot family and uses an 8bit SPI trasnfer to acomplish this. 

The user can select one of the 2 options to increment and decrement wiper position by one:
1) writing 676 or 767 to /sys/bus/iio/iio:deviceX/out_resistanceX_raw
2) Using push-buttons: uses 2 interrupts from gpios declared under the device in the device-tree; below is a snippet from RASPI4 DTB I use; I have 2 push buttons connected to GPIOs 23 and 24:

As you can see I use the mcp4231 10k version but feel free to use any of the supported devices in the drive.
```
digpot: mcp4151@0 {
		compatible = "microchip,mcp4231-103";
		spi-max-frequency = <1000000>;
		spi-cpol;
		spi-cpha;
		reg = <0>;

		bp_up {
			label = "PUSH_KEY_1";
			gpios = <&gpio 23 GPIO_ACTIVE_LOW>;
			trigger = "falling";
		};

		bp_down {
			label = "PUSH_KEY_2";
			gpios = <&gpio 24 GPIO_ACTIVE_LOW>;
			trigger = "falling";
		};
	};
```
