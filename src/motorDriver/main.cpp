#include "motorStructs.hpp"
#include "socketHandler.hpp"

#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

const char * SOCKET_NAME =  "/tmp/resol.sock";
const std::size_t BUFFER_SIZE = 12;

int main() {

  /* Create local socket. */

  MotorSocket socket {SOCKET_NAME};
  socket.startListening();

  /* This is the main loop for handling connections. */

  for (;;) {

    /* Wait for incoming connection. */
    MotorCient client {socket.accpetClient()};

    socket.processClient(client);

  }



  exit(EXIT_SUCCESS);
}