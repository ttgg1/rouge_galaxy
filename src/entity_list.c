#include "utils.h"
#include "SDL_error.h"

#include "entity_list.h"

entity_list_t *li_empty_list() {
    entity_list_t *list;
    list = (entity_list_t *) malloc(sizeof(entity_list_t));
    if (list == NULL) {
        debug_print("Failed alloc memory for linked_list ! \n");
    }
    list->size = 0;
    list->head = NULL;
    return list;
}

entity_list_t *li_copy(entity_list_t *list){
    if (list->size == 0) {
        return li_empty_list();
    }
    entity_list_t *cpy;
    list = (entity_list_t *) malloc(sizeof(entity_list_t));
    if (list == NULL) {
        debug_print("Failed alloc memory for linked_list ! \n");
        return;
    }

    entity_node_t *current = list->head;
    entity_node_t *node;
    entity_node_t *last;
    node = (entity_node_t *) malloc(sizeof(entity_node_t));
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
        node = (entity_node_t *) malloc(sizeof(entity_node_t));
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

void li_destroy_node(entity_node_t *node) {
if (node->value != NULL) {
        free(node->value);
    }
    free(node);
}

void li_destroy(entity_list_t *list) {
    entity_node_t *current = list->head;
    entity_node_t *temp;
    while (current != NULL) {
        temp = current->next;
        li_destroy_node(current);
        current = temp;
    }
    free(list);
}

void li_append(entity_list_t *list, entity_t *value) {
    entity_node_t *node;
    node = (entity_node_t *) malloc(sizeof(entity_node_t));
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

    entity_node_t *current = list->head;
    while (current->next != NULL) {
        
        current = current->next;
    }
    current->next = node;
    list->size++;
}

void li_push(entity_list_t *list, entity_t *value){
    entity_node_t *node;
    node = (entity_node_t *) malloc(sizeof(entity_node_t));
    if (node == NULL) {
        debug_print("Failed alloc memory for list_node ! \n");
        return;
    }
    node->value = value;
    node->next = list->head;
    list->head = node;
    list->size++;
}


entity_t *li_pop(entity_list_t *list){
    if (list->head == NULL) {
        return;
    }
    entity_t *value = list->head->value;
    entity_node_t *temp = list->head->next;
    li_destroy_node(list->head);
    list->head = temp;
    list->size--;
    return value;
}

entity_t *li_get_at_index(entity_list_t *list, int index) {
    if (index > list->size) {
        return;
    }
    entity_node_t *current = list->head;
    while (current->next != NULL && index > 0) {
        current = current->next;
        index--;
    }
    if (index == 0) {
        return current->value;
    }
    return;
}

entity_node_t *li_get_node_at_index(entity_list_t *list, int index) {
    if (index > list->size) {
        return;
    }
    entity_node_t *current = list->head;
    while (current->next != NULL && index > 0) {
        current = current->next;
        index--;
    }
    if (index == 0) {
        return current;
    }
}

void li_set_at_index(entity_list_t *list, int index, entity_t *value) {
    if (index > list->size) {
        return;
    }
    entity_node_t *current = list->head;
    while (current->next != NULL && index > 0) {
        current = current->next;
        index--;
    }
    if (index == 0) {
        current->value = value;
    }
}

void li_remove_index(entity_list_t *list, int index){
    if (index > list->size) {
        return;
    }
    entity_node_t *current = list->head;
    entity_node_t *last;
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

