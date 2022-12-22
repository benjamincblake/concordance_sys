#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#define LIST_MAXLEN_TEXT 100
#define MAX_WORD_LEN 40

typedef struct node_s {
    char index_word[MAX_WORD_LEN];
	char capped_word[MAX_WORD_LEN];
	char index_line[LIST_MAXLEN_TEXT];
    int adjustment;
    struct node_s  *next;
} node_s;

typedef struct node_t {
    char text[LIST_MAXLEN_TEXT];
    struct node_t  *next;
} node_t;

node_t *new_node(char *);
node_t *add_front(node_t *, node_t *);
node_t *add_end(node_t *, node_t *);
node_t *peek_front(node_t *);
node_t *remove_front(node_t *);
node_t *add_inorder(node_t * list, node_t *new);
node_s *new_struct_node(char *index_word, char *capped_word, char *index_line, int adjustment);
node_s *add_struct_inorder(node_s * list, node_s *new);
node_s *remove_struct_front(node_s *list);
void    apply(node_t *, void(*fn)(node_t *, void *), void *arg);
void apply_s(node_s *list, void (*fn)(node_s *list, void *), void *arg); 


#endif
