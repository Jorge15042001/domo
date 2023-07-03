#pragma once

#include "motorDriver.hpp"
#include "motorStructs.hpp"

#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

class MotorCient {

public:
  int client_id;
  MotorCient(const int client_id) : client_id(client_id) {}
  ~MotorCient() { close(this->client_id); }
  [[nodiscard]] bool reply(const MotorResponse& msg)const;

  MotorCient(const MotorCient &other) = delete;
  MotorCient(MotorCient &&other) = delete;
  MotorCient &operator=(const MotorCient &other) = delete;
  MotorCient &operator=(MotorCient &&other) = delete;
  
};

class MotorSocket {
  std::string socket_name;
  int socket_id;


  int createSocket() const;
  void bindSocket() const;
  //TODO: move this method to the client
  [[nodiscard]] MotorMessage readMessage(const int client_id) const;

  // void processHomeMode(const MotorCient &client, const MotorMessage &msg) const;
  // void processStepMode(const MotorCient &client, const MotorMessage &msg) const;
  // void processGoToMode(const MotorCient &client, const MotorMessage &msg) const;
  // void processContinuous(const MotorCient &client,
  //                        const MotorMessage &msg) const;

public:
  mutable MotorDriver motor;
  MotorSocket(const char *const socket_name, const char *const motor_port,
              const int bdrate);
  ~MotorSocket();

  MotorSocket(const MotorSocket &other) = delete;
  MotorSocket(MotorSocket &&other) = delete;
  MotorSocket &operator=(const MotorSocket &other) = delete;
  MotorSocket &operator=(MotorSocket &&other) = delete;

  void startListening() const;
  int accpetClient() const;
  void processClient(const MotorCient &client) const;
};

// MotorPath getDesiredPath(const int);
