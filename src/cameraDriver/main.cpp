#include "cameraStructs.hpp"
#include "socketHandler.hpp"

#include <cstddef>
#include <fmt/core.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

const char *SOCKET_NAME = "/tmp/camera.sock";
// const std::size_t BUFFER_SIZE = 12;

int main(int argc, char **argv) {
  gst_init(&argc, &argv);
  gst_debug_set_default_threshold(GST_LEVEL_WARNING);

  /* Create local socket. */

  CameraSocket socket{SOCKET_NAME, "07814191"};
  std::cout << "socket created" << std::endl;

  socket.camera.initializeCamera();
  // socket.camera.takeSnapshot("/tmp");
  // socket.camera.startRecording("/tmp/video");
  // sleep(10);
  // socket.camera.endRecording();

  socket.startListening();

  /* This is the main loop for handling connections. */

  for (;;) {

    fmt::print("waitting for connections\n");
    /* Wait for incoming connection. */
    CameraClient client{socket.accpetClient()};

    socket.processClient(client);
  }

  exit(EXIT_SUCCESS);
}
