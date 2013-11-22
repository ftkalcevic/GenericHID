SRCS=BicolourLED.c BicolourLED.h Common.h Controls.c Controls.h Counter.c Counter.h Descriptors.c Descriptors.h DigitalEncoder.c DigitalEncoder.h DirSwitch.c DirSwitch.h Joystick.c Joystick.h KeyMatrix.c KeyMatrix.h LCD.c LCD.h LCD_SPI.c LCD_SPI.h LED.c LED.h Potentiometer.c Potentiometer.h PWM.c PWM.h RGB.c RGB.h RotarySwitch.c RotarySwitch.h Serial.c Serial.h Switch.c Switch.h TricolourLED.c TricolourLED.h USBKey_Bicolour.h

.PHONY : all
.PHONY : clean

all:	../../../../../bin/usbkey.hex ../../../../../bin/generic16.hex ../../../../../bin/teensypp2.hex

../../../../../bin/usbkey.hex:	$(SRCS)
	make -f makefile.usbkey clean
	make -f makefile.usbkey

../../../../../bin/generic16.hex:	$(SRCS)
	make -f makefile.generic16 clean
	make -f makefile.generic16

../../../../../bin/teensypp2.hex:	$(SRCS)
	make -f makefile.teensypp2 clean
	make -f makefile.teensypp2

clean:
	make -f makefile.usbkey clean
	make -f makefile.generic16 clean
	make -f makefile.teensypp2 clean


