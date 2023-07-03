#pragma once
#include "motorStructs.hpp"

#include <CppLinuxSerial/SerialPort.hpp>
#include <cstddef>

#include <string>
#include <thread>
#include <unistd.h>
#include <vector>
#include <optional>

class SerialCommunication {
  mn::CppLinuxSerial::SerialPort port;
public:
  SerialCommunication(const char *const motor_port, const int bdrate);
  // ~SerialCommunication();
  SerialCommunication(SerialCommunication&)=delete;
  SerialCommunication(SerialCommunication&&)=delete;
  std::string sendMessage (const std::string& msg, const bool locking);

};

class MotorDriver {
  // if this variable is set to true, it means "position" may not be accuarate
  // and need to be recalibrated
  SerialCommunication serian_channel;

public:
  MotorDriver(const char *const port, const int bdrate);
  void initializeMotor();
  // TODO: readMessage???
  [[nodiscard]] bool writeMessage(const char *const msg);
  [[nodiscard]] MotorResponse goHome();
  [[nodiscard]] MotorResponse reset();
  [[nodiscard]] MotorResponse goToPosition(const double position);
  [[nodiscard]] MotorResponse moveMilimiters(const double milimiters);
  [[nodiscard]] MotorResponse moveContinuous(const double start,
                                             const double end);
};

