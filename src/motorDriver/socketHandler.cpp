#include "socketHandler.hpp"
#include "motorDriver.hpp"
#include "motorStructs.hpp"
#include <cstddef>
#include <fmt/core.h>
#include <iostream>
#include <unistd.h>

bool MotorCient::reply(const MotorResponse &msg) const {
  fmt::print("replying...\n");
  const int ret = write(this->client_id, &msg, sizeof(msg));
  return ret != -1 && ret == sizeof(msg);
  fmt::print("replyed...\n");
}

MotorSocket::MotorSocket(const char *const SOCKET_NAME,
                         const char *const motor_port, const int bdrate)
    : socket_name(SOCKET_NAME), motor(motor_port, bdrate) {
  // std::cout<<"creating socket"<<std::endl;
  this->socket_id = this->createSocket();
  this->bindSocket();

  // go to gome
  // this->motor.goHome();
}
MotorSocket::~MotorSocket() {
  close(this->socket_id);
  unlink(this->socket_name.c_str());
}

int MotorSocket::createSocket() const {
  /*
   * In case the program exited inadvertently on the last run,
   * remove the socket.
   */

  unlink(this->socket_name.c_str());

  /* Create local socket. */

  const int listen_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (listen_socket == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  return listen_socket;
}
void MotorSocket::bindSocket() const {

  struct sockaddr_un addr;
  /*
   * For portability clear the whole structure, since some
   * implementations have additional (nonstandard) fields in
   * the structure.
   */

  memset(&addr, 0, sizeof(struct sockaddr_un));

  /* Bind socket to socket name. */

  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, this->socket_name.c_str(), sizeof(addr.sun_path) - 1);

  int ret = bind(this->socket_id, (const struct sockaddr *)&addr,
                 sizeof(struct sockaddr_un));
  if (ret == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
}
void MotorSocket::startListening() const {
  int ret = listen(this->socket_id, 20);
  if (ret == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
}
int MotorSocket::accpetClient() const {

  const int data_socket = accept(this->socket_id, NULL, NULL);
  if (data_socket == -1) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  return data_socket;
}

MotorMessage MotorSocket::readMessage(const int client_id) const {

  MotorMessage msg;

  const int ret = read(client_id, &msg, sizeof(MotorMessage));

  if (ret == -1) {
    perror("Failed to read message from server");
    exit(EXIT_FAILURE);
  }
  return msg;
}

void MotorSocket::processClient(const MotorCient &client) const {
  bool loop = true;
  while (loop) {
    const MotorMessage msg = readMessage(client.client_id);
    switch (msg.movement_mode) {
    case END: {
      loop = false;
      client.reply({});
    } break;
    case HOME_MODE: {
      auto response = this->motor.goHome();
      client.reply(response);
    } break;
    case RELATIVE: {
      auto response =
          this->motor.relativeMove(msg.speed, msg.mag, msg.movement_unit);
      client.reply(response);
    } break;
    case ABSOLUTE: {
      auto response =
          this->motor.absoluteMove(msg.speed, msg.mag, msg.movement_unit);
      client.reply(response);
    } break;
    default:
      perror("Failed to process message");
      client.reply({false, 0, 0, 0});
    }
  }
}
