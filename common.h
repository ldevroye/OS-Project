#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int _checked(int ret, char* calling_function) {
  if (ret < 0) {
    perror(calling_function);
    exit(EXIT_FAILURE);
  }
  return ret;
}

// The macro allows us to retrieve the name of the calling function
#define checked(call) _checked(call, #call)

/**
 * @brief Send data under the form <size_t len><...data>
 * Function name is 'ssend' instead of 'send' because the latter already exists.
 */
int ssend(int sock, void* data, size_t len, void* tstamp) {
  checked(write(sock, &len, sizeof(len)));        // envoie la length du message
  checked(write(sock, tstamp, sizeof(tstamp)));   // envoie la timestamp
  return checked(write(sock, data, len));         // envoie message
}

/**
 * @brief Receive data under the form <size_t len><data...>.
 */
size_t receive(int sock, void** dest, void** tstamp) {
  size_t nbytes_to_receive;
  if (checked(read(sock, &nbytes_to_receive, sizeof(nbytes_to_receive))) == 0) {
    // Connection closed
    return 0;
  };

  size_t t_length = 8;
  unsigned char* t_buffer = malloc(t_length);
  if (t_buffer == NULL) {
    fprintf(stderr, "malloc could not allocate %zd bytes", t_length);
    perror("");
    exit(1);
  }
  checked(read(sock, &t_buffer, t_length));
  *tstamp = t_buffer;

  unsigned char* buffer = malloc(nbytes_to_receive);
  if (buffer == NULL) {
    fprintf(stderr, "malloc could not allocate %zd bytes", nbytes_to_receive);
    perror("");
    exit(1);
  }
  size_t total_received = 0;
  while (nbytes_to_receive > 0) {
    size_t received = checked(read(sock, &buffer[total_received], nbytes_to_receive));
    if (received < 0) {
      return total_received;
    }
    total_received += received;
    nbytes_to_receive -= received;
  }
  *dest = buffer;

  return total_received;
}

#endif  // __COMMON_H
