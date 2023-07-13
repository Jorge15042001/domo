#pragma once

#include <cstddef>
#include <filesystem>
enum CameraMode { SNAP_MODE, START_RECORDING, END_RECORDING, END_CONNECTION};
struct CameraMessagePartial{
  CameraMode mode;
  std::size_t path_length;
};

struct CameraMessage{
  CameraMode mode;
  std::filesystem::path path;
};

struct CameraResponse{
  bool success;
};
