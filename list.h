#ifndef __YAEVENT_LIST_H
#define __YAEVENT_LIST_H

#include <stddef.h>

typedef struct yev_cblst_cell {
  int fd;
  void (*cb)(int, short);

  struct yev_cblst_cell* next;
} yev_cblst_cell;
typedef struct yev_cblist {
  struct yev_cblst_cell* head;
  struct yev_cblst_cell* last;
} yev_cblst;

yev_cblst* new_yev_cblst();
void yev_cblst_add(yev_cblst* lst, int fd, void (*cb)(int,short));
yev_cblst_cell* yev_cblst_find(yev_cblst* lst, int fd);
void yev_cblst_remove(yev_cblst* lst, int fd);
void free_yev_cblst(yev_cblst* lst);

#endif
