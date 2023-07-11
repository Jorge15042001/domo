#include "socketHandler.hpp"
#include <fmt/core.h>
#include <iostream>
#include <optional>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

MotorSocketClient::MotorSocketClient(const char *const SOCKET_NAME)
    : socket_name(SOCKET_NAME) {
  const int data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (data_socket == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  this->id = data_socket;
}
MotorSocketClient::~MotorSocketClient() { close(this->id); }
void MotorSocketClient::connectToSocket() {
  /*
   * For portability clear the whole structure, since some
   * implementations have additional (nonstandard) fields in
   * the structure.
   */

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(struct sockaddr_un));

  /* Connect socket to socket address */

  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, this->socket_name.c_str(), sizeof(addr.sun_path) - 1);

  std::cout << std::endl;
  int ret = connect(this->id, (const struct sockaddr *)&addr,
                    sizeof(struct sockaddr_un));
  std::cout << std::endl;
  if (ret == -1) {
    fprintf(stderr, "The server is down.\n");
    exit(EXIT_FAILURE);
  }
}

bool MotorSocketClient::sendMessage(const MotorMessage &msg) const {
  // fmt::print("Sending message to fd {} [{},{},{},{}]\n", this->id,
  //            msg.movement_mode, msg.movement_unit, msg.speed, msg.mag);
  // fmt::print("size: {}\n", sizeof(MotorMessage));
  const int ret = write(this->id, &msg, sizeof(MotorMessage));
  // fmt::print("sent message, got {}\n", ret);
  return ret > 0;
}
std::optional<MotorResponse> MotorSocketClient::readResponse() const {
  MotorResponse res;
  const int ret = read(this->id, &res, sizeof(MotorResponse));
  if (ret > 0)
    return res;
  return {};
}
