
#include "../motorDriver/motorStructs.hpp"
#include "socketHandler.hpp"

#include <cstdlib>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <iostream>

const char *const MOTOR_SERVER_SOCKET = "/tmp/resol.sock";

void runHomeMode(const MotorSocketClient &client) {
  const MotorMessage msg{HOME_MODE, 0., 0., 0};

  const bool sent_succ = client.sendMessage(msg);
  if (!sent_succ)
    exit(EXIT_FAILURE);

  const auto read = client.readResponse();
  if (!read.has_value())
    exit(EXIT_FAILURE);
}
void runSteepMode(const MotorSocketClient &client) {
  const MotorMessage msg{STEP_MODE, 3, 170, 10};
  const bool sent_succ = client.sendMessage(msg);
  if (!sent_succ)
    exit(EXIT_FAILURE);

  while (true) {
    std::cout<<"iteration"<<std::endl;

    const auto res = client.readResponse();
    if (!res.has_value())
      exit(EXIT_FAILURE);

    if(res->completed)break;

    const bool write_succ = client.sendMessage({CONTINUE});
    if (!write_succ)
      exit(EXIT_FAILURE);
  }

  return;
}

void runContinuousMode(const MotorSocketClient &client) {
  const MotorMessage msg{CONTINUOUS_MODE, 3, 170};
  const bool sent_succ = client.sendMessage(msg);
  if (!sent_succ)
    exit(EXIT_FAILURE);


  const auto res = client.readResponse();
  if (!res.has_value())
    exit(EXIT_FAILURE);
  if (!res->success)
    exit(EXIT_FAILURE);

  const bool write_succ = client.sendMessage({CONTINUE});
  if (!write_succ)
    exit(EXIT_FAILURE);

  const auto res2 = client.readResponse();
  if (!res.has_value())
    exit(EXIT_FAILURE);

  if (!res2->success)
    exit(EXIT_FAILURE);
  
  return;
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
  runContinuousMode(socket);
  // runSteepMode(motorServerClient,0);

  // TODO:  save the files to disk is necessary

  // TODO:  unlock the servers
  exit(EXIT_SUCCESS);
}
