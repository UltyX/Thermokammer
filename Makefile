main: main.o pid.o gpio.o
	g++ -o app main.o pid.o gpio.o -lbcm2835
