/*
 * linkedlist.c
 *
 * Based on the implementation approach described in "The Practice 
 * of Programming" by Kernighan and Pike (Addison-Wesley, 1999).
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "seng265-list.h"


node_t *new_node(char *text) {
    assert( text != NULL);

    node_t *temp = (node_t *)emalloc(sizeof(node_t));

    strncpy(temp->text, text, LIST_MAXLEN_TEXT);
    temp->next = NULL;

    return temp;
}

node_t *add_front(node_t *list, node_t *new) {
    new->next = list;
    return new;
}


node_t *add_end(node_t *list, node_t *new) {
    node_t *curr;

    if (list == NULL) {
        new->next = NULL;
        return new;
    }

    for (curr = list; curr->next != NULL; curr = curr->next);
    curr->next = new;
    new->next = NULL;
    return list;
}


node_t *peek_front(node_t *list) {
    return list;
}


node_t *remove_front(node_t *list) {
    if (list == NULL) {
        return NULL;
    }

    return list->next;
}

node_t *add_inorder(node_t * list, node_t *new) {
    node_t *prev = NULL;
    node_t *curr = NULL;

    if (list == NULL) {
        return new;
    }

    for (curr = list; curr != NULL; curr = curr->next) {
        if (strcmp(new->text, curr->text) > 0) {
            prev = curr;
        } else {
            break;
        }
    }

    new->next = curr;

    if (prev == NULL) {
        return (new);
    } else {
        prev->next = new;
        return list;
    }
}

node_s *new_struct_node(char *index_word, char *capped_word, char *index_line, int adjustment) {

    node_s *temp = (node_s *)emalloc(sizeof(node_s));
    

    strncpy(temp->index_word, index_word, LIST_MAXLEN_TEXT);
    strncpy(temp->capped_word, capped_word, LIST_MAXLEN_TEXT);
    strncpy(temp->index_line, index_line, LIST_MAXLEN_TEXT);
    temp->adjustment = adjustment;
    temp->next = NULL;

    return temp;
}

node_s *add_struct_inorder(node_s * list, node_s *new) {
    node_s *prev = NULL;
    node_s *curr = NULL;

    if (list == NULL) {
        return new;
    }

    for (curr = list; curr != NULL; curr = curr->next) {
        if (strcmp(new->capped_word, curr->capped_word) >= 0) {
            prev = curr;
        } else {
            break;
        }
    }

    new->next = curr;

    if (prev == NULL) {
        return (new);
    } else {
        prev->next = new;
        return list;
    }
}

node_s *remove_struct_front(node_s *list) {
    if (list == NULL) {
        return NULL;
    }

    return list->next;
}

void apply(node_t *list,
           void (*fn)(node_t *list, void *),
           void *arg)
{
    for ( ; list != NULL; list = list->next) {
        (*fn)(list, arg);
    }
}

void apply_s(node_s *list,
           void (*fn)(node_s *list, void *),
           void *arg)
{
    for ( ; list != NULL; list = list->next) {
        (*fn)(list, arg);
    }
}


