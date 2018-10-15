#include "WindMsgQueue.h"
#include "SerialSensor.h"
#include <sstream>
#include <iostream>
#include <stdexcept>      // std::invalid_argument


using namespace std;


class WindSensor : public SerialSensor {
	
public: 
	WindSensor();
	~WindSensor();
	virtual void waitForConfig();
	virtual void measure();
	uint16_t getAngle();
	float getWindSpeed();
	float getTemperature();
	WindMsgQueue msgQueue;
	
private:
	void extractData();
	uint8_t stype;
	uint16_t angle;
	float windSpeed, U, V, W, temperature;
	string buf;
	char byte;
};

WindSensor::WindSensor() {
}

WindSensor::~WindSensor() {
	delete serial;
}

void WindSensor::waitForConfig() {
	while( msgQueue.receiveConfig() != EXIT_SUCCESS );
	stype = msgQueue.getSensorType();
	switch( stype ) {
		case FT205 :
			baudRate = 38400;
			break;
		case TRISONICA :
			baudRate = 115200;
			break;
		case FT742 :
			baudRate = 9600;
			break;
	}
	port = "/dev/ttyUSB" + to_string( msgQueue.getSerialPortNum() );
	added = msgQueue.getSensorStatus();

}

void WindSensor::measure() {
	byte = '\0';
	while( byte != '\r' ) {
		if( serial->read_port( &byte ) != 1 ) {
			cout << "Error reading from windSensor port " << port << endl;
		}
		buf += byte;	
	}				
	byte = '\0';
	extractData();	
}

void WindSensor::extractData() {
	stringstream ss(buf);
	int count = 0;
	
	switch( stype ) {
	case FT742 :
		while ( ss.good() )
		{
			count++;
			string substr;
			getline( ss, substr, ',' );
			if ( count == 2 ) {
				try {
					angle = (uint16_t)stoi( substr );
				}
				catch ( const std::invalid_argument& ia ) {
					std::cerr << "Invalid argument: " << ia.what() << '\n';
				}
			}
			if( count == 4 ) {
				try {
					windSpeed = stof( substr );
				}
				catch ( const std::invalid_argument& ia ) {
					std::cerr << "Invalid argument: " << ia.what() << '\n';
				}
			}
		}
		break;
	case FT205:
  		while( ss.good() )
	    {
	    	count++;
	      string substr;
	      getline( ss, substr, ',' );
	      if( count == 2 ) {
					try {
						windSpeed = stof( substr.substr( 4 ) );
					}
					catch ( const std::invalid_argument& ia ) {
						std::cerr << "Invalid argument: " << ia.what() << '\n';
					}
      	}
      	if( count == 3 ){
					try {
						angle = (uint16_t)stoi( substr );
					}
					catch ( const std::invalid_argument& ia ) {
						std::cerr << "Invalid argument: " << ia.what() << '\n';
					}
      	}
	    }
	  	break;

	case TRISONICA:
		string tmp;
		ss >> windSpeed >> angle >> tmp >> tmp >> tmp >> temperature;
		break;
	}
	buf.clear();
}

uint16_t WindSensor::getAngle() {
	return angle;
}

float WindSensor::getWindSpeed() {
	return windSpeed;
}

float WindSensor::getTemperature() {
	return temperature;
}

