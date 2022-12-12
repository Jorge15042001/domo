#pragma once

#include "motorStructs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int createSocket (const char*);

void bindSocket(const int, const char*);

void startListening(const int);

int accpetClient(const int listen_socket);

MotorPath getDesiredPath(const int);
