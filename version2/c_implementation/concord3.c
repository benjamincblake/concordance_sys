/*
 * concord3.c
 *
 * Starter file provided to students for Assignment #3, SENG 265,
 * Fall 2022.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "emalloc.h"
#include "seng265-list.h"

/*
 * Compile-time constants
 */

#define MAX_LINE_LEN 100
#define MAX_WORD_LEN 40

/*
 * Function prototypes
 */

node_s *index_generator(char *line, node_t *excl,node_s *head);
void make_upper(char *w);
void index_sentence(char *s, char* w, char* cap_w);
int format(char *sentence, char* word);
void slice(const char *str, char *result, size_t start, size_t end);
void str_remove_back(char *sentence);
void str_remove_front(char *sentence);
char *get_front(char *s);
char *get_back(char *s);
int get_index(char* s, char *w, int len);

/*
 * Helper Functions
 */

/*
 * ----------------------------------------------------------
 * input();
 * takes stdin minus the first two lines
 * and allocates them to a linked list the returns it.
 * ----------------------------------------------------------
*/

node_t *input(){
    char version[1];
	char first_line[5];
	
	fgets(version, MAX_LINE_LEN, stdin);
	fgets(first_line, MAX_LINE_LEN, stdin);

    if(version[0] == '1'){
        printf("Input is version 1, concord3 expected version 2");
        exit(0);
    }
    
    char temp[MAX_LINE_LEN];
    node_t *temp_node = NULL;
    node_t *head = NULL;


    while(fgets(temp, MAX_LINE_LEN, stdin)){
        temp[strcspn(temp, "\n")] = 0;
        temp_node = new_node(temp);
        head = add_end(head, temp_node);
    }
    
    return head;
}

/*
 * ----------------------------------------------------------
 * *allocate_excl(node_t *input);
 * Input: input lines linked list
 * Output: A linked list of the exclusion words
 * ----------------------------------------------------------
*/

node_t *allocate_excl(node_t *input){
    char seperator[4] = "\"\"\"\"";
    node_t *temp = NULL;
    node_t *head = NULL;
    char curr_line[MAX_LINE_LEN];
    strncpy(curr_line, input->text, MAX_LINE_LEN);

    while(strncmp(seperator, curr_line, 4) != 0){
        temp = new_node(curr_line);
        head = add_end(head, temp);
        input = input->next;
        strncpy(curr_line, input->text, MAX_LINE_LEN);
    }

    return head;

}

/*
 * ----------------------------------------------------------
 * *allocate_index_lines(node_t *input);
 * Input: input lines linked list
 * Output: A linked list of the lines for indexing
 * ----------------------------------------------------------
*/

node_t *allocate_index_lines(node_t *input){
    char seperator[4] = "\"\"\"\"";
    node_t *temp = NULL;
    node_t *head = NULL;
    int check = 0;
    char curr_line[MAX_LINE_LEN];
    strncpy(curr_line, input->text, MAX_LINE_LEN);

    while(input != NULL){
        strncpy(curr_line, input->text, MAX_LINE_LEN);
        if(strncmp(seperator, curr_line, 4) == 0){
            check = 1;
        }else if(check == 1){
            temp = new_node(curr_line);
            head = add_end(head, temp);
        }
        input = input->next;
        
    }

    return head;

}

/*
 * ----------------------------------------------------------
 * *generate_index_lines(node_t *input, node_t *excl)
 * Input: lines_for_indexing linked list, excl linked list
 * Output: generates a linked list of type node_s (custom)
 * which contains a fully formatted index line aswell as
 * supplemental data
 * ----------------------------------------------------------
*/

node_s *generate_index_lines(node_t *input, node_t *excl){
    node_s *head = NULL;
    node_s *temp = NULL;
    char curr_line[MAX_LINE_LEN];
    while(input != NULL){
        strncpy(curr_line, input->text, MAX_LINE_LEN);
        head = index_generator(curr_line, excl, head);
        input = input->next;
    }

    while(temp != NULL){
        strncpy(curr_line, temp->index_line, MAX_LINE_LEN);

        temp = temp->next;
    }
    
    return head;
}

/*
 * ------------------------------------------------------------------
 * *index_generator(char *line, node_t *excl, node_s *head);
 * Input: line_for_indexing, list of excl_words, list of index_lines
 * Output: A updated index_lines list with all possible indexed 
 * sentences in the original sentence added
 * ------------------------------------------------------------------
*/

