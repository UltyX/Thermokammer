# Thermokammer

Thermokammer Rasbery PI Code


Instructions:

Install the bcm2835-1.52 library.
Run the make command. 
Stop the serial-getty process which may block the serial intput by running  "sudo systemctl stop serial-getty@ttyAMA0.service".


Troubleshooting:
Make sure the ADDA board and the RS232 board are connected to the PI.
Make sure the serial interface is enabled in the PI settings.
