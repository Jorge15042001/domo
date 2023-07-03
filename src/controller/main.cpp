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
void runSteepMode(const MotorSocketClient &client, const std::double_t start,
                  const std::double_t end, const std::size_t steps,
                  const MovementUnit mv_u) {
  const std::double_t mov_per_step = (end - start) / steps;
  const MotorMessage msg{MotorMode::ABSOLUTE, mv_u, 10, start};
  const bool sent_succ = client.sendMessage(msg);
  if (!sent_succ)
    throw "Comunication error";
  const auto read = client.readResponse();
  if (!read.has_value())
    throw "Comunication error";

  for (std::size_t i = 0; i < steps; i++) {
    const MotorMessage msg2{MotorMode::RELATIVE, mv_u, 10, mov_per_step};
    const bool sent_succ = client.sendMessage(msg);
    if (!sent_succ)
      throw "Comunication error";
    const auto read = client.readResponse();
    if (!read.has_value())
      throw "Comunication error";
  }
}

// void runContinuousMode(const MotorSocketClient &client) {
//   const MotorMessage msg{CONTINUOUS_MODE, 3, 170};
//   const bool sent_succ = client.sendMessage(msg);
//   if (!sent_succ)
//     exit(EXIT_FAILURE);
//
//
//   const auto res = client.readResponse();
//   if (!res.has_value())
//     exit(EXIT_FAILURE);
//   if (!res->success)
//     exit(EXIT_FAILURE);
//
//   const bool write_succ = client.sendMessage({CONTINUE});
//   if (!write_succ)
//     exit(EXIT_FAILURE);
//
//   const auto res2 = client.readResponse();
//   if (!res.has_value())
//     exit(EXIT_FAILURE);
//
//   if (!res2->success)
//     exit(EXIT_FAILURE);
//
//   return;
// }
//
int main(const int argc, const char *const *const argv) {
  // TODO: read desired mode

  /* Create local socket. */
  MotorSocketClient socket{MOTOR_SERVER_SOCKET};
  // TODO:  start camera server if not running
  // TODO:  exit if failed to connect to the camera server
  // TODO:  exit if server is busy

  // TODO: start motor server if not running
  // TODO:  exit if failed to connect to the motor server
  // stablish connection with motor server
  socket.connectToSocket();

  // TODO:  runSelectedMode
  runHomeMode(socket);
  runSteepMode(socket, 0., 100., 10,MovementUnit::mm );

  // TODO:  save the files to disk is necessary

  // TODO:  unlock the servers
  exit(EXIT_SUCCESS);
}