node_s *index_generator(char *line, node_t *excl, node_s *head){
    char *t;
    node_t *temp = excl;
    int freq = 0;	
    int len;
    int excl_len;
    char line_copy[MAX_LINE_LEN];
    strncpy(line_copy, line, MAX_LINE_LEN);
    char line_copy2[MAX_LINE_LEN];
    strncpy(line_copy2, line, MAX_LINE_LEN);
    node_s *temp_node = NULL;
    char *capped_word = emalloc(sizeof(char) * MAX_WORD_LEN);
    char *capped_excl = emalloc(sizeof(char) * MAX_WORD_LEN);
    

    t = strtok(line, " ");
    while (t) {
        strncpy(capped_word, t, MAX_WORD_LEN);
        make_upper(capped_word);
        while(temp != NULL){
            char *curr_excl = temp->text;
            strncpy(capped_excl, curr_excl, MAX_WORD_LEN);
            make_upper(capped_excl);
            excl_len = strlen(curr_excl);
            len = strlen(t);
            
            if(strncmp(capped_word, capped_excl, excl_len) == 0){
                if(excl_len == len){
                    freq++;
                }
            }
            temp = temp->next;	
            }

        if(freq == 0){
            strncpy(line_copy2, line_copy, MAX_LINE_LEN);
            index_sentence(line_copy2, t, capped_word);
            int adjustment = format(line_copy2, capped_word);
            temp_node = new_struct_node(t, capped_word, line_copy2, adjustment); 
            head = add_struct_inorder(head, temp_node);
        }
	
        freq = 0;
        temp = excl;
        t = strtok(NULL, " ");
    }

    free(capped_word);
    free(capped_excl);
    
    return head;
}

/*
 * ------------------------------------------------------------------
 * get_index(char* s, char *w, int len);
 * Inspiration from: https://stackoverflow.com/questions/20213799/finding-last-occurence-of-string
 * Input: haystack, needle, length of needle
 * Output: index of last occurence of needle
 * ------------------------------------------------------------------
*/

int get_index(char* s, char *w, int len){
    if (w == NULL || s == NULL) {
        return 0;
    }

    char *cp;
    int slen = strlen(s);
    for (cp = s + slen; cp >= s; cp--)
    {
    if (strncmp(cp, w, len) == 0)
        break;
    }
    
    int pos = (int) (cp - s);

    return pos;
}

/*
 * ------------------------------------------------------------------
 * format(char *sentence, char* word);
 * Input: the sentence to format, word 
 * Output: the number of spaces to add to the front of the formatted
 * sentence
 * ------------------------------------------------------------------
*/

int format(char *sentence, char* word){
    int index_len = strlen(word);
    int pos = get_index(sentence, word, index_len);
    
    while (pos > 29){
        str_remove_front(sentence);
        pos = get_index(sentence, word, index_len);
    }

    int adjustment = 29 - pos;

    char *first_word = get_front(sentence);
    char* location = strstr(sentence, first_word);
    int f_index = (int) (location - sentence);

    f_index += adjustment;

    while(f_index < 9){
        str_remove_front(sentence);
        pos = get_index(sentence, word, index_len);
        adjustment = 29 - pos;
        first_word = get_front(sentence);
        location = strstr(sentence, first_word);
        f_index = (int) (location - sentence);
        f_index += adjustment;
    }

    char *last_word = get_back(sentence);

    if(last_word != NULL){
        int l_len = strlen(last_word);
        int l_index = get_index(sentence, last_word, l_len);
        l_index += l_len;
        l_index += adjustment;

        while (l_index > 60){
            str_remove_back(sentence);
            last_word = get_back(sentence);
            
            if (last_word == NULL){
                break;
            }

            l_len = strlen(last_word);
            l_index = get_index(sentence, last_word, l_len);
            l_len = strlen(last_word);
            l_index += l_len;
            l_index += adjustment;
        }
    }

    int slen = strlen(sentence) - 1;
    if(sentence[slen] == ' '){
        while(!(isalpha(sentence[slen]))){
            sentence[slen] = 0;
            slen--;
        }
    }

    pos = get_index(sentence, word, index_len);
    adjustment = 29 - pos;

    
    return adjustment;
}


/*
 * ------------------------------------------------------------------
 * *get_front(char *s);
 * Input: sentence
 * Output: the first word
 * ------------------------------------------------------------------
*/

