#include "pipe_networking.h"

int main() {
    int sd = server_setup();
    server_connect(sd);
}
