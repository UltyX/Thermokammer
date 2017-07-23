 
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


void write_data_to_csv(vector<float> time, vector<float> voltage, vector<float> temperature);

void set_peltie(        ADDA_GPIO* gpio_0  ,float voltage_on);
void set_heating_pads(  ADDA_GPIO* gpio_0  ,float voltage_on);
float convert_voltage_to_temperature(float voltage_i);

float r_pad_voltage_info  = -1.0; //
float peltie_voltage_info = -1.0; //

int main() {
    float target_temperature    = 30.0;     // soll temperatur
    double messdauer 		= 60*60*24; // messdauer in Sekunden

    bool pid_regelung           = true;

    string seriel_nachricht 	= "";	 // Nachrit über das RS232 Kabel
    string send_seriel_nachricht= "";	 // Nachrit über das RS232 Kabel
    int soll_temperatur_index   = 0;
    

    vector<float> time_vec;     	 // Zeitvector
    vector<float> voltage_vec;  	 // Spannungsvector
    vector<float> temperature_vec;  	 // Temperaturvector

    PID pid_0    		= PID( 5, -5, 14.29, 40.45 , 1/1222.1);                 // PID-Regler. BOP  -5 V bis 5 V | p=0 d=0 i=1/90000
    ADDA_GPIO gpio_0 		= ADDA_GPIO();                    			// AD und DA wandler Bord Klasse die über die bcm2835 und die beispiel Funktionen auf das Board zugreift.
    Serial my_serial_instance   = Serial();						// RS232 Connection //    while(1){my_serial_instance.recive_string();}

    set_heating_pads( &gpio_0  , 0.0 );  // Erstmal alles ausschalten
    set_peltie( &gpio_0  , 0.0 );        // Erstmal alles ausschalten

    // Zeit vars
    std::chrono::duration<double> elapsed_seconds, dt;                          // Differenz zeitpunkt
    std::chrono::time_point<std::chrono::system_clock> start, dt_stamp;         // Zeitpunkte 1, 2
    start       = std::chrono::system_clock::now();                             // Start zeitpunkt setzen
    dt_stamp    = std::chrono::system_clock::now();                             // Start zeitpunkt setzen

    // Zeit vars

    float temp_regelwert = 0.0;
    do{

        seriel_nachricht = my_serial_instance.recive_string();      // RS232 String auslesen
        seriel_nachricht = seriel_nachricht.substr(0, seriel_nachricht.find("\n") ); 

        soll_temperatur_index = seriel_nachricht.find("temp=");     // Nachricht nach soll temperatur angabe durchsuchen
        if (soll_temperatur_index != std::string::npos){            // Neue Soll-Temperatur empfangen
            std::cout << seriel_nachricht << std::endl; //debug
            std::cout << seriel_nachricht.substr(soll_temperatur_index+5 ) << std::endl; //debug
            target_temperature = std::stof( seriel_nachricht.substr( soll_temperatur_index+5 )  ); // Neue Temperatur setzen. Punkt als komma verwenden !
            std::cout << "target_temperature"<<target_temperature << " °C " << std::endl; //debug
        }
        
        if (seriel_nachricht.find("pid") != std::string::npos){    // auf pid regelung umstellen
            pid_regelung = true;
            std::cout << "pid" << std::endl; //debug
        }
        if (seriel_nachricht.find("2p") != std::string::npos){     // auf 2 punkt regelung umstellen
            pid_regelung = false;
            std::cout << "2p" << std::endl; //debug
        }

        voltage_vec.push_back( gpio_0.get_AD_voltage(3) );                                  // Spannungs - Messwert in vector abspeichern
        temperature_vec.push_back( convert_voltage_to_temperature( voltage_vec.back() ) );  // Temperatur- Messwert in vector abspeichern

        dt          = std::chrono::system_clock::now() - dt_stamp;  // dt für regelung
        dt_stamp    = std::chrono::system_clock::now();             // zeitpunkt für nächste dt berechnung merken

        temp_regelwert = pid_0.calculate( target_temperature, temperature_vec.back(),  dt.count() );  // aktuellen regelwert berechnen

        if (pid_regelung){      //PID

            if ( temp_regelwert > 0.0  ){                           // wenn regelgröße positiv
                set_heating_pads( &gpio_0  , temp_regelwert );      // wärme pads on
                set_peltie( &gpio_0  , 0.0 );                       // peltie off
            }
            else{                                                   // wenn negativ
                set_peltie( &gpio_0  , (-1*temp_regelwert) );       // peltie on
                set_heating_pads( &gpio_0  , 0.0 );                 // wärme pads off
            }

        }
        else{                 //2 Punkt

            if ( temperature_vec.back() < target_temperature ){    // 2 Punkt Regelung/ Ein
                set_heating_pads( &gpio_0  , 5.0 );
                set_peltie( &gpio_0  , 0.0 );
            }    
            else{                                                  // 2 Punkt Regelung/ Aus
                set_heating_pads( &gpio_0  , 0.0 );
                set_peltie( &gpio_0  , 5.0 );
            }
        }

        elapsed_seconds = dt_stamp - start;                                                     // Differenz bilden
        time_vec.push_back( elapsed_seconds.count() );                                          // Zeit in vector abspeichern

        send_seriel_nachricht = std::to_string(time_vec.back()) + " Sec. " + std::to_string( temperature_vec.back() ) + " °C \n";
        my_serial_instance.send_string(send_seriel_nachricht);

        std::cout << time_vec.back()<< " Sec.   " << voltage_vec.back() << " V   "<< temperature_vec.back() << " °C  " 
                  << r_pad_voltage_info << " V U_pads " << peltie_voltage_info << " V U_peltie "<< temp_regelwert << " V U_regelwert " <<std::endl;


    // Als sainity check, soll bei t >= 80.0°C abbrechen 
    // Vergleich die zuletzt gemessene zeit mit der Gesammtmessdauer und brich ab wenn diese größer ist, oder das ende signal gesendet wurde.

    }while(  (seriel_nachricht.find("ende") == std::string::npos ) && ( temperature_vec.back() < 100.0 ) ); // ( time_vec.back() < messdauer) &&

    
    set_heating_pads( &gpio_0  , 0.0 );    // Program ende ausgabe abschalten
    set_peltie( &gpio_0  , 0.0 );          // Program ende ausgabe abschalten
    
    


    write_data_to_csv(time_vec, voltage_vec, temperature_vec );      // Messdaten in CSV Datei schreiben
    
    

    return 0;
}



