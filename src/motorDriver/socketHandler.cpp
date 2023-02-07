#include "socketHandler.hpp"
#include "motorDriver.hpp"
#include "motorStructs.hpp"
#include <cstddef>
#include <iostream>
#include <unistd.h>

MotorSocket::MotorSocket(const char *const SOCKET_NAME, const char *const motor_port,
                         const int bdrate)
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
  int message_type;
  int movement;
  std::size_t val1;
  std::size_t val2;

  const int ret1 = read(client_id, &message_type, sizeof(int));
  const int ret2 = read(client_id, &movement, sizeof(int));
  const int ret3 = read(client_id, &val1, sizeof(std::size_t));
  const int ret4 = read(client_id, &val2, sizeof(std::size_t));

  if (ret1 == -1 || ret2 == -1 || ret3 != -1 || ret4 != -1) {
    perror("Failed to read message from server");
    exit(EXIT_FAILURE);
  }
  return {message_type, movement, val1, val2};
}

void MotorSocket::processClient(const MotorCient &client) const {
  const MotorMessage msg = readMessage(client.client_id);
  switch (msg.mode) {
  case HOME_MODE:
    this->processHomeMode(client, msg);
    break;
  case STEP_MODE:
    this->processStepMode(client, msg);
    break;
  case CONTINUOUS_MODE:
    this->processContinuous(client, msg);
    break;
  default:
    perror("Failed to process message");
    exit(EXIT_FAILURE);
  }
}

void MotorSocket::processHomeMode(const MotorCient &client,
                                  const MotorMessage &msg) const {
  // make the motor go home
  MotorResponse res = this->motor.goHome();
  // response to the client
}

void MotorSocket::processGoToMode(const MotorCient &client,
                                  const MotorMessage &msg) const {
  MotorResponse res = this->motor.goToPosition(msg.value1);
}
void MotorSocket::processStepMode(const MotorCient &client,
                                  const MotorMessage &msg) const {
  for (std::size_t i = 0; i < msg.value1; i++) {
    MotorResponse res = this->motor.moveSteps(1, msg.direction);
    // if failed throw
    // signal client of moved step
  }
  // reply to client
}
void MotorSocket::processContinuous(const MotorCient &client,
                                    const MotorMessage &msg) const {
  MotorResponse res = this->motor.moveContinuous(msg.value1, msg.value2);
  // reply to the client
}
