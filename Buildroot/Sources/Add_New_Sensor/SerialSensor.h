#include "serial_port.h"
#include <string>
#include <stdint.h>

using namespace std;

class SerialSensor {

public:
	SerialSensor();
	~SerialSensor();
	void init();
	bool isAdded();
	virtual void waitForConfig() = 0;
	virtual void measure()       = 0;

protected:
	virtual void extractData();
	Serial_Port* serial;
	string port;
	int baudRate;
	bool added;
};

SerialSensor::SerialSensor() {
}

SerialSensor::~SerialSensor() {
	delete serial;
}

bool SerialSensor::isAdded() {
	return added;
}

void SerialSensor::init() {
	serial = new Serial_Port( port.c_str(), baudRate, 1 );
	serial->start();
}
