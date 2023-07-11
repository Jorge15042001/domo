#include "motorDriver.hpp"
#include "motorStructs.hpp"
#include <CppLinuxSerial/SerialPort.hpp>
#include <algorithm>
#include <cstddef>
#include <fmt/core.h>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

SerialCommunication::SerialCommunication(const char *const motor_port,
                                         const int bdrate)
    : port(motor_port, bdrate) {
  this->port.Open();
}
std::string SerialCommunication::sendMessage(const std::string &msg,
                                             const bool locking) {
  std::string response;
  this->port.Write(msg);

  if (!locking)
    return response;

  while (response.find('\n') == std::string::npos) {
    this->port.Read(response);
  }
  return response;
}

MotorDriver::MotorDriver(const char *const motor_port, const int bdrate)
    : serian_channel(motor_port, bdrate) {}

void MotorDriver::initializeMotor() {
  std::cout << "initializing motor" << std::endl;
  // goHome();
}

MotorResponse MotorDriver::goHome() {
  const MotorMessage msg{MotorMode::HOME_MODE, MovementUnit::mm, 0, 0};
  const std::string msg_serialized = msg.serailize();
  const std::string response_str =
      this->serian_channel.sendMessage(msg_serialized, true);
  const MotorResponse response(response_str);
  return response;
}

MotorResponse MotorDriver::relativeMove(const std::double_t speed,
                                        const std::double_t movement,
                                        const MovementUnit unit) {
  const MotorMessage msg{MotorMode::RELATIVE, unit, speed, movement};
  const std::string msg_serialized = msg.serailize();
  fmt::print("{}\n",msg_serialized);
  const std::string response_str =
      this->serian_channel.sendMessage(msg_serialized, true);
  const MotorResponse response(response_str);
  return response;
}

MotorResponse MotorDriver::absoluteMove(const std::double_t speed,
                                        const std::double_t movement,
                                        const MovementUnit unit) {

  const MotorMessage msg{MotorMode::ABSOLUTE, unit, speed, movement};
  const std::string msg_serialized = msg.serailize();
  const std::string response_str =
      this->serian_channel.sendMessage(msg_serialized, true);
  const MotorResponse response(response_str);
  return response;
}
// MotorResponse MotorDriver::goToPosition(const double pos) {
// }
// MotorResponse MotorDriver::moveMilimiters(const double milimiters) {}
// MotorResponse MotorDriver::moveContinuous(const double start,
//                                           const double end) {}
