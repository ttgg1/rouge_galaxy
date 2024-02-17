#ifndef LIST_NODE_TYPE
#define LIST_NODE_TYPE entity_t
#endif // LIST_NODE_TYPE

#include "utils.h"
#include "SDL_error.h"

#include "list.h"

LINKED_LIST_T_ *li_empty_list() {
    LINKED_LIST_T_ *list;
    list = (LINKED_LIST_T_ *) malloc(sizeof(LINKED_LIST_T_));
    if (list == NULL) {
        debug_print("Failed alloc memory for linked_list ! \n");
    }
    list->size = 0;
    list->head = NULL;
    return list;
}

LINKED_LIST_T_ *li_copy(LINKED_LIST_T_ *list){
    if (list->size == 0) {
        return li_empty_list();
    }
    LINKED_LIST_T_ *cpy;
    list = (LINKED_LIST_T_ *) malloc(sizeof(LINKED_LIST_T_));
    if (list == NULL) {
        debug_print("Failed alloc memory for linked_list ! \n");
        return;
    }

    LIST_NODE_T_ *current = list->head;
    LIST_NODE_T_ *node;
    LIST_NODE_T_ *last;
    node = (LIST_NODE_T_ *) malloc(sizeof(LIST_NODE_T_));
    if (node == NULL) {
        debug_print("Failed alloc memory for list_node ! \n");
        return;
    }
    node->value = current->value;
    cpy->head = node;
    cpy->size = 1;

    current = current->next;
    
    while (current != NULL) {
        last = node;
        node = (LIST_NODE_T_ *) malloc(sizeof(LIST_NODE_T_));
        if (node == NULL) {
            debug_print("Failed alloc memory for list_node ! \n");
            return;
        }
        node->value = current->value;
        last->next = node;
        current = current->next;
        cpy->size++;
    }

    return cpy;
}

void li_destroy_node(LIST_NODE_T_ *node) {
if (node->value != NULL) {
        free(node->value);
    }
    free(node);
}

void li_destroy(LINKED_LIST_T_ *list) {
    LIST_NODE_T_ *current = list->head;
    LIST_NODE_T_ *temp;
    while (current != NULL) {
        temp = current->next;
        li_destroy_node(current);
        current = temp;
    }
    free(list);
}

void li_append(LINKED_LIST_T_ *list, LIST_NODE_TYPE *value) {
    LIST_NODE_T_ *node;
    node = (LIST_NODE_T_ *) malloc(sizeof(LIST_NODE_T_));
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

    LIST_NODE_T_ *current = list->head;
    while (current->next != NULL) {
        
        current = current->next;
    }
    current->next = node;
    list->size++;
}

void li_push(LINKED_LIST_T_ *list, LIST_NODE_TYPE *value){
    LIST_NODE_T_ *node;
    node = (LIST_NODE_T_ *) malloc(sizeof(LIST_NODE_T_));
    if (node == NULL) {
        debug_print("Failed alloc memory for list_node ! \n");
        return;
    }
    node->value = value;
    node->next = list->head;
    list->head = node;
    list->size++;
}


LIST_NODE_TYPE *li_pop(LINKED_LIST_T_ *list){
    if (list->head == NULL) {
        return;
    }
    LIST_NODE_TYPE *value = list->head->value;
    LIST_NODE_T_ *temp = list->head->next;
    li_destroy_node(list->head);
    list->head = temp;
    list->size--;
    return value;
}

LIST_NODE_TYPE *li_get_at_index(LINKED_LIST_T_ *list, int index) {
    if (index > list->size) {
        return;
    }
    LIST_NODE_T_ *current = list->head;
    while (current->next != NULL && index > 0) {
        current = current->next;
        index--;
    }
    if (index == 0) {
        return current->value;
    }
    return;
}

void li_set_at_index(LINKED_LIST_T_ *list, int index, LIST_NODE_TYPE *value) {
    if (index > list->size) {
        return;
    }
    LIST_NODE_T_ *current = list->head;
    while (current->next != NULL && index > 0) {
        current = current->next;
        index--;
    }
    if (index == 0) {
        current->value = value;
    }
}

void li_remove_index(LINKED_LIST_T_ *list, int index){
    if (index > list->size) {
        return;
    }
    LIST_NODE_T_ *current = list->head;
    LIST_NODE_T_ *last;
    while (current->next != NULL && index > 0) {
        last = current;
        current = current->next;
        index--;
    }
    if (index == 0) {
        last->next = current->next;
        li_destroy_node(current);
    }
}

