#include "motorDriver.hpp"
#include "motorStructs.hpp"
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

std::vector<std::string> readLines(mn::CppLinuxSerial::SerialPort &port) {
  std::vector<std::string> lines;
  std::string buff;
  while (true) {
    std::string read;
    port.Read(read);
    buff += read;
    if (buff.ends_with('\n'))
      break;
  }
  // split the string in to lines
  while (true) {
    const std::size_t idx = buff.find('\n');
    if (idx == std::string::npos)
      break;
    const std::string line = buff.substr(0, idx);
    buff.erase(0, idx + 1);
    lines.push_back(line);
  }

  return lines;
}

MotorDriver::MotorDriver(const char *const motor_port, const int bdrate)
    : port(motor_port, bdrate) {
  // this->port.SetTimeout(0); // non-blocking mode
  this->port.Open();
  sleep(2);

  std::string readOpen;
}
bool MotorDriver::writeMessage(const char *const msg, const bool sync) {
  this->port.Write(msg);

  bool success = true;
  std::size_t required_respnonses = 1;

  while (required_respnonses !=
         0) { // keep reading until 2 responses have arrived
    for (auto &line : readLines(this->port)) { // read lines from serial port
      // success answer
      if (line.starts_with("ok")) {
        required_respnonses--;
      }
      // error
      else if (line.starts_with("error")) {
        success = false;
        required_respnonses--;
      }
    }
  }
  if (!success)
    return false;
  if (!sync)
    return success;

  bool keep_ = true;
  while (keep_) {
    this->port.Write("?\n");
    for (const auto &line : readLines(this->port)) {
      if (!line.starts_with('<'))
        continue;
      if (line.find("Idle") == std::string::npos)
        continue;
      keep_ = false;
      break;
    }

    usleep(100000);
  }

  return success;
}

void MotorDriver::initializeMotor() {

  // set default values

  std::cout << "Initiating slider" << std::endl;
  std::cout << "Unlocking" << std::endl;
  this->writeMessage("$X\n");
  std::cout << "Device unlocked" << std::endl;

  this->writeMessage("$3=0\n"); // changing direction
  this->writeMessage("$5=1\n"); // normally low stop switches

  this->writeMessage("$20=1\n"); // soft limits
  this->writeMessage("$21=1\n"); // hard limist
  this->writeMessage("$22=1\n"); // enable homing cicle

  this->writeMessage("$25=200\n"); // velocidad de homing

  this->writeMessage("$27=3\n"); // pull of distance

  this->writeMessage("$100=639.36\n"); // pasos por milimetro eje x
  this->writeMessage("$110=1000\n");   // velocidad maxima x mm/min

  this->writeMessage("$120=50\n"); // acceleration eje x

  this->writeMessage("$130=178\n"); // acceleration eje y

  // this->port.Write("G4P5.0\n");
  this->goHome();
}

MotorResponse MotorDriver::goHome() {

  std::cout << "going home" << std::endl;

  const bool success = this->writeMessage("$HX\n",true);

  return {success, true};
}

MotorResponse MotorDriver::goToPosition(const double pos) {
  std::cout << "gointg to position mode"
            << "position: " << pos << std::endl;
  // transform from normal coordinates to device coordinates(negative)
  const double movement = pos * -1;
  const bool succ = this->writeMessage(
      ("$J=G90G21X" + std::to_string(movement) + "F400\n").c_str(),true);
  return {succ, true};
}
MotorResponse MotorDriver::moveMilimiters(const double milimiters) {
  // transform from normal coordinates to device coordinates(negative)
  const double movement = milimiters * -1;
  const bool succ = this->writeMessage(
      ("$J=G91G21X" + std::to_string(movement) + "F400\n").c_str(), true);
  return {succ, true};
}
MotorResponse MotorDriver::moveContinuous(const double start,
                                          const double end) {
  //  go to the start of the movement
  MotorResponse res = this->goToPosition(start);
  // signal the motor is ready

  // move until end
  MotorResponse res2 = this->goToPosition(end);
  // signal de motor have reached final position

  // reply to server
}
