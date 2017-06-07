 
#include "pid.h"
#include "gpio.h"
#include "serial.h"
#include <stdio.h>
#include <iostream>


#include <vector>   // Daten zwischenspeicher
#include <fstream>  // Datei zugriff

#include <unistd.h> // sleep
#include <chrono>   // clock time
using namespace std;


void write_data_to_csv(vector<float> time, vector<float> voltage);

int main() {
    float target_temperature    = 30.0;
    double messdauer 		= 60*60*24; // messdauer in Sekunden
    string seriel_nachricht 	= "";	 // Nachrit über das RS232 Kabel
    vector<float> time_vec;     	 // Zeitvector
    vector<float> voltage_vec;  	 // Spannungsvector
    vector<float> temperature_vec;  	 // Temperaturvector

    PID pid_0    		= PID( 5, 0, 0.1, 0.01, 0.5);                           // PID-Regler.
    ADDA_GPIO gpio_0 		= ADDA_GPIO();                    			// AD und DA wandler Bord Klasse die über die bcm2835 und die beispiel Funktionen auf das Board zugreift.
    Serial my_serial_instance   = Serial();						// RS232 Connection //    while(1){my_serial_instance.recive_string();}



    // Zeit vars
    std::chrono::duration<double> elapsed_seconds, dt;                          // Differenz zeitpunkt
    std::chrono::time_point<std::chrono::system_clock> start, dt_stamp;         // Zeitpunkte 1, 2
    start       = std::chrono::system_clock::now();                             // Start zeitpunkt setzen
    dt_stamp    = std::chrono::system_clock::now();                             // Start zeitpunkt setzen

    // Zeit vars


    do{

//        seriel_nachricht = my_serial_instance.recive_string();      // RS232 String auslesen


        voltage_vec.push_back( gpio_0.get_AD_voltage(3) );                                  // Spannungs - Messwert in vector abspeichern
        temperature_vec.push_back( convert_voltage_to_temperature( voltage_vec.back() ) );  // Temperatur- Messwert in vector abspeichern

        dt          = std::chrono::system_clock::now() - dt_stamp;
        dt_stamp    = std::chrono::system_clock::now();
        set_heating_pads( pid_0.calculate( target_temperature, temperature_vec.back(),dt.count() ) );


        elapsed_seconds = dt_stamp - start;                                                     // Differenz bilden
        time_vec.push_back( elapsed_seconds.count() );                                          // Zeit in vector abspeichern

        std::cout << time_vec.back()<< " Sec.   " << voltage_vec.back() << " V   "<< temperature_vec.back() << " °C" << std::endl;



    }while( ( time_vec.back() < messdauer) && !(my_serial_instance.recive_string() ) ); // Vergleich die zuletzt gemessene zeit mit der Gesammtmessdauer und brich ab wenn diese größer ist.
    
    
    
    


    write_data_to_csv(time_vec, voltage_vec );      // Messdaten in CSV Datei schreiben
    
    

    return 0;
}

void set_peltie(float voltage_on = 0.0){

    if (voltage_on > 5.0){
        voltage_on = 5.0;
    }
    else if (voltage_on < 0.0){
        voltage_on = 0.0;
    }

    gpio_0.set_output_voltage(1, voltage_on);

}

void set_heating_pads(float voltage_on = 0.0){

    if (voltage_on > 5.0){
        voltage_on = 5.0;
    }
    else if (voltage_on < 0.0){
        voltage_on = 0.0;
    }

    gpio_0.set_output_voltage(0, voltage_on);

}

void convert_voltage_to_temperature(float voltage_i){
    return 3.04948162555188+16,1414944544373*voltage_i;
}

// Erhällt 2 Vectornen und speichert diese als Komma Tabelle ab. Richtet sich dabei nach der länge des Zeitvectors
void write_data_to_csv(vector<float> time, vector<float> voltage) {

    int length = time.size();
    ofstream outfile;
    outfile.open("result.csv",fstream::out | fstream::trunc);




    for (int i=0; i< length; i++ )
    {

        outfile << time[i];
        outfile << ", ";
        outfile << voltage[i];
        outfile << endl;
    }


    outfile.close();

}
