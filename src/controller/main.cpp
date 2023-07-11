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
                  const MovementUnit mv_u, const std::double_t speed) {
  fmt::print("stepMode running\n");
  const std::double_t mov_per_step = (end - start) / steps;
  const MotorMessage msg{MotorMode::ABSOLUTE, mv_u, speed, start};
  fmt::print("Moving to start\n");
  const bool sent_succ = client.sendMessage(msg);
  fmt::print("Done moving to start success:{}\n",sent_succ);
  if (!sent_succ)
    throw "Comunication error";
  const auto read = client.readResponse();
  if (!read.has_value())
    throw "Comunication error";

  for (std::size_t i = 0; i < steps; i++) {
    const MotorMessage msg2{MotorMode::RELATIVE, mv_u, speed, mov_per_step};
    // fmt::print("Moving to {} mode {} pos {}\n", i, MotorMode::RELATIVE, mov_per_step);
    // fmt::print("Sending message[{},{},{},{}]\n", msg.movement_mode, msg.movement_unit, msg.speed, msg.mag);
    const bool sent_succ = client.sendMessage(msg2);
    if (!sent_succ)
      throw "Comunication error";
    const auto read = client.readResponse();
    if (!read.has_value())
      throw "Comunication error";
    sleep(1);
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
void runClose(const MotorSocketClient& client){
  const MotorMessage msg{MotorMode::END, MovementUnit::mm, 0., 0};
  std::cout << std::endl;

  const bool sent_succ = client.sendMessage(msg);
  if (!sent_succ)
    throw "Comunication error";

  const auto read = client.readResponse();
  if (!read.has_value())
    throw "Comunication error";
}

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
  // runHomeMode(socket);

  runSteepMode(socket, 1000., 20000., 20, MovementUnit::step,10000);
  // runContinuousMode(socket, 1000., 100000., MovementUnit::step,1000);

  runClose(socket);

  // TODO:  save the files to disk is necessary

  // TODO:  unlock the servers
  exit(EXIT_SUCCESS);
}
