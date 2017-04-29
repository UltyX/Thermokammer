main: main.o pid.o gpio.o serial.o
	g++ -o app main.o pid.o gpio.o serial.o -lbcm2835
