#include "yaevent.h"
#include "misc.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

yaevent_base_t* yaevent_init()
{
  yaevent_base_t* yev = (yaevent_base_t*)malloc(sizeof(yaevent_base_t));
  if(!yev) yev_die_noerror("yaevent_init");

  yev->read_cbs = new_yev_cblst();
  yev->write_cbs = new_yev_cblst();
  yev->except_cbs = new_yev_cblst();

  return yev;
}

void yaevent_add(yaevent_base_t* yev, int fd, void(*callback)(int,short), short event_type)
{
  switch (event_type){
  case EVENT_READ:
    yev_cblst_add(yev->read_cbs, fd, callback);
    break;
  case EVENT_WRITE:
    yev_cblst_add(yev->write_cbs, fd, callback);
    break;
  case EVENT_EXCEPT:
    yev_cblst_add(yev->except_cbs, fd, callback);
    break;
  default:
    yev_die_noerror("unknown event_type");
  }
}

void yaevent_del(yaevent_base_t* yev, int fd, short event_type)
{
  switch(event_type) {
  case EVENT_READ:
    yev_cblst_remove(yev->read_cbs, fd);
    break;
  case EVENT_WRITE:
    yev_cblst_remove(yev->write_cbs, fd);
    break;
  case EVENT_EXCEPT:
    yev_cblst_remove(yev->except_cbs, fd);
    break;
  default:
    yev_die_noerror("unknown event_type");
  }
}

void yaevent_loop_once(yaevent_base_t* yev)
{
  fd_set readfds;
  fd_set writefds;
  fd_set exceptfds;
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  FD_ZERO(&exceptfds);

  int fd_max = 0;

  yev_cblst_cell* cb;
  cb = yev->read_cbs->head;
  while(cb) {
    if(cb->fd > fd_max) fd_max = cb->fd;
    FD_SET(cb->fd, &readfds);
    cb = cb->next;
  }
  cb = yev->write_cbs->head;
  while(cb) {
    if(cb->fd > fd_max) fd_max = cb->fd;
    FD_SET(cb->fd, &writefds);
    cb = cb->next;
  }
  cb = yev->except_cbs->head;
  while(cb) {
    if(cb->fd > fd_max) fd_max = cb->fd;
    FD_SET(cb->fd, &exceptfds);
    cb = cb->next;
  }

  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 500000;

  int fd = select(fd_max+1, &readfds, &writefds, &exceptfds, &tv);

  cb = yev->read_cbs->head;
  while(cb) {
    if(FD_ISSET(cb->fd, &readfds))
      cb->cb(cb->fd, EVENT_READ);
    cb = cb->next;
  }
  cb = yev->write_cbs->head;
  while(cb) {
    if(FD_ISSET(cb->fd, &writefds))
      cb->cb(cb->fd, EVENT_WRITE);
    cb = cb->next;
  }
  cb = yev->except_cbs->head;
  while(cb) {
    if(FD_ISSET(cb->fd, &exceptfds))
      cb->cb(cb->fd, EVENT_EXCEPT);
    cb = cb->next;
  }
}

void yaevent_dispatch(yaevent_base_t* yev)
{
  while(1)
    yaevent_loop_once(yev);
}

void yaevent_free(yaevent_base_t* yev)
{
  free_yev_cblst(yev->read_cbs);
  free_yev_cblst(yev->write_cbs);
  free_yev_cblst(yev->except_cbs);
  free(yev);
}
