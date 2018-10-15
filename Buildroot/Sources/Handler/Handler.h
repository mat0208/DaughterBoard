#include <string>
#include <iostream>
#include <sstream>
#include <list>  
#include <sys/time.h>
#include <time.h>
#include "MAVLinkSerial_port.h"
#include "LidarMsgQueue.h"
#include "WindMsgQueue.h"
#include "PyranometerMsgQueue.h"
#include "OPLSMsgQueue.h"


#define DEBUG

using namespace std;


class Handler{
	
 public:
  const char* XBeePort = "/dev/ttyUSB0";
  const int baudRate   = 38400;
  const int DAUGHTER_BOARD_SYSID = 32;
  //#define SENSOR_OK             0
  //#define SENSOR_NOT_RESPONDING 0xFF
	
  Handler();
  ~Handler();
  void init();
  void handle();
  void waitForConfig();
  void heartBeat();  
	
 private:	
  LidarMsgQueue lidarMsgQueue;
  WindMsgQueue windMsgQueue;
  PyranometerMsgQueue pyranometerMsgQueue;
  OPLSMsgQueue oplsMsgQueue;
	
  void receiveMsgQueue();
  void buildMAVMsg();              
  void sendMAVMsg();	
  void sendConfigToSensors(); 

  struct timeval tv;
  int32_t sec, usec;
  void setTime();
	
  MAVLinkSerial_Port* MAVSerial;
  mavlink_message_t heartBeatMsg, configMsg, windMsg, lidarMsg, pyranometerMsg, oplsMsg;
  mavlink_config_t cfg;
  bool windBuild, lidarBuild, pyranometerBuild, oplsBuild, windSend, lidarSend, pyranometerSend, oplsSend;
  uint16_t angle;
  float windSpeed;
  float temperature;
  uint16_t distance;
  float solarIrradiance;
  oplsData oplsdata;
  uint8_t status;
};


Handler::Handler() {
}

Handler::~Handler() {
  MAVSerial->close_serial();
  delete MAVSerial;
}

void Handler::init() {
  MAVSerial = new MAVLinkSerial_Port( XBeePort, baudRate, 0 );
  MAVSerial->start();
  windBuild        = false;
  lidarBuild       = false;
  pyranometerBuild = false;
  oplsBuild        = false;
  windSend         = false;
  lidarSend        = false;
  pyranometerSend  = false;
  oplsSend         = false;
}


void Handler::waitForConfig() {
  while( 1 ) {
    if( MAVSerial->read_message( &configMsg ) ) {
      if( configMsg.msgid == MAVLINK_MSG_ID_CONFIG ) {
	mavlink_msg_config_decode( &configMsg, &cfg );
	sendConfigToSensors();
	return;
      }
    }
    cout << "no config received" << endl;
  }
}	

void Handler::handle() {
  receiveMsgQueue();
  if( lidarBuild || windBuild || pyranometerBuild || oplsBuild ) {
    buildMAVMsg();
  }
  if( lidarSend || windSend || pyranometerSend || oplsSend ) {
    sendMAVMsg();
  }
}


void Handler::buildMAVMsg() {
  // Wind Sensor
  if( windBuild ) {
#ifdef DEBUG
    cout << "Wind : " << angle << " " << windSpeed << " " << temperature << endl;
#endif
    mavlink_msg_wind_sensor_pack( DAUGHTER_BOARD_SYSID, 1, &windMsg, sec, usec, angle, windSpeed, temperature, 0 ); 
    windSend = true;
    windBuild = false;
  }else {
    windSend = false;
  }
  // Lidar
  if( lidarBuild ) {
#ifdef DEBUG
    cout << "Lidar : " << distance << endl;
#endif
    mavlink_msg_lidar_pack( DAUGHTER_BOARD_SYSID, 1, &lidarMsg, sec, usec, distance, 0 ); 
    lidarSend = true;
    lidarBuild = false;
  }else {
    lidarSend = false;
  }
  // Pyranometer
  if( pyranometerBuild ) {
#ifdef DEBUG
    cout << "PY : " << sec << " " << solarIrradiance << endl;
#endif
    mavlink_msg_pyranometer_pack( DAUGHTER_BOARD_SYSID, 1, &pyranometerMsg, sec, usec, solarIrradiance, 0 ); 
    pyranometerSend = true;
    pyranometerBuild = false;
  }else {
    pyranometerSend = false;
  }
  // OPLS
  if( oplsBuild ) {
    mavlink_msg_opls_pack( DAUGHTER_BOARD_SYSID, 1, &oplsMsg, sec, usec, oplsdata.time_, oplsdata.ch4,
			   oplsdata.et , oplsdata.h2o, oplsdata.p, oplsdata.t, oplsdata.rf, 
			   oplsdata.lon, oplsdata.lat, oplsdata.lsr ); 
    oplsSend  = true;
    oplsBuild = false;
  }else {
    oplsSend = false;
  }

}

