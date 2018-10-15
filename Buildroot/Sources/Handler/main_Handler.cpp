#include "Handler.h"
#include <thread>


int main( int argc, char **argv ) {
	
	Handler *handler = new Handler();

	handler->init();
	
	std::thread heartBeatThread( [ handler ]{handler->heartBeat();} );  //https://stackoverflow.com/questions/31617942/invalid-use-of-void-expression-with-thread-and-an-object
	
	std::cout << "wait for config" << std::endl;
	handler->waitForConfig();
	std::cout << "config received" << std::endl;
	
	while( 1 ) {
		handler->handle();
	}
	
	heartBeatThread.join();
	delete handler;
	
	return EXIT_SUCCESS;
}


