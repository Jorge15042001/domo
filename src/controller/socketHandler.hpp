#pragma once

#include "../motorDriver/motorStructs.hpp"


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string>
#include <optional>


class MotorSocketClient{
  int id;
  const std::string socket_name;

public:
  MotorSocketClient(const char* const socket_name);
  ~MotorSocketClient();
  void connectToSocket();

  [[nodiscard]] bool sendMessage(const MotorMessage& msg)const ;
  [[nodiscard]] std::optional<MotorResponse> readResponse()const ;

  MotorSocketClient(const MotorSocketClient &other) = delete;
  MotorSocketClient(MotorSocketClient&&other) = delete;
  MotorSocketClient &operator=(const MotorSocketClient&other) = delete;
  MotorSocketClient &operator=(MotorSocketClient &&other) = delete;

};

// int createSocket();
// void connectToSocket(const int,const char*);
// void sendDesiredPath(const int,const MotorPath *const);
// void sendMoveStep(const int, const int step);
// void runHomeMode(const int motorClient);
