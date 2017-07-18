# Thermokammer

Thermokammer Rasbery PI Code


# Instructions:

Install the bcm2835-1.52 library.

Run the "make" command or
"g++ main.cpp pid.cpp serial.cpp gpio.cpp -std=c++11 -lbcm2835"

Stop the serial-getty process which may block the serial input by running  "sudo systemctl stop serial-getty@ttyAMA0.service".

Run the executable by running "sudo ./app".

# RS232 Commands
Set Target temperature:

echo temp=40.5 > COM1 


Stop the program:

echo ende > COM1 

# Troubleshooting:

Make sure the AD/DA board and the RS232 board are connected to the PI.

Make sure the serial interface is enabled in the PI settings.

