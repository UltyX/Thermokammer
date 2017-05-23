main: main.o pid.o gpio.o serial.o
	g++ -o app main.o pid.o gpio.o serial.o -std=c++11 -lbcm2835
