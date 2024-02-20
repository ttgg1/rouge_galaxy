#include "SDL_error.h"
#include "utils.h"

#include "list.h"

list_t *li_emptyList() {
  list_t *list;
  list = (list_t *)malloc(sizeof(list_t));
  if (list == NULL) {
    debug_print("Failed alloc memory for linked_list ! \n");
  }
  list->size = 0;
  list->head = NULL;
  return list;
}

list_t *li_copy(list_t *list) {
  if (list->size == 0) {
    return li_emptyList();
  }
  list_t *cpy;
  list = (list_t *)malloc(sizeof(list_t));
  if (list == NULL) {
    debug_print("Failed alloc memory for linked_list ! \n");
    return NULL;
  }

  node_t *current = list->head;
  node_t *node;
  node_t *last;
  node = (node_t *)malloc(sizeof(node_t));
  if (node == NULL) {
    debug_print("Failed alloc memory for list_node ! \n");
    return NULL;
  }
  node->value = current->value;
  cpy->head = node;
  cpy->size = 1;

  current = current->next;

  while (current != NULL) {
    last = node;
    node = (node_t *)malloc(sizeof(node_t));
    if (node == NULL) {
      debug_print("Failed alloc memory for list_node ! \n");
      return NULL;
    }
    node->value = current->value;
    last->next = node;
    current = current->next;
    cpy->size++;
  }

  return cpy;
}

void li_destroyNode(node_t *node) {
  if (node->value != NULL) {
    free(node->value);
  }
  free(node);
}

void li_destroy(list_t *list) {
  node_t *current = list->head;
  node_t *temp;
  while (current != NULL) {
    temp = current->next;
    li_destroyNode(current);
    current = temp;
  }
  free(list);
}

void li_append(list_t *list, entity_t *value) {
  node_t *node;
  node = (node_t *)malloc(sizeof(node_t));
  if (node == NULL) {
    debug_print("Failed alloc memory for list_node ! \n");
    return;
  }
  node->value = value;
  node->next = NULL;

  if (list->head == NULL) {
    list->head = node;
    list->size = 1;
    return;
  }

  node_t *current = list->head;
  while (current->next != NULL) {

    current = current->next;
  }
  current->next = node;
  list->size++;
}

void li_push(list_t *list, entity_t *value) {
  node_t *node;
  node = (node_t *)malloc(sizeof(node_t));
  if (node == NULL) {
    debug_print("Failed alloc memory for list_node ! \n");
    return;
  }
  node->value = value;
  node->next = list->head;
  list->head = node;
  list->size++;
}

entity_t *li_pop(list_t *list) {
  if (list->head == NULL) {
    return NULL;
  }
  entity_t *value = list->head->value;
  node_t *temp = list->head->next;
  // li_destroyNode(list->head);
  list->head = temp;
  list->size--;
  return value;
}

void *li_getAtIndex(list_t *list, int index) {
  if (index > list->size) {
    return NULL;
  }
  node_t *current = list->head;
  while (current->next != NULL && index > 0) {
    current = current->next;
    index--;
  }
  if (index == 0) {
    return current->value;
  }
  return NULL;
}

node_t *li_getNodeAtIndex(list_t *list, int index) {
  if (index > list->size) {
    return NULL;
  }
  node_t *current = list->head;
  while (current->next != NULL && index > 0) {
    current = current->next;
    index--;
  }
  if (index == 0) {
    return current;
  }
  return NULL;
}

void li_setAtIndex(list_t *list, int index, void *value) {
  if (index > list->size) {
    return;
  }
  node_t *current = list->head;
  while (current->next != NULL && index > 0) {
    current = current->next;
    index--;
  }
  if (index == 0) {
    current->value = value;
  }
}

void li_removeIndex(list_t *list, int index) {
  if (index > list->size) {
    return;
  }
  node_t *current = list->head;
  node_t *last;
  while (current->next != NULL && index > 0) {
    last = current;
    current = current->next;
    index--;
  }
  if (index == 0) {
    last->next = current->next;
    // li_destroyNode(current);
  }
}
