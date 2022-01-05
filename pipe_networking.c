#include "pipe_networking.h"

/*=========================
  server_setup
  args:

  creates the WKP (upstream) and opens it, waiting for a
  connection.

  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  //use getaddrinfo
  struct addrinfo * hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = AI_PASSIVE; //only needed on server
  getaddrinfo(NULL, 9845, hints, &results);  //Server sets node to NULL

  //create socket
  int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  bind(sd, results->ai_addr, results->ai_addrlen);

  free(hints);
  freeaddrinfo(results);
  
  return sd;
}

/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  listen(sd, 10);
  int client_socket;
  socklen_t sock_size;
  struct sockaddr_storage client_address;
  sock_size = sizeof(client_address);
  client_socket = accept(sd,(struct sockaddr *)&client_address, &sock_size);
  
  return client_socket;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client, int from_client) {
    char secret_name[100];
    read(from_client, secret_name, sizeof(secret_name));
    printf("Received secret name: %s\n", secret_name);
    
    *to_client = open(secret_name, O_WRONLY);
    write(*to_client, secret_name, sizeof(secret_name));
    
    char response[100];
    read(from_client, response, sizeof(response));
    printf("%s\n", response);
    
    return from_client;
}

/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake() {
  //use getaddrinfo
  struct addrinfo * hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = AI_PASSIVE; //only needed on server
  getaddrinfo(NULL, 9845, hints, &results);  //Server sets node to NULL
  
  //create socket
  int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  connect(sd, results->ai_addr, results->ai_addrlen);
    
  return sd;
}
