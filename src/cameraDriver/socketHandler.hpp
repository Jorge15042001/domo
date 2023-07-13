#pragma once

#include "cameraDriver.hpp"
#include "cameraStructs.hpp"

#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

class CameraClient {

public:
  int client_id;
  CameraClient(const int client_id) : client_id(client_id) {}
  ~CameraClient() { close(this->client_id); }

  CameraClient(const CameraClient &other) = delete;
  CameraClient(CameraClient &&other) = delete;
  CameraClient &operator=(const CameraClient &other) = delete;
  CameraClient &operator=(CameraClient &&other) = delete;

  void reply(const CameraResponse& res)const;
  CameraMessage readMessage()const ;
};

class CameraSocket {
  std::string socket_name;
  int socket_id;

  int createSocket() const;
  void bindSocket() const;
  // TODO: move this method to the client
  //  [[nodiscard]] CameraMessage readMessage(const int client_id) const;
  //  [[nodiscard]] bool writeMessage(const int client_id, const
  //  CameraResponse&msg) const;

  [[nodiscard]] CameraResponse processSnapMode(const CameraClient &client,
                                               const CameraMessage &msg) const;
  [[nodiscard]] CameraResponse
  processStartRecording(const CameraClient &client,
                        const CameraMessage &msg) const;
  [[nodiscard]] CameraResponse
  processEndRecording(const CameraClient &client,
                        const CameraMessage &msg) const;

public:
  mutable CameraDriver camera;
  CameraSocket(const char *const socket_name, const char *const camera_id);
  ~CameraSocket();

  CameraSocket(const CameraSocket &other) = delete;
  CameraSocket(CameraSocket &&other) = delete;
  CameraSocket &operator=(const CameraSocket &other) = delete;
  CameraSocket &operator=(CameraSocket &&other) = delete;

  void startListening() const;
  int accpetClient() const;
  void processClient(const CameraClient &client) const;
};

// MotorPath getDesiredPath(const int);
