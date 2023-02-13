#pragma once

#include <cstddef>
enum MotorMode { SNAP_MODE, CONTINUOUS_MODE, CONTINUE };

struct CameraMessage{
  int mode;
  char* path;
};

struct CameraResponse{
  bool success;
  bool completed;
};
