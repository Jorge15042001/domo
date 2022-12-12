#define SOCKET_NAME "/tmp/resol.sock"
#define BUFFER_SIZE 12

#include "socketHandler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int down_flag = 0;
    char buffer[BUFFER_SIZE];

    /* Create local socket. */

    const int listen_socket = createSocket(SOCKET_NAME);
    bindSocket(listen_socket,SOCKET_NAME);
    startListening(listen_socket);

    /* This is the main loop for handling connections. */

    for (;;) {

        /* Wait for incoming connection. */

        const int data_socket = accpetClient(listen_socket); 

        MotorPath desiredPath  = getDesiredPath(data_socket);
        printf("desiredPath %ld %ld\n",desiredPath.initalPosition,desiredPath.endPositon);
        sleep(3);
        const int success = 1;
        write(data_socket,&success,sizeof(int));

        close(data_socket);

        /* Quit on DOWN command. */

    }

    close(listen_socket);

    /* Unlink the socket. */

    unlink(SOCKET_NAME);

    exit(EXIT_SUCCESS);
}
