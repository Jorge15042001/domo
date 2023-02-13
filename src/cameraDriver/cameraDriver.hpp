#pragma  once
#include "cameraStructs.hpp"

#include <cstddef>
#include <CppLinuxSerial/SerialPort.hpp>

#include <string>
#include <vector>
#include <thread>



class CameraDriver{

public:
  CameraDriver(const char* const port, const int bdrate);
  void initializeCamera();
  [[nodiscard]] CameraResponse takeSnapshot(const char* const path);
  // [[nodiscard]] CameraResponse record(const double start, const double end);


};
