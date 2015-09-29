#ifndef __YAEVENT_YAEVENT_H
#define __YAEVENT_YAEVENT_H

#include "list.h"

typedef struct {
  yev_cblst* read_cbs;
  yev_cblst* write_cbs;
  yev_cblst* except_cbs;
} yaevent_base_t;

#define EVENT_READ 0
#define EVENT_WRITE 1
#define EVENT_EXCEPT 2

yaevent_base_t* yaevent_init();
void yaevent_add(yaevent_base_t*, int fd, void(*callback)(int,short), short event_type);
void yaevent_del(yaevent_base_t*, int fd, short event_type);
void yaevent_loop_once(yaevent_base_t*);
void yaevent_dispatch(yaevent_base_t*);
void yaevent_free(yaevent_base_t*);

#endif
