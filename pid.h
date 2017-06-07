#ifndef _PID_H_
#define _PID_H_

// Source: https://gist.github.com/bradley219/5373998


class PID
{
	private:

        double _max;			// max - maximum value of manipulated variable
        double _min;			// min - minimum value of manipulated variable
        double _Kp;				// Kp -  proportional gain
        double _Kd;				// Kd -  derivative gain
        double _Ki;				// Ki -  Integral gain
        double _pre_error;		// soll - ist der letzten berechnung, wird für die ableitung des dt teils verwendet
		double _integral;
	
	
    public:
		
        PID(double max, double min, double Kp, double Kd, double Ki );
		
        // Returns the manipulated variable given a setpoint and current process value
        double calculate(double soll, double ist , double dt);
        ~PID();

    


};

#endif
