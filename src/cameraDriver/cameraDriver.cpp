#include "cameraDriver.hpp"
#include <CppLinuxSerial/SerialPort.hpp>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>


CameraDriver::CameraDriver(const char *const motor_port, const int bdrate) {

}

void CameraDriver::initializeCamera() {

  // set default values

}


CameraResponse CameraDriver::takeSnapshot(const char *const  path) {
  std::cout<<"Taking snapshot"<<std::endl;
  std::cout<<"snapshot saved in "<<path<<std::endl;
  return {true, true};
}

// CameraResponse CameraDriver::record(const double start,
//                                           const double end) {
//   //  go to the start of the movement
//   CameraResponse res = this->takeSnapshot(start);
//   // signal the motor is ready
//
//   // move until end
//   CameraResponse res2 = this->takeSnapshot(end);
//   // signal de motor have reached final position
//
//   // reply to server
// }
