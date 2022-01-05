#include "pipe_networking.h"
#include <unistd.h>
#include <stdio.h>

void processing(int from_server) {
  char msg[100];
  printf("Input: ");
  fgets(msg, sizeof(msg), stdin);
  
  write(from_server, msg, sizeof(msg));

  char processed[100];
  int n = read(from_server, processed, sizeof(processed));
  if (n) {
    printf("Processed: %s\n", processed);
    processing(from_server);
  }
}

int main() {
  int to_server;
  int from_server;

  from_server = client_handshake();

  processing(from_server);
}
