#ifndef ENTITY_LIST_H_
#define ENTITY_LIST_H_

#include <stdlib.h>
#include <stdint.h>
#include "entity.h"


typedef struct entity_node
{
  entity_t *value;
  struct entity_node *next;
} entity_node_t;

typedef struct entity_list
{
    entity_node_t *head;
    int size;
} entity_list_t;


entity_list_t *li_emptyList();
entity_list_t *li_copy(entity_list_t *list);

void li_destroy(entity_list_t *list);
void li_destroyNode(entity_node_t *node);

void li_push(entity_list_t *list, entity_t *value);
void li_append(entity_list_t *list, entity_t *value);
void li_setAtIndex(entity_list_t *list, int index, entity_t *value);

entity_t *li_pop(entity_list_t *list);
entity_t *li_getAtIndex(entity_list_t *list, int index);
entity_node_t *li_getNodeAtIndex(entity_list_t *list, int index);
void li_removeIndex(entity_list_t *list, int index);

#endif  // ENTITY_LIST_H_