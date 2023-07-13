#pragma  once
#include "cameraStructs.hpp"

#include <cstddef>
#include <CppLinuxSerial/SerialPort.hpp>

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <filesystem>


#include "tcamcamera.h"


enum OperationMode{
  NONE,SNAPSHOT,VIDEO
};
struct CUSTOMDATA{
  bool SaveNextImage;
  std::size_t frame_counter;
  OperationMode mode;
  gsttcam::TcamCamera* camera;
  std::condition_variable con;
  std::mutex mut;
  std::filesystem::path save_path;
};
class CameraDriver{

  gsttcam::TcamCamera m_cam;
  CUSTOMDATA cam_control_data;

public:
  CameraDriver(const char * cameraid);
  void initializeCamera();
  [[nodiscard]] CameraResponse takeSnapshot(const std::filesystem::path& path);
  [[nodiscard]] CameraResponse startRecording(const std::filesystem::path& path);
  [[nodiscard]] CameraResponse endRecording();


};
