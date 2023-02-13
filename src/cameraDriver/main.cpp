#include "cameraStructs.hpp"
#include "socketHandler.hpp"

#include <cstddef>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

const char * SOCKET_NAME =  "/tmp/resol.sock";
// const std::size_t BUFFER_SIZE = 12;

int main() {

  /* Create local socket. */

  CameraSocket socket {SOCKET_NAME,"/dev/ttyACM0",115200};
  std::cout<<"socket created"<<std::endl;

  socket.camera.initializeCamera();
  // socket.motor.goToPosition(5);
  // socket.motor.moveMilimiters(100);
  // socket.motor.moveContinuous(5, 100);

  socket.startListening();


  /* This is the main loop for handling connections. */

  for (;;) {

    /* Wait for incoming connection. */
    CameraClient client {socket.accpetClient()};

    socket.processClient(client);

  }



  exit(EXIT_SUCCESS);
}