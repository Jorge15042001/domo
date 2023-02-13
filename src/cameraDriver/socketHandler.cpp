#include "socketHandler.hpp"
#include <cstddef>
#include <iostream>
#include <unistd.h>

CameraSocket::CameraSocket(const char *const SOCKET_NAME,
                         const char *const motor_port, const int bdrate)
    : socket_name(SOCKET_NAME), camera(motor_port, bdrate) {
  // std::cout<<"creating socket"<<std::endl;
  this->socket_id = this->createSocket();
  this->bindSocket();

  // go to gome
  // this->motor.goHome();
}
CameraSocket::~CameraSocket() {
  close(this->socket_id);
  unlink(this->socket_name.c_str());
}

int CameraSocket::createSocket() const {
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
void CameraSocket::bindSocket() const {

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
void CameraSocket::startListening() const {
  int ret = listen(this->socket_id, 20);
  if (ret == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
}
int CameraSocket::accpetClient() const {

  const int data_socket = accept(this->socket_id, NULL, NULL);
  if (data_socket == -1) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  return data_socket;
}

CameraMessage CameraSocket::readMessage(const int client_id) const {

  CameraMessage msg;

  const int ret = read(client_id, &msg, sizeof(CameraMessage));

  if (ret == -1) {
    perror("Failed to read message from server");
    exit(EXIT_FAILURE);
  }
  return msg;
}
bool CameraSocket::writeMessage(const int client_id,
                               const CameraResponse &msg) const {
  const int ret = write(client_id, &msg, sizeof(CameraResponse));
  if (ret == -1) {
    perror("Failed to read message from server");
    exit(EXIT_FAILURE);
  }
  return ret > 0;
}

void CameraSocket::processClient(const CameraClient &client) const {
  const CameraMessage msg = readMessage(client.client_id);
  switch (msg.mode) {
    break;
  case SNAP_MODE:
    this->processSnapMode(client, msg);
    break;
  case CONTINUOUS_MODE:
    this->processContinuousMode(client, msg);
    break;
  default:
    perror("Failed to process message");
    exit(EXIT_FAILURE);
  }
}


void CameraSocket::processSnapMode(const CameraClient &client,
                                  const CameraMessage &msg) const {


  while (true){
  CameraResponse res = this->camera.takeSnapshot(msg.path);
  }

  // for (std::size_t i = 0; i < msg.steps; i++) {
  //
  //   res = this->camera.moveMilimiters(step_size);
  //   if (!res.success)
  //     return;
  //   // signal the client
  //   const bool write_succ = this->writeMessage(client.client_id, {true, false});
  //   if (!write_succ)
  //     break;
  //   // wait for the client to con
  //   CameraMessage m = this->readMessage(client.client_id);
  //   if (m.mode!=CONTINUE)break;
  // }
  //
  // repley to client
  const bool write_succ = this->writeMessage(client.client_id, {true, true});
}
void CameraSocket::processContinuousMode(const CameraClient &client,
                                    const CameraMessage &msg) const {
  // CameraResponse res = this->motor.moveContinuous(msg.value1, msg.value2);
  // const CameraResponse res1 = this->camera.goToPosition(msg.value1);
  // // signal the client
  //   const bool write_succ = this->writeMessage(client.client_id, {true, false});
  //   if (!write_succ)return;
  //   //wait for CONTINUE signal
  //   const CameraMessage m = this->readMessage(client.client_id);
  //   if (m.mode!=CONTINUE)return;
  //
  // const CameraResponse res2 = this->camera.goToPosition(msg.value2);
  // // reply to the client
  // const bool write_succ2 = this->writeMessage(client.client_id, res2);
  //
}
