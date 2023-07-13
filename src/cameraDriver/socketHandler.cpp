#include "socketHandler.hpp"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <memory>

void CameraClient::reply(const CameraResponse& res)const{
  write(client_id, &res, sizeof(res));
}
CameraMessage CameraClient::readMessage()const{
  CameraMessagePartial msgPartial;
  const int ret1 = read(client_id, &msgPartial, sizeof(CameraMessagePartial));

  if (ret1 == -1) {
    perror("Failed to read message from server");
    exit(EXIT_FAILURE);
  }

  std::string file_path;
  file_path.reserve(msgPartial.path_length);
  // std::unique_ptr<char[]> file_path;
  // file_path.reset((char*)malloc(msgPartial.path_length));
  // char * file_path = (char*)malloc(msgPartial.path_length);
  const int ret2 = read(client_id, file_path.data(),msgPartial.path_length);
  if (ret2 == -1) {
    perror("Failed to read message from server");
    exit(EXIT_FAILURE);
  }


  return {msgPartial.mode, file_path.data()};
}
CameraSocket::CameraSocket(const char *const SOCKET_NAME, const char *camera_id)
    : socket_name(SOCKET_NAME), camera(camera_id) {
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

// CameraMessage CameraSocket::readMessage(const int client_id) const {
//
//   CameraMessage msg;
//
//   const int ret = read(client_id, &msg, sizeof(CameraMessage));
//
//   if (ret == -1) {
//     perror("Failed to read message from server");
//     exit(EXIT_FAILURE);
//   }
//   return msg;
// }
// bool CameraSocket::writeMessage(const int client_id,
//                                 const CameraResponse &msg) const {
//   const int ret = write(client_id, &msg, sizeof(CameraResponse));
//   if (ret == -1) {
//     perror("Failed to read message from server");
//     exit(EXIT_FAILURE);
//   }
//   return ret > 0;
// }
//
void CameraSocket::processClient(const CameraClient &client) const {
  bool loop = true;
  while (loop) {
    const CameraMessage msg = client.readMessage();
    switch (msg.mode) {
      break;
    case END_CONNECTION:
      loop = false;
    case SNAP_MODE: {
      const auto response = this->processSnapMode(client, msg);
      client.reply(response);
    } break;
    case START_RECORDING: {
      const auto response = this->processStartRecording(client, msg);
      client.reply(response);
    } break;
    case END_RECORDING: {
      const auto response = this->processEndRecording(client, msg);
      client.reply(response);
    } break;
    default:
      perror("Failed to process message");
      exit(EXIT_FAILURE);
    }
  }
}

CameraResponse CameraSocket::processSnapMode(const CameraClient &client,
                                             const CameraMessage &msg) const {
  return camera.takeSnapshot(msg.path);
}
CameraResponse
CameraSocket::processStartRecording(const CameraClient &client,
                                    const CameraMessage &msg) const {
  return camera.startRecording(msg.path);
}
CameraResponse
CameraSocket::processEndRecording(const CameraClient &client,
                                    const CameraMessage &msg) const {
  return camera.endRecording();
}
