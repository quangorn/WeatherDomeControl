E:\Programs\avr8-gnu-toolchain-win32_x86\bin\avr-size.exe -C --mcu=atmega8 %1
E:\Programs\avr8-gnu-toolchain-win32_x86\bin\avr-objcopy.exe -j .text -j .data -O ihex %1 %1.hex
E:\Programs\AVR\avrdude\avrdude.exe -p m8 -c usbasp -P usb -U flash:w:%1.hex:i