void set_peltie(ADDA_GPIO* gpio_0  ,float voltage_on = 0.0){

    if (voltage_on > 5.0){
        voltage_on = 5.0;
    }
    else if (voltage_on < 0.0){
        voltage_on = 0.0;
    }
    if (peltie_voltage_info == voltage_on){ // neu setzen des gleichen wertes überspringen
        return;
    }

    gpio_0->set_output_voltage(0, voltage_on);
    peltie_voltage_info = voltage_on;
    std::cout << "set_peltie "<< voltage_on<< " V" << std::endl; //debug
}

void set_heating_pads(ADDA_GPIO* gpio_0  ,float voltage_on = 0.0){

    if (voltage_on > 5.0){
        voltage_on = 5.0;
    }
    else if (voltage_on < 0.0){
        voltage_on = 0.0;
    }
    if (r_pad_voltage_info == voltage_on){ // neu setzen des gleichen wertes überspringen
        return;
    }

    gpio_0->set_output_voltage(1, voltage_on);
    r_pad_voltage_info = voltage_on;
    std::cout << "set_heat_pads "<< voltage_on<< " V" << std::endl; //debug
}

float convert_voltage_to_temperature(float voltage_i){
    return 3.04948162555188+16.1414944544373*voltage_i;
}

// Erhällt 2 Vectornen und speichert diese als Komma Tabelle ab. Richtet sich dabei nach der länge des Zeitvectors
void write_data_to_csv(vector<float> time, vector<float> voltage, vector<float> temperature ) {

    int length = time.size();
    ofstream outfile;
    outfile.open("result.csv",fstream::out | fstream::trunc);




    for (int i=0; i< length; i++ )
    {

        outfile << time[i];
        outfile << ", ";
        outfile << voltage[i];
        outfile << ", ";
        outfile << temperature[i];
        outfile << endl;
    }


    outfile.close();

}
