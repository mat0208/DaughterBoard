#include "I2cSensor.h"
#include "LidarMsgQueue.h"

#define DEBUG

class Lidar : public I2cSensor{
	
public:
	// For registers definitions see: https://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf
	const char LIDAR_I2C_ADDR      = 0x62;
	const char ACQ_COMMAND_REG     = 0x00;
	const char BIAS_CORREC_VAL     = 0x04;
	const char STATUS_REG          = 0x01;
	const char FULL_DELAY_HIGH_REG = 0x0F;
	const char FULL_DELAY_LOW_REG  = 0x10;

	Lidar();
	~Lidar();
	void waitForConfig();
	void measure();
	int getDistance();
	LidarMsgQueue msgQueue;

private:
	int distance;
};


Lidar::Lidar() {
}

Lidar::~Lidar() {
}


void Lidar::waitForConfig() {
	while( msgQueue.receiveConfig() != EXIT_SUCCESS );
	added = msgQueue.getSensorStatus();
}

void Lidar::measure() {
	// Take distance measurement with receiver bias correction
	char buf[] = { ACQ_COMMAND_REG, BIAS_CORREC_VAL };
	i2cWrite( buf, 2 );
	
	// Wait until device is ready for new command
	char buf2[] = { STATUS_REG };
	i2cWrite( buf2, 1 );
	char tmp[] = { 1 };
	while( ( tmp[ 0 ] & 1 ) != 0 ) {
		i2cRead( tmp, 1 );
		usleep( 50000 );
	}
	
	// Read distance measurement high byte
	char hiVal[] = { FULL_DELAY_HIGH_REG };
	i2cWrite( hiVal, 1 );
	i2cRead ( hiVal, 1 );
	
	// Read distance measurement low byte
	char loVal[] = { FULL_DELAY_LOW_REG };
	i2cWrite( loVal, 1 );
	i2cRead ( loVal, 1 );
	
	// Build distance word with high and low bytes
	distance = hiVal[ 0 ];
	distance <<= 8;
	distance += loVal[ 0 ];
}

int Lidar::getDistance() {
	return distance;
}