char *get_front(char *s){
    //char *temp = NULL;
    //temp = strchr(s, ' ');
    char temp[MAX_WORD_LEN];
    int i;

    for (i = 0; i <  strlen(s); i++)
    {
        if (isalpha(s[i]))
            temp[i] = s[i];

        if (s[i] == ' ')
            break;
    }

    temp[i] = '\0';
    char *f_word = temp;

    return f_word;
    
}

/*
 * ------------------------------------------------------------------
 * *get_back(char *s);
 * Input: sentence
 * Output: the last word
 * ------------------------------------------------------------------
*/

char *get_back(char *s){
    char *temp = NULL;
    
    temp = strrchr(s, ' ');

    return temp;
}

/*
 * ------------------------------------------------------------------
 * str_remove_front(char *sentence);
 * Input: sentence
 * Output: void, but the first word is removed
 * ------------------------------------------------------------------
*/

void str_remove_front(char *sentence){
    char *temp = NULL;
    temp = strchr(sentence, ' ');
    if(temp != NULL){
        temp = temp + 1;
        strncpy(&sentence[0], temp, MAX_LINE_LEN);
    }
    
}

/*
 * ------------------------------------------------------------------
 * str_remove_back(char *sentence);
 * Input: sentence
 * Output: void, but the last word is removed
 * ------------------------------------------------------------------
*/

void str_remove_back(char *sentence){
    char *temp = NULL;
    temp = strrchr(sentence, ' ');
    int temp_len = strlen(temp);
    int index = get_index(sentence, temp, temp_len);
    int len = strlen(temp);
    index += len;

    int i;

    for(i=index; i>0; i--){
        if (isalpha(sentence[i]))
            sentence[i] = 0;

        if (sentence[i] == ' ')
            break;
    }

    sentence[i] = '\0';

}

/*
 * ------------------------------------------------------------------
 * print_index_word(node_s *node, void *arg);
 * Input: node of type s
 * Output: void, prints node_s's index line
 * ------------------------------------------------------------------
*/

void print_index_word(node_s *node, void *arg)
{
    char *format = (char *)arg;
    printf("%*c", node->adjustment, ' ');
    printf(format, node->index_line);
}

/*
 * ------------------------------------------------------------------
 * void make_upper(char *w);
 * Input: word
 * Output: void, uppercases word
 * ------------------------------------------------------------------
*/

void make_upper(char *w){

	int len = strlen(w);

	for(int i=0; i < len; i++){
		w[i] = toupper(w[i]);	
	}

}

/*
 * ------------------------------------------------------------------
 * index_sentence(char *s, char* w, char* cap_w);
 * Input: sentence, word, capitalized word
 * Output: void, replaces word with capped word in sentence
 * ------------------------------------------------------------------
*/

void index_sentence(char *s, char* w, char* cap_w){
    char curr_line[MAX_LINE_LEN];
    strncpy(curr_line, cap_w, MAX_LINE_LEN);
	int len = strlen(w);
	int pos = get_index(s, w, strlen(w));
	strncpy(&s[pos], curr_line, len);
}

/*
 * ------------------------------------------------------------------
 * free_list(node_t *head)
 * Input: linked list of type node_t
 * Output: void, frees the list in memory
 * ------------------------------------------------------------------
*/

void free_list(node_t *head){
    node_t *temp_node = head;
    while (temp_node != NULL) {
        assert(temp_node != NULL);
        head = remove_front(head);
        free(temp_node);
        temp_node = head;
    }
   
    assert(head == NULL); 
}

/*
 * ------------------------------------------------------------------
 * free_list_s(node_s *head);
 * Input: linked list of type node_s
 * Output: void, frees the list in memory
 * ------------------------------------------------------------------
*/

void free_list_s(node_s *head){
    node_s *temp_node = head;
    while (temp_node != NULL) {
        assert(temp_node != NULL);
        head = remove_struct_front(head);
        free(temp_node);
        temp_node = head;
    }
   
    assert(head == NULL); 
}

/*
 * Main Driver Code
 */

int main(int argc, char *argv[])
{
    node_t *input_lines = input();
    node_t *excl_words = allocate_excl(input_lines);
    node_t *lines_for_indexing = allocate_index_lines(input_lines);
    node_s *index_lines = generate_index_lines(lines_for_indexing, excl_words);

    apply_s(index_lines, print_index_word, "%s\n");

    free_list(input_lines);
    free_list(excl_words);
    free_list(lines_for_indexing);
    free_list_s(index_lines);

    exit(0);

}
