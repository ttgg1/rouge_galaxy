#ifndef _LIST_H_
#define _LIST_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct node {
  void *value;
  struct node *next;
} node_t;

typedef struct list {
  node_t *head;
  int size;
} list_t;

list_t *li_emptyList();
list_t *li_copy(list_t *list);

void li_destroy(list_t *list);
void li_destroyNode(node_t *node);

void li_push(list_t *list, void *value);
void li_append(list_t *list, void *value);
void li_setAtIndex(list_t *list, int index, void *value);

void *li_pop(list_t *list);
void *li_getAtIndex(list_t *list, int index);
node_t *li_getNodeAtIndex(list_t *list, int index);
void li_removeIndex(list_t *list, int index);

#endif // _LIST_H_
