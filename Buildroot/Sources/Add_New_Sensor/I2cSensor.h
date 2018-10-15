#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>


class I2cSensor {

public:
 	const char* I2C_FILENAME = "/dev/i2c-1";
	I2cSensor();
	~I2cSensor();
	int init( char i2cAddr );
	virtual void waitForConfig() = 0;
	virtual void measure() = 0;
	bool isAdded();

protected:
	int fd; // file descriptor for /dev/i2c-1
	void i2cWrite( char* buf, int nbBytes );
	void i2cRead ( char* buf, int nbBytes );
	bool added;
};

I2cSensor::I2cSensor() {
}

I2cSensor::~I2cSensor() {
}

bool I2cSensor::isAdded() {
	return added;
}

int I2cSensor::init( char i2cAddr ) {
  if( ( fd = open( I2C_FILENAME, O_RDWR ) ) < 0 ) {
    std::cout << "Failed to open the i2c bus" << std::endl;
    return EXIT_FAILURE;
  }
  if( ioctl( fd, I2C_SLAVE, i2cAddr ) < 0 ) {
    std::cout << "Failed to acquire bus access and/or talk to slave." << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;	
}

void I2cSensor::i2cWrite( char* buf, int nbBytes ) {
  if( write( fd, buf, nbBytes ) != nbBytes ) {
    /*ERROR HANDLING: i2c transaction failed*/
    std::cout << "Failed to write to the i2c bus." << std::endl;
  }
}

void I2cSensor::i2cRead( char* buf, int nbBytes ) {
  if( read( fd, buf, nbBytes ) != nbBytes ) {
    /*ERROR HANDLING i2c transaction failed*/
    std::cout << "Failed to read from i2c bus." << std::endl;
  }
}
