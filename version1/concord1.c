/* concord1.c
 * "Takes a specific file input for indexing and outputs the indexed file
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* 
 * Compile-time constants 
 */

#define MAX_EXCLUSION_LEN 21
#define MAX_EXCLUSION_WORDS 20
#define MAX_INDEX_LINES 100
#define MAX_INDEX_LEN 70
#define MAX_INDEX_WORDS 100

/* 
 * struct declaration 
 */

typedef struct index_lines_t {
    char index_word[MAX_EXCLUSION_LEN];
	char capped_word[MAX_EXCLUSION_LEN];
	char index_line[MAX_INDEX_LEN];
} index_line;


/* 
 * Global Var's 
 */

int num_index_words = 0;
int num_excl_words = 0;
int num_lines = 0;
char excl_words[MAX_EXCLUSION_WORDS][MAX_EXCLUSION_LEN];
char input_lines[MAX_INDEX_LINES][MAX_INDEX_LEN];
char index_words[MAX_INDEX_WORDS][MAX_EXCLUSION_LEN];
index_line index_lines[MAX_INDEX_LINES];


/*
 *Function Prototypes 
 */

void line_by_line(void);
void allocate(void);
void tokenize_line (char *, int);
int compare_words(const void *, const void *);
void make_upper(char *);
void index_sentence(char *, char* , int);

/*
 * Main Driver Code
 */

int main(int argc, char *argv[]) {

        line_by_line();
        allocate();
	qsort(index_lines, num_index_words, sizeof(index_line), compare_words);			
	
	int loop;

	for(loop = 0; loop < num_index_words; loop++){
		printf("%s", index_lines[loop].index_line);
	}

	exit(0);
}


/*
 * Helper Functions
 */

/*
 * line by line takes all contents of file and allocates them to the input array
 */

void line_by_line() {
	char version[1];
	char first_line[5];
	
	fgets(version, MAX_INDEX_LEN, stdin);
	fgets(first_line, MAX_INDEX_LEN, stdin);

	while(fgets(input_lines[num_lines], MAX_INDEX_LEN, stdin)){
		num_lines++;				
	}

}

/*
 *allocate takes the input array and allocates the exclusion words and lines for indexing apropriately
 */

void allocate() {
	int iterator = 0;
	char curr_line[MAX_INDEX_LEN];
	char seperator[4] = "\"\"\"\"";
	while(strncmp(seperator, input_lines[iterator], 4) != 0){
		strncpy(excl_words[num_excl_words], input_lines[iterator], MAX_EXCLUSION_LEN);
		excl_words[num_excl_words][strcspn(excl_words[num_excl_words], "\n")] = 0;
		num_excl_words++;
		iterator++;
	}
		

	while(iterator < num_lines){
		iterator++;
		strncpy(curr_line, input_lines[iterator], MAX_INDEX_LEN);
		curr_line[strcspn(curr_line, "\n")] = 0;
		/*getting rid of newline character with a little magic from https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input */
		tokenize_line(curr_line, iterator);
		memset(curr_line, 0, MAX_INDEX_LEN);
	}


}


/*
 * tokenize_line tokenizes given line and allocates any index words into struct array
 */

void tokenize_line (char *input_line, int curr_line) {
    char *t;
    int iter = 0;    
    int freq = 0;	
    int len;
    int excl_len;

    t = strtok (input_line, " ");
    while (t) {
	while(iter < num_excl_words){
		char *curr_excl = excl_words[iter];
		excl_len = strlen(curr_excl);
		len = strlen(t);
		
		if(strncmp(t, excl_words[iter], excl_len) == 0){
                	if(excl_len == len){
				freq++;
			}
                }

		iter++;				
	}
	if(freq == 0){
        	strncpy(index_lines[num_index_words].index_word, t, MAX_INDEX_LEN);
		strncpy(index_lines[num_index_words].capped_word, t, MAX_INDEX_LEN);
		make_upper(index_lines[num_index_words].capped_word);
		strncpy(index_lines[num_index_words].index_line, input_lines[curr_line], MAX_INDEX_LEN);
        	index_sentence(index_lines[num_index_words].index_line, index_lines[num_index_words].index_word, num_index_words);
		num_index_words++;
	}
	
	freq = 0;
	iter = 0;
        t = strtok(NULL, " ");
    }

    return;
}

/*
 * comparator function for qsort
 */

int compare_words(const void *wa, const void *wb) {
    index_line *p_wa = (index_line *)wa;
    index_line *p_wb = (index_line *)wb;

    return strcmp(p_wa->index_word, p_wb->index_word);
}

/*
 * make_upper changes word given to uppercase
 */

void make_upper(char *w){

	int len = strlen(w);

	for(int i=0; i < len; i++){
		w[i] = toupper(w[i]);	
	}

}

/*
 * index_sentence capitalizes the indexed word in a given sentence
 */

void index_sentence(char *s, char* w, int ref){
	int len = strlen(w);
	char* location = strstr(s, w);
	int pos = (int) (location - s);
	strncpy(&index_lines[ref].index_line[pos], index_lines[ref].capped_word, len);
	
}







