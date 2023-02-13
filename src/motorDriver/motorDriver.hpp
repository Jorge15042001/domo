#pragma  once
#include "motorStructs.hpp"

#include <cstddef>
#include <CppLinuxSerial/SerialPort.hpp>

#include <string>
#include <vector>
#include <thread>



class MotorDriver{
  // if this variable is set to true, it means "position" may not be accuarate and need to be recalibrated
  bool needs_home;
  double position;//mm
  mn::CppLinuxSerial::SerialPort port;
  std::size_t sent_msgs = 0;
  std::size_t response_msgs = 0;

public:
  MotorDriver(const char* const port, const int bdrate);
  void initializeMotor();
  [[nodiscard]] bool writeMessage(const char * const msg,const bool sync = false);
  [[nodiscard]] MotorResponse goHome();
  [[nodiscard]] MotorResponse reset();
  [[nodiscard]] MotorResponse goToPosition(const double position);
  [[nodiscard]] MotorResponse moveMilimiters(const double milimiters);
  [[nodiscard]] MotorResponse moveContinuous(const double start, const double end);


};
