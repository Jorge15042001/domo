#include "motorDriver.hpp"
#include "motorStructs.hpp"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <unistd.h>


MotorDriver::MotorDriver(const char *const motor_port, const int bdrate)
    : port(motor_port, bdrate) {
  // this->port.SetTimeout(0); // non-blocking mode
  this->port.Open();

  std::string readOpen;
  usleep(2000000);
  this->port.Read(readOpen);

  std::cout << "Read from motor" << readOpen << std::endl;
}


void MotorDriver::initializeMotor() {
  std::string readResponse;
  readResponse.reserve(10);


  // set default values


  std::cout << "Initiating slider" << std::endl;
  this->port.Write("$X\n");
  usleep(2000000);
  std::cout << "Unlocking" << std::endl;
  this->port.Read(readResponse);
  std::cout << "Received response: " << readResponse<<std::endl;
  std::cout << "Device unlocked" << std::endl;
  readResponse ="";

  this->port.Write("$3=1\n");// changing direction
  this->port.Write("$5=1\n");// normally low stop switches

  this->port.Write("$20=0\n");// soft limits
  this->port.Write("$21=1\n");// hard limist
  this->port.Write("$22=1\n");// enable homing cicle

  this->port.Write("$25=500\n");//velocidad de homing
                                 //
  this->port.Write("$27=3\n");//pull of distance
                              //
  this->port.Write("$100=500\n");// pasos por milimetro eje x
  this->port.Write("$110=1500\n");// velocidad maxima x mm/min
                                 //
  this->port.Write("$120=50\n");// acceleration eje x
  this->port.Write("$122=50\n");// acceleration eje z
  this->port.Write("$121=50\n");// acceleration eje y

  this->port.Write("G04 P5.0\n");

  usleep(1000000);
  this->port.Read(readResponse);
  std::cout << "Received response: " << readResponse<<std::endl;
  readResponse ="";

  this->goHome();

}

MotorResponse MotorDriver::goHome() {
  
  std::string readResponse;

  std::cout<<"going home"<<std::endl;

  this->port.Write("$HX\n");// acceleration eje y
  this->port.Write("G04 P5.0\n");
  this->port.Write("?");
  this->port.Read(readResponse);
  std::cout << "Received response: " << readResponse<<std::endl;

  return {true};
}

MotorResponse MotorDriver::goToPosition(const double pos) {
  const long steps = pos - this->position;
  std::string readResponse;

  if (steps == 0)
    return MotorResponse{true};

  this->port.Write("$J=X"+std::to_string(pos)+"F400\n");
  sleep(50);
  
  this->port.Read(readResponse);
  std::cout << "Received response: " << readResponse<<std::endl;


}
MotorResponse MotorDriver::moveSteps(const double stpes,
                                     const int direction) {
  // move steps in the specified direction

  // reply to server
}
MotorResponse MotorDriver::moveContinuous(const double start,
                                          const double end) {
  //  go to the start of the movement
  MotorResponse res = this->goToPosition(start);

  // move until end
  MotorResponse res2 = this->goToPosition(end);

  // reply to server
}
