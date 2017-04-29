#ifndef serial
#define serial

#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART



class Serial{

private:
	int uart0_filestream = -1;

public:
	
	
	Serial();
	~Serial();
	
	
	void test_send_string();
	void test_recive();
	


};

#endif
