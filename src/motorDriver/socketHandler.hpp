#pragma once

#include "motorStructs.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

class MotorSocket {
  std::string socket_name;
  int socket_id;

  int createSocket() const;
  void bindSocket() const;
  MotorMessage readMessage(const int client_id) const;
  void processHomeMode(const MotorMessage &msg) const;
  void processStepMode(const MotorMessage &msg) const;
  void processContinuous(const MotorMessage &msg) const;

public:
  MotorSocket(const char *const socket_name);
  ~MotorSocket();

  MotorSocket(const MotorSocket &other) = delete;
  MotorSocket(MotorSocket &&other) = delete;
  MotorSocket &operator=(const MotorSocket &other) = delete;
  MotorSocket &operator=(MotorSocket &&other) = delete;

  void startListening() const;
  int accpetClient() const;
  void processClient(const int client_id) const;
};

// MotorPath getDesiredPath(const int);
