#pragma once

enum MotorMode { STEP_MODE,HOME_MODE, CONTINUOUS_MODE, AVAILABLE };

struct MotorState{
  int mode;
  int position;

} ;

struct MotorMessage{
  int mode;
  double value1;
  double value2;
};
