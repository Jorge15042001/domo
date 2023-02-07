#pragma  once
#include <cstddef>
#include <CppLinuxSerial/SerialPort.hpp>

struct MotorResponse{
  bool success;

};

class MotorDriver{
  // if this variable is set to true, it means "position" may not be accuarate and need to be recalibrated
  bool needs_home;
  double position;//mm
  mn::CppLinuxSerial::SerialPort port;

public:
  MotorDriver(const char* const port, const int bdrate);
  void initializeMotor();
  MotorResponse goHome();
  MotorResponse reset();
  MotorResponse goToPosition(const double position);
  MotorResponse moveSteps(const double stpes,const int direction);
  MotorResponse moveContinuous(const double start, const double end);


};
