#include "list.h"
#include "misc.h"
#include <stdlib.h>
#include <assert.h>

yev_cblst* new_yev_cblst()
{
  yev_cblst* lst = (yev_cblst*)malloc(sizeof(yev_cblst));
  if(!lst) yev_die("new_yev_cblst");
  lst->head = NULL;
  lst->last = NULL;
  return lst;
}

void yev_cblst_add(yev_cblst* lst, int fd, void(*cb)(int,short))
{
  yev_cblst_cell* cell = (yev_cblst_cell*)malloc(sizeof(yev_cblst_cell));
  if(!cell) yev_die("yev_cblst_add");
  cell->fd = fd;
  cell->cb = cb;
  cell->next = NULL;

  // if list is empty
  if(!lst->last) {
    assert(!lst->head);
    lst->head = cell;
    lst->last = cell;
    return;
  }

  // if list isn't empty
  lst->last->next = cell;
  lst->last = cell;
}

yev_cblst_cell* yev_cblst_find(yev_cblst* lst, int fd)
{
  yev_cblst_cell* cell = lst->head;
  if(!cell) return NULL;
  while(cell) {
    if (cell->fd == fd)
      return cell;
    cell = cell->next;
  }
  return NULL;
}

void yev_cblst_remove(yev_cblst* lst, int fd)
{
  if(!lst->head) return;
  if(lst->head->fd == fd) {
    yev_cblst_cell* cell = lst->head;
    lst->head = cell->next;
    if(cell == lst->last) {
      lst->last = NULL;
    }
    free(cell);
    return;
  }

  yev_cblst_cell* cell = lst->head;
  while(cell->next) {
    if(cell->next->fd == fd) {
      yev_cblst_cell* next = cell->next;
      cell->next = next->next;
      free(next);
    }
    cell = cell->next;
  }
}

void free_yev_cblst(yev_cblst* lst)
{
  yev_cblst_cell* cell = lst->head;
  while(cell) {
    yev_cblst_cell* next = cell->next;
    free(cell);
    cell = next;
  }
  free(lst);
}