void Handler::sendMAVMsg() {
  // Wind sensor
  if( windSend ) {
    MAVSerial->write_message( &windMsg );
    windSend = false;
  }
  // Lidar
  if( lidarSend ) {
    MAVSerial->write_message( &lidarMsg );
    lidarSend = false;
  }
  // Pyranometer
  if( pyranometerSend ) {
    MAVSerial->write_message( &pyranometerMsg );
    pyranometerSend = false;
  }
  // OPLS
  if( oplsSend ) {
    MAVSerial->write_message( &oplsMsg );
    oplsSend = false;
  }


}

void Handler::receiveMsgQueue() {	
  // Wind sensor
  if( windMsgQueue.receiveData() == EXIT_SUCCESS ) {
    setTime();
    windBuild = true;
    angle = windMsgQueue.getAngle();
    windSpeed = windMsgQueue.getSpeed();
    temperature = windMsgQueue.getTemperature();
  }else { 
    windBuild = false;
  }
  // Lidar
  if( lidarMsgQueue.receiveData() == EXIT_SUCCESS ) {
    setTime();
    lidarBuild = true;
    distance = lidarMsgQueue.getDistance();
    std::cout << "distance : " << distance << std::endl;
  }else {
    lidarBuild = false;
  }
  // Pyranometer
  if( pyranometerMsgQueue.receiveData() == EXIT_SUCCESS ) {
    setTime();
    pyranometerBuild = true;
    solarIrradiance = pyranometerMsgQueue.getSolarIrradiance();
  }else {
    pyranometerBuild = false;
  }
  // OPLS
  if( oplsMsgQueue.receiveData() == EXIT_SUCCESS ) {
    setTime();
    oplsBuild = true;
    oplsdata  = oplsMsgQueue.getOPLSData();
  }else {
    oplsBuild = false;
  }

}

void Handler::sendConfigToSensors() {
  windMsgQueue.setSensorStatus( cfg.windSensorStatus );
  windMsgQueue.setSensorType( cfg.windSensorType );
  windMsgQueue.setSerialPortNum( cfg.windSensorComPortNum );
  lidarMsgQueue.setSensorStatus( cfg.lidarStatus );
  pyranometerMsgQueue.setSensorStatus( cfg.pyranometerStatus );
  pyranometerMsgQueue.setSerialPortNum( cfg.pyranometerComPortNum );
  oplsMsgQueue.setSensorStatus( cfg.oplsStatus );
  oplsMsgQueue.setSerialPortNum( cfg.oplsComPortNum );
  windMsgQueue.sendConfig();
  lidarMsgQueue.sendConfig();
  pyranometerMsgQueue.sendConfig();
  oplsMsgQueue.sendConfig();
}

void Handler::heartBeat() {  // Sends a heartbeat message every seconds
  mavlink_msg_heartbeat_pack( DAUGHTER_BOARD_SYSID, 1, &heartBeatMsg, 0 ); 
  while( 1 ) {
    usleep( 1000000 );
    std::cout << "heartbeat" << std::endl;
    MAVSerial->write_message( &heartBeatMsg );
  }
}

void Handler::setTime() {
  gettimeofday( &tv, NULL );
  sec  = tv.tv_sec;
  usec = tv.tv_usec;
}
