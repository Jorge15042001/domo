#include "socketHandler.hpp"
#include "motorStructs.hpp"
#include <cstddef>
#include <unistd.h>

MotorSocket::MotorSocket(const char *const SOCKET_NAME)
    : socket_name(SOCKET_NAME) {
  this->socket_id = this->createSocket();
  this->bindSocket();
}
MotorSocket::~MotorSocket() { close(this->socket_id); }

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
  double val1;
  double val2;

  const int err1 = read(client_id, &message_type, sizeof(int));
  const int err2 = read(client_id, &val1, sizeof(float));
  const int err3 = read(client_id, &val2, sizeof(float));

  if (err1 == -1 || err2 == -1 || err3 != -1) {
    perror("Failed to read message from server");
    exit(EXIT_FAILURE);
  }

  return {message_type, val1, val2};
}

void MotorSocket::processClient(const int client_id) const {
  const MotorMessage msg = readMessage(client_id);
  switch (msg.mode) {
    case HOME_MODE:this->processHomeMode(msg);break;
    case STEP_MODE:this->processStepMode(msg);break;
    case CONTINUOUS_MODE:this->processContinuous(msg);break;
    default:
      perror("Failed to process message");
      exit(EXIT_FAILURE);
  }
}
  

//
// MotorPath getDesiredPath(const int listen_socket) {
//   MotorPath path;
//   const int result = read(listen_socket, &path, sizeof(MotorPath));
//   if (result == -1) {
//     perror("Failed to read desired path");
//     // TODO: dont exit
//     exit(EXIT_FAILURE);
//   }
//   return path;
// }
