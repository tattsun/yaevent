#include <stdio.h>
#include "yaevent.h"

int main() {
  yaevent_base_t* yev = yaevent_init();

  while(1) {
    yaevent_loop_once(yev);
    printf("loop\n");
  }

  return 0;
}
