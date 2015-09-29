# yaevent

Yet another event library based on select(2) just for fun.

## Example

Simple TCP Server

```c
#include <stdio.h>
#include "yaevent.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void accept_cb(int sock, short t)
{
  struct sockaddr_in client;
  int len = sizeof(client);
  int csock = accept(sock, (struct sockaddr*)&client, (socklen_t*)&len);
  printf("accepted");
  write(csock, "OK", 2);
  close(csock);
}

int main()
{
  yaevent_base_t* yev = yaevent_init();

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sock, (struct sockaddr*)&addr, sizeof(addr));
  listen(sock, 5);

  yaevent_add(yev, sock, accept_cb, EVENT_READ);

  while(1) {
    yaevent_loop_once(yev);
    printf("loop\n");
  }

  yaevent_free(yev);

  return 0;
}

```
