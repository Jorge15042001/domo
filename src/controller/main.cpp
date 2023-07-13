#include "socketHandler.hpp"

#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <errno.h>
#include <fmt/core.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

const char *const MOTOR_SERVER_SOCKET = "/tmp/resol.sock";
const char *const CAMERA_SERVER_SOCKET = "/tmp/camera.sock";
const char *const SAVE_TO_PATH = "/tmp/videos";

void runHomeMode(const MotorSocketClient &client) {
  const MotorMessage msg{MotorMode::HOME_MODE, MovementUnit::mm, 0., 0};
  std::cout << std::endl;

  const bool sent_succ = client.sendMessage(msg);
  if (!sent_succ)
    throw "Comunication error";

  const auto read = client.readResponse();
  if (!read.has_value())
    throw "Comunication error";
}
void runSteepMode(const MotorSocketClient &motor_client,
                  const CameraSocketClient &camera_client,
                  const std::double_t start, const std::double_t end,
                  const std::size_t steps, const MovementUnit mv_u,
                  const std::double_t speed, const std::filesystem::path& path) {
  fmt::print("stepMode running\n");
  const std::double_t mov_per_step = (end - start) / steps;

  const MotorMessage msg_motor_1{MotorMode::ABSOLUTE, mv_u, speed, start};
  const CameraMessage msg_cam_1{CameraMode::SNAP_MODE, path};

  const bool sent_succ_motor1 = motor_client.sendMessage(msg_motor_1);
  if (!sent_succ_motor1)
    throw "Comunication error";
  const auto read_motor_1 = motor_client.readResponse();
  if (!read_motor_1.has_value())
    throw "Comunication error";

  const bool sent_succ_cam1 = camera_client.sendMessage(msg_cam_1);
  if (!sent_succ_cam1)
    throw "Comunication error";
  const auto read_cam_1 = camera_client.readResponse();
  if (!read_cam_1.has_value())
    throw "Comunication error";

  for (std::size_t i = 0; i < steps; i++) {
    const MotorMessage msg_motor_2{MotorMode::RELATIVE, mv_u, speed, mov_per_step};
    // fmt::print("Moving to {} mode {} pos {}\n", i, MotorMode::RELATIVE,
    // mov_per_step); fmt::print("Sending message[{},{},{},{}]\n",
    // msg.movement_mode, msg.movement_unit, msg.speed, msg.mag);
    const bool sent_succ_motor2 = motor_client.sendMessage(msg_motor_2);
    if (!sent_succ_motor2)
      throw "Comunication error";
    const auto read_motor_2 = motor_client.readResponse();
    if (!read_motor_2.has_value())
      throw "Comunication error";

    const bool sent_succ_cam2 = camera_client.sendMessage(msg_cam_1);
    if (!sent_succ_cam2)
      throw "Comunication error";
    const auto read_cam_2 = camera_client.readResponse();
    if (!read_cam_2.has_value())
      throw "Comunication error";

    // sleep(1);
  }
  fmt::print("..Done\n");
}

// void runContinuousMode(const MotorSocketClient &client) {
void runContinuousMode(const MotorSocketClient &client,
                       const std::double_t start, const std::double_t end,
                       const MovementUnit mv_u, const std::double_t speed) {
  const MotorMessage msg{MotorMode::ABSOLUTE, mv_u, speed, start};
  const bool sent_succ = client.sendMessage(msg);
  if (!sent_succ)
    throw "Comunication error";

  const auto read = client.readResponse();
  if (!read.has_value())
    throw "Comunication error";

  const MotorMessage msg2{MotorMode::ABSOLUTE, mv_u, speed, end};
  const bool sent_succ2 = client.sendMessage(msg2);
  if (!sent_succ2)
    throw "Comunication error";

  const auto read2 = client.readResponse();
  if (!read2.has_value())
    throw "Comunication error";
}
void runClose(const MotorSocketClient &motor_client, const CameraSocketClient& cam_client) {
  const MotorMessage msg_end_motor{MotorMode::END, MovementUnit::mm, 0., 0};
  const CameraMessage msg_end_cam{CameraMode::END_CONNECTION,""};

  std::cout << std::endl;

  const bool sent_motor_succ = motor_client.sendMessage(msg_end_motor);
  if (!sent_motor_succ)
    throw "Comunication error";

  const bool sent_cam_succ = cam_client.sendMessage(msg_end_cam);
  if (!sent_cam_succ)
    throw "Comunication error";

  const auto read_motor_response = motor_client.readResponse();
  if (!read_motor_response.has_value())
    throw "Comunication error";

  const auto read_cam_response= cam_client.readResponse();
  if (!read_cam_response.has_value())
    throw "Comunication error";
}

int main(const int argc, const char *const *const argv) {
  // TODO: read desired mode

  /* Create local socket. */
  MotorSocketClient motor_client{MOTOR_SERVER_SOCKET};
  CameraSocketClient camera_client{CAMERA_SERVER_SOCKET};
  // TODO:  start camera server if not running
  // TODO:  exit if failed to connect to the camera server
  // TODO:  exit if server is busy

  // TODO: start motor server if not running
  // TODO:  exit if failed to connect to the motor server
  // stablish connection with motor server
  motor_client.connectToSocket();
  camera_client.connectToSocket();

  // TODO:  runSelectedMode
  // runHomeMode(socket);

  runSteepMode(motor_client, camera_client, 20000., 100000., 20, MovementUnit::step, 10000, "/tmp/images");
  // runContinuousMode(socket, 1000., 100000., MovementUnit::step,1000);

  runClose(motor_client, camera_client);

  // TODO:  save the files to disk is necessary

  // TODO:  unlock the servers
  exit(EXIT_SUCCESS);
}
