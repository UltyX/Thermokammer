 
#include "pid.h"
#include "gpio.h"
#include "serial.h"
#include <stdio.h>
#include <iostream>

int main() {

//    PID pid_0 			= PID(0.1, 100, -100, 0.1, 0.01, 0.5);			// PID-Regler.
    ADDA_GPIO gpio_0 		= ADDA_GPIO();                    				// AD und DA wandler Bord Klasse die über die bcm2835 und die beispiel Funktionen auf das Board zugreift.
    Serial my_serial_instance 	= Serial();										// RS232 Connection


//    while(1){my_serial_instance.recive_string();}

	
    for (int i = 0; i<6; i++){
        gpio_0.set_output_voltage(0, i);
        std::cout << gpio_0.get_AD_voltage(3) <<std::endl;
        my_serial_instance.recive_string();
    }
    
    
    
    
    
    
    
    /*
    double val = 20;
    for (int i = 0; i < 100; i++) {
        double inc = pid.calculate(0, val);
        printf("val:% 7.3f inc:% 7.3f\n", val, inc);
        val += inc;
    }
    
    int a;
    while(1){
        //std::cin >> a;
        gpio_0.set_output_voltage(5.0);
        //gpio_0.get_AD_voltage();
    }*/
    return 0;
}
