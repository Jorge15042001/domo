#include "socketHandler.hpp"
#include "motorDriver.hpp"
#include "motorStructs.hpp"
#include <cstddef>
#include <iostream>
#include <unistd.h>

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
bool MotorSocket::writeMessage(const int client_id,
                               const MotorResponse &msg) const {
  const int ret = write(client_id, &msg, sizeof(MotorResponse));
  if (ret == -1) {
    perror("Failed to read message from server");
    exit(EXIT_FAILURE);
  }
  return ret > 0;
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
  const MotorResponse res = this->motor.goHome();
  // response to the client
  const bool succ = this->writeMessage(client.client_id, res);
  
}

void MotorSocket::processGoToMode(const MotorCient &client,
                                  const MotorMessage &msg) const {
  const MotorResponse res = this->motor.goToPosition(msg.value1);
  // response to the client
}
void MotorSocket::processStepMode(const MotorCient &client,
                                  const MotorMessage &msg) const {
  std::cout << "stting initial position" << std::endl;
  MotorResponse res = this->motor.goToPosition(msg.value1);
  const double step_size = (msg.value2 - msg.value1) / msg.steps;
  std::cout<<"step size:"<<step_size<<std::endl;
  for (std::size_t i = 0; i < msg.steps; i++) {

    res = this->motor.moveMilimiters(step_size);
    if (!res.success)
      return;
    // signal the client
    const bool write_succ = this->writeMessage(client.client_id, {true, false});
    if (!write_succ)
      break;
    // wait for the client to con
    MotorMessage m = this->readMessage(client.client_id);
    if (m.mode!=CONTINUE)break;
  }
  // repley to client
  const bool write_succ = this->writeMessage(client.client_id, {true, true});
}
void MotorSocket::processContinuous(const MotorCient &client,
                                    const MotorMessage &msg) const {
  // MotorResponse res = this->motor.moveContinuous(msg.value1, msg.value2);
  const MotorResponse res1 = this->motor.goToPosition(msg.value1);
  // signal the client
    const bool write_succ = this->writeMessage(client.client_id, {true, false});
    if (!write_succ)return;
    //wait for CONTINUE signal
    const MotorMessage m = this->readMessage(client.client_id);
    if (m.mode!=CONTINUE)return;

  const MotorResponse res2 = this->motor.goToPosition(msg.value2);
  // reply to the client
  const bool write_succ2 = this->writeMessage(client.client_id, res2);

}
