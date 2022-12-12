#pragma once

#include <stddef.h>
enum MotorMode { STEP_MODE, CONTINUOUS_MODE, AVAILABLE };

typedef struct {
  int mode;
  int position;

} MotorState;

typedef struct{
  size_t initalPosition;
  size_t endPositon;
  // int resolution;
  

} MotorPath;
