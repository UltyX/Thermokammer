#ifndef _serial__
#define _serial__

#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART



class Serial{

private:
	int uart0_filestream;

public:
	
	
	Serial();
	~Serial();
	
	
	void send_string();
	void recive_string();
	


};

#endif
