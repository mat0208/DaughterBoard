#include "OPLS.h"
#include <thread>


int main( int argc, char **argv ) {
	
	OPLS *opls = new OPLS();
	opls->waitForConfig();
	if( !opls->isAdded() )
		return EXIT_SUCCESS;
	opls->init();
	
	usleep( 1000000 );
	thread serialThread( [ opls ]{opls->measure();} );
	
	while( 1 ) {
		if( opls->parseData() ) {
			opls->msgQueue.setOPLSData( opls->getData() );
			opls->msgQueue.sendData();
		}		
	}
	
	//Never reached
	serialThread.join();
	delete opls;
	
	return EXIT_SUCCESS;
}
