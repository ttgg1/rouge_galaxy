#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdint.h>
#include "entity.h"

// define LIST_NODE_TYPE before including 
typedef struct list_node_entity_t
{
  entity_t *value;
  struct list_node_entity_t *next;
} list_node_entity_t_t;

typedef struct list_node_int
{
  int *value;
  struct list_node_int *next;
} list_node_int_t;


typedef struct linked_list_entity_t
{
    list_node_entity_t_t *head;
    int size;
} linked_list_entity_t_t;

typedef struct linked_list_int
{
    list_node_int_t *head;
    int size;
} linked_list_int_t;
#endif // LIST_H_

#ifdef LIST_NODE_TYPE

#define TOKENPASTE2(x,y) x ## y
#define TOKENPASTE3(x,y,z) x ## y ## z

#define LIST_NODE(T) TOKENPASTE2(list_node_, T) 
#define LIST_NODE_T(T) TOKENPASTE3(list_node_, T, _t)

#define LIST_NODE_ LIST_NODE(LIST_NODE_TYPE)
#define LIST_NODE_T_ LIST_NODE_T(LIST_NODE_TYPE)


#define LINKED_LIST(T) TOKENPASTE2(linked_list_, T) 
#define LINKED_LIST_T(T) TOKENPASTE3(linked_list_, T, _t)

#define LINKED_LIST_ LINKED_LIST(LIST_NODE_TYPE)
#define LINKED_LIST_T_ LINKED_LIST_T(LIST_NODE_TYPE)


LINKED_LIST_T_ *li_empty_list();
LINKED_LIST_T_ *li_copy(LINKED_LIST_T_ *list);
void li_destroy(LINKED_LIST_T_ *list);
void li_append(LINKED_LIST_T_ *list, LIST_NODE_TYPE *value);
void li_push(LINKED_LIST_T_ *list, LIST_NODE_TYPE *value);
LIST_NODE_TYPE *li_pop(LINKED_LIST_T_ *list);
LIST_NODE_TYPE *li_get_at_index(LINKED_LIST_T_ *list, int index);
void li_set_at_index(LINKED_LIST_T_ *list, int index, LIST_NODE_TYPE *value);
void li_remove_index(LINKED_LIST_T_ *list, int index);
void li_destroy_node(LIST_NODE_T_ *node);

#endif // LIST_NODE_TYPE

