
#include "serial.h"			// header file




Serial::Serial(){
    
    uart0_filestream = -1;
    
        //-------------------------
	//----- SETUP USART 0 -----
	//-------------------------
	//At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively
	//	
	//OPEN THE UART
	//The flags (defined in fcntl.h):
	//	Access modes (use 1 of these):
	//		O_RDONLY - Open for reading only.
	//		O_RDWR - Open for reading and writing.
	//		O_WRONLY - Open for writing only.
	//
	//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
	//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
	//											immediately with a failure status if the output can't be written immediately.
	//
	//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.


// Achim // das no delay geht nicht so wie ich es mit windows getestet habe. geht warscheinlich nur in einer dauer while schleife	
        uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode


  //      uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY );  //Open in read/write mode
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}
	
	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);
}

void Serial::send_string( std::string msg_i ){
        //----- TX BYTES -----
	unsigned char *tx_buffer = new unsigned char[ msg_i.length()+1 ];  // Konvertierung von eingangsstring nach 
	std::strcpy( (char*) tx_buffer, msg_i.c_str()  );                  // unsigned char *tx_buffer[] der länge des strings +1
	
	if (uart0_filestream != -1)
	{
		int count = write(uart0_filestream, &tx_buffer[0], msg_i.length()  );		//Filestream, bytes to write, number of bytes to write
		if (count < 0)
		{
			printf("UART TX error\n");
		}
	}
}


std::string Serial::recive_string(){
        //----- CHECK FOR ANY RX BYTES -----
	std::string nachricht_string = "";
        if (uart0_filestream != -1)
	{
		// Read up to 255 characters from the port if they are there
		unsigned char rx_buffer[256];
		int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);		//Filestream, buffer to store in, number of bytes to read (max)
		if (rx_length < 0)
		{
			//An error occured (will occur if there are no bytes)
                       //printf("Read Error");
                 ;
		}
		else if (rx_length == 0)
		{
			//No data waiting
                       printf("No Data");
		}
		else
		{
			//Bytes received
			rx_buffer[rx_length] = '\0';
                        nachricht_string = std::string(reinterpret_cast<char*>(rx_buffer), 256);
                        //printf(nachricht_string);
                        //printf("%i bytes read : %s\n", rx_length, rx_buffer);

		}
	}
        return nachricht_string;
}


Serial::~Serial(){
	
	//----- CLOSE THE UART -----
	close(uart0_filestream);
}


