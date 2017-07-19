#ifndef _serial__
#define _serial__

#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <cstring>
#include <string>

// source http://www.raspberry-projects.com/pi/programming-in-c/uart-serial-port/using-the-uart

class Serial{

private:
	int uart0_filestream;

public:
	
	
	Serial();
	~Serial();
	
	
	void send_string(std::string msg_i);
        std::string recive_string();
	


};

#endif
