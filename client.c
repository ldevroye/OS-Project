#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

#include "common.h"

int main(int argc, char const *argv[]) {
  // create client socket with default parameters
  int sock = checked(socket(AF_INET, SOCK_STREAM, 0));
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(8080);
  // Conversion de string vers IPv4 ou IPv6 en binaire
  checked(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr));
  checked(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)));

  // create thread pour gestion de la réception des messages
  pthread_t tids[1];  // thread ids
  pthread_create(&tids[0], NULL, recv_messages, NULL);
  
  send_messages();

  return 0;
}

void send_messages() {
  // envoi des messages
  char buffer[1024];
  ssize_t nbytes = 1;
  while (nbytes > 0 && fgets(buffer, 1024, stdin)) {    // lit l'input de stdin
    // Supprimer le \n
    size_t len = strlen(buffer);
    buffer[len - 1] = '\0';

    time_t timestamp;
    time_t now = time(&timestamp);

    // On garde la même taille de string pour explicitement envoyer le '\0'
    nbytes = ssend(sock, buffer, len, timestamp);                  // envoie le message
    if (nbytes > 0) {
      // TODO check envoi worked ?
    }
  }
}

void recv_messages() {
  while (true) {
    char *recvbuffer;
    time_t *time_stamp;
    struct tm *struct_time;
    struct_time = localtime(&timestamp);

    nbytes = receive(sock, (void *)&recvbuffer, (void *)&time_stamp);      // reçoit message
    if (nbytes > 0) {                                 // vérifie message a été reçu
      printf("%s : %s\n", asctime(time_stamp), recvbuffer);
      free(recvbuffer);
    }
  }
}


/*

int main(int argc, char const *argv[]) {
  int sock = checked(socket(AF_INET, SOCK_STREAM, 0));
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(8080);

  // Conversion de string vers IPv4 ou IPv6 en binaire
  checked(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr));

  checked(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)));
  char buffer[1024];
  ssize_t nbytes = 1;
  while (nbytes > 0 && fgets(buffer, 1024, stdin)) {
    // Supprimer le \n
    size_t len = strlen(buffer);
    buffer[len - 1] = '\0';
    // On garde la même taille de string pour explicitement envoyer le '\0'
    nbytes = ssend(sock, buffer, len);
    if (nbytes > 0) {
      char *recvbuffer;
      nbytes = receive(sock, (void *)&recvbuffer);
      if (nbytes > 0) {
        printf("Client received %s\n", recvbuffer);
        free(recvbuffer);
      }
    }
  }
  return 0;
}

*/