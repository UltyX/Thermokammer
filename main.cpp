 
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




int main() {

    double messdauer = 60*5;    // messdauer in Sekunden

    vector<float> time_vec;     // Zeitvector
    vector<float> voltage_vec;  // Spannungsvector

    //PID pid_0 		= PID(0.1, 100, -100, 0.1, 0.01, 0.5);                  // PID-Regler.
    ADDA_GPIO gpio_0 		= ADDA_GPIO();                    			// AD und DA wandler Bord Klasse die über die bcm2835 und die beispiel Funktionen auf das Board zugreift.
    //Serial my_serial_instance = Serial();						// RS232 Connection //    while(1){my_serial_instance.recive_string();}



    // Zeit vars
    std::chrono::time_point<std::chrono::system_clock> start, end;  // Zeitpunkte 1, 2
    start = std::chrono::system_clock::now();                       // Start zeitpunkt setzen
    std::chrono::duration<double> elapsed_seconds;                  // Differenz zeitpunkt
    // Zeit vars


    do{
        //gpio_0.set_output_voltage(0, i);                          // Ausgansspannung auf DA Kanal 0 setzen
        //my_serial_instance.recive_string();                       // RS232 String auslesen

        voltage_vec.push_back( gpio_0.get_AD_voltage(3) );          // Messwert in vector abspeichern
        end = std::chrono::system_clock::now();                     // Endzeitpunkt setzen
        elapsed_seconds = end-start;                                // Differenz bilden
        time_vec.push_back( elapsed_seconds.count() );              // Zeit in vector abspeichern

    }while( time_vec.back() < messdauer);                           // Vergleich die zuletzt gemessene zeit mit der Gesammtmessdauer und brich ab wenn diese größer ist.
    
    
    
    


    write_data_to_csv(time_vec, voltage_vec );      // Messdaten in CSV Datei schreiben
    
    

    return 0;
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
