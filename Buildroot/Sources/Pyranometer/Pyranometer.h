#include "SerialSensor.h"
#include "PyranometerMsgQueue.h"
#include <sstream>
#include <iostream>
#include <stdexcept>      // std::invalid_argument


//Pyranometer SP-212 from apogee Instruments

class Pyranometer : public SerialSensor {
	
public:
	Pyranometer();
	~Pyranometer();
	void waitForConfig();
	void measure();
        float getSolarIrradiance();
	PyranometerMsgQueue msgQueue;
	
private:
	void extractData();
	float solarIrradiance;
	string buf;
	char byte;	
};

Pyranometer::Pyranometer() {
}

Pyranometer::~Pyranometer() {
}

void Pyranometer::waitForConfig() {
	while( msgQueue.receiveConfig() != EXIT_SUCCESS );
	port = "/dev/ttyUSB" + to_string( msgQueue.getSerialPortNum() );	
	baudRate = 9600;
	added = msgQueue.getSensorStatus();
}

void Pyranometer::measure() {
	byte = '\0';
	while( byte != '\r' ) {
		if( serial->read_port( &byte ) != 1 ) {
			cout << "Error reading from windSensor port " << port << endl;
		}
		buf += byte;	
	}
	byte = '\0';
	buf = buf.substr( 1 );
	extractData();
}

void Pyranometer::extractData() {
	try {
		solarIrradiance = (float)stof( buf );
	}
	catch ( const std::invalid_argument& ia ) {
		std::cerr << "Invalid argument: " << ia.what() << '\n';
		solarIrradiance = -1;
	}

	buf.clear();
}

float Pyranometer::getSolarIrradiance() {
	return solarIrradiance;
}
