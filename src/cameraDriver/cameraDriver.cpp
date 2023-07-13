#include "cameraDriver.hpp"
#include "opencv2/opencv.hpp"
#include <CppLinuxSerial/SerialPort.hpp>
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstring>
#include <filesystem>
#include <fmt/core.h>
#include <iostream>
#include <mutex>
#include <opencv2/core/hal/interface.h>
#include <opencv2/imgcodecs.hpp>
#include <ostream>
#include <sstream>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

class GstFrameInfo {
  GstFrameInfo(const GstFrameInfo &) = delete;
  GstFrameInfo(GstFrameInfo &&) = delete;
  GstFrameInfo &operator=(GstFrameInfo &) = delete;
  GstFrameInfo &operator=(GstFrameInfo &&) = delete;

public:
  GstSample *sample;
  GstBuffer *buffer;
  GstMapInfo info;
  int width;
  int height;
  std::size_t color_depth;
  std::string capture_format;

  GstFrameInfo(GstAppSink *appsink)
      : sample(gst_app_sink_pull_sample(appsink)),
        buffer(gst_sample_get_buffer(sample)) {
    gst_buffer_map(buffer, &info, GST_MAP_READ);
    const GstStructure *str;
    GstCaps *caps = gst_sample_get_caps(sample);
    str = gst_caps_get_structure(caps, 0);
    capture_format = gst_structure_get_string(str, "format");
    color_depth = [this] {
      if (capture_format == "BGRx")
        return 4;
      if (capture_format == "GRAY8")
        return 1;
      throw "unknown capture format";
    }();
    gst_structure_get_int(str, "width", &width);
    gst_structure_get_int(str, "height", &height);
  }
  ~GstFrameInfo() {
    gst_buffer_unmap(buffer, &info);
    gst_sample_unref(sample);
  }
};

static std::string nowstr() {
  constexpr static int64_t ONEMICROSECOND = 1000000;
  auto now = std::chrono::system_clock::now();
  auto onems = std::chrono::microseconds(1);
  int64_t epochus = now.time_since_epoch() / onems;
  time_t epoch = epochus / ONEMICROSECOND;
  struct tm tms {};
  localtime_r(&epoch, &tms);
  char buf[128];
  size_t nb = strftime(buf, sizeof(buf), "%Y-%m-%d__%H:%M:%S", &tms);
  nb += ::sprintf(&buf[nb], ".%06d", int(epochus % ONEMICROSECOND));
  return std::string(buf, nb);
}

////////////////////////////////////////////////////////////////////
// Callback called for new images by the internal appsink

GstFlowReturn new_frame_cb(GstAppSink *appsink, gpointer data) {

  fmt::print("running callback\n");
  // static int frame_count = 0;
  CUSTOMDATA &custom_data = *(CUSTOMDATA *)data;

  GstFrameInfo frame_info(appsink);
  // std::cout << "frame " << frame_count << '\n';
  // frame_count++;

  if (frame_info.info.data == NULL) {
    return GST_FLOW_OK;
  }

  cv::Mat img;
  img.create(frame_info.height, frame_info.width,
             CV_8UC(frame_info.color_depth));

  memcpy(img.data, frame_info.info.data,
         frame_info.height * frame_info.width * frame_info.color_depth);

  custom_data.SaveNextImage = false;

  std::filesystem::path fullpath = custom_data.save_path;
  fullpath /= nowstr() + ".png";
  fmt::print("filepath: {}\n", fullpath.string());
  cv::imwrite(fullpath, img);

  // pCustomData->frame_writer.writeFrame(frame_info.info.data,
  // frame_info.width * frame_info.height * frame_info.color_depth);
  // assert(sizeof(frame_t)==frame_info.width*frame_info.height);
  // cb->pushBlob(frame_info.info.data);
  if (custom_data.frame_counter == 0) {
    custom_data.con.notify_all();
  }
  custom_data.frame_counter++;

  return GST_FLOW_OK;
}

CameraDriver::CameraDriver(const char *const cameraid) : m_cam(cameraid) {

  m_cam.set_capture_format("GRAY8", {1920, 1080}, {15, 1});
  fmt::print("capture format set\n");
  cam_control_data.camera = &m_cam;
  cam_control_data.mode = NONE;
  m_cam.set_new_frame_callback(new_frame_cb, &cam_control_data);
}
void CameraDriver::initializeCamera() {

  // set default values
}

CameraResponse CameraDriver::takeSnapshot(const std::filesystem::path &path) {
  cam_control_data.frame_counter=0;
  cam_control_data.mode = SNAPSHOT;
  cam_control_data.SaveNextImage = true;
  cam_control_data.save_path = path;
  m_cam.start();
  std::unique_lock<std::mutex> lck(cam_control_data.mut);
  cam_control_data.con.wait(lck);
  m_cam.stop();
  cam_control_data.mode = NONE;
  return {true};
}

CameraResponse CameraDriver::startRecording(const std::filesystem::path &path) {
  cam_control_data.frame_counter=0;
  cam_control_data.mode = VIDEO;
  cam_control_data.SaveNextImage = true;
  cam_control_data.save_path = path;
  m_cam.start();
  std::unique_lock<std::mutex> lck(cam_control_data.mut);
  cam_control_data.con.wait(lck);
  return {true};
}
CameraResponse CameraDriver::endRecording() {
  m_cam.stop();
  cam_control_data.mode = NONE;
  return {true};
}
