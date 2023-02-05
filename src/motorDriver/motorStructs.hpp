#pragma once

#include <cstddef>
enum MotorMode { STEP_MODE,HOME_MODE, CONTINUOUS_MODE, AVAILABLE };
enum MotorMovent {CLOCK_WISE, COUNTER_CLOCK_wISE};

struct MotorState{
  int mode;
  int position;

} ;

struct MotorMessage{
  int mode;
  int direction;
  std::size_t value1;
  std::size_t value2;
};
