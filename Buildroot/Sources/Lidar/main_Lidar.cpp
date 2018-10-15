#include <iostream>
#include "Lidar.h"
#include <unistd.h>

using namespace std;

int main(int argc, char **argv)
{
	
	Lidar lidar;
	
	#ifdef DEBUG
		cout << "wait for config" << endl;
	#endif
	
	lidar.waitForConfig();
	
	#ifdef DEBUG
		cout << "config received" << endl;
	#endif

	if( !lidar.isAdded() )
		return EXIT_SUCCESS;
	
	if( lidar.init( lidar.LIDAR_I2C_ADDR ) != EXIT_SUCCESS ) {
		cout << "Failed to initialize Lidar" << endl;
		return EXIT_FAILURE;
	}

	usleep( 1000000 );	
	while( 1 ) {
		lidar.measure();
		lidar.msgQueue.setDistance( lidar.getDistance() );
		lidar.msgQueue.sendData();
		#ifdef DEBUG
			cout << lidar.getDistance() << endl;
		#endif
	}
	
	return EXIT_SUCCESS;
}

