#include "socketHandler.h"
#include "motorStructs.h"
#include <unistd.h>

int createSocket(const char *SOCKET_NAME) {
  /*
   * In case the program exited inadvertently on the last run,
   * remove the socket.
   */

  unlink(SOCKET_NAME);

  /* Create local socket. */

  const int listen_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (listen_socket == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  return listen_socket;
}
void bindSocket(const int listen_socket, const char *SOCKET_NAME) {

  struct sockaddr_un addr;
  /*
   * For portability clear the whole structure, since some
   * implementations have additional (nonstandard) fields in
   * the structure.
   */

  memset(&addr, 0, sizeof(struct sockaddr_un));

  /* Bind socket to socket name. */

  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

  int ret = bind(listen_socket, (const struct sockaddr *)&addr,
                 sizeof(struct sockaddr_un));
  if (ret == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
}
void startListening(const int listen_socket) {
  int ret = listen(listen_socket, 20);
  if (ret == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
}
int accpetClient(const int listen_socket) {

  const int data_socket = accept(listen_socket, NULL, NULL);
  if (data_socket == -1) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  return data_socket;
}

MotorPath getDesiredPath(const int listen_socket) {
  MotorPath path;
  const int result = read(listen_socket, &path, sizeof(MotorPath));
  if (result == -1) {
    perror("Failed to read desired path");
    //TODO: dont exit
    exit(EXIT_FAILURE);
  }
  return path;
}
