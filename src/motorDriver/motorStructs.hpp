#pragma once

#include <cstddef>
enum MotorMode { STEP_MODE,HOME_MODE, CONTINUOUS_MODE, CONTINUE };
enum MotorMovent {CLOCK_WISE, COUNTER_CLOCK_wISE};

struct MotorState{
  int mode;
  int position;

} ;

struct MotorMessage{
  int mode;
  double value1;
  double value2;
  std::size_t steps;
};

struct MotorResponse{
  bool success;
  bool completed;
};
