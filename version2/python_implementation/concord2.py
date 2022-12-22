#!/usr/bin/env python3
import sys

"""
GLOBAL VARIABLES
"""
exclusion_words = []
lines_for_indexing = []
index_lines = []

"""
MAIN DRIVER CODE
"""

def main() -> None:
    version = int(sys.stdin.readline())
    if(version == 1):
        print("Input is version 1, concord2.py expected version 2")
        exit()
    
    input()
    process_lines()
    index_lines.sort(key=comparator)

    for line in index_lines:
        print(f"{line['index_line']}")
        
"""
HELPER FUNCTIONS
"""

"""
---------------------------------------------------------------------
input() takes lines from stdin and directs the to the apropriate list
---------------------------------------------------------------------
"""
def input() -> None:
    sys.stdin.readline()
    boundary_hit = False
    for line in sys.stdin:
        line = line.rstrip("\n")
        if(boundary_hit == False):
            if(line == '""""'):
                boundary_hit = True
            else:
                exclusion_words.append(line)
        else:
            if line.strip():
                lines_for_indexing.append(line)  

              
"""
-----------------------------------------------------------------------------------------------
process_lines() takes lines for indexing and process them, then allocates them to a dictionary.
The dictionary is then stored in a list
-----------------------------------------------------------------------------------------------
"""
def process_lines() -> None:
    for line in lines_for_indexing:
        curr = line.split()
        for word in curr:
            if(is_exclusion(word) == 0):
                temp = word
                temp = temp.upper()
                temp_line = line
                ind = temp_line.rfind(word)
                temp_line = temp_line[:ind] + temp + temp_line[ind+len(word):]
                index = temp_line.find(temp) 
                temp_line = format_line(index, temp_line, temp)
                index = {
                    "index_word": word.lower(),
                    "capped_index": temp,
                    "index_line": temp_line,
                }
                index_lines.append(index)


"""
----------------------------------------------------------------
format_line() takes an input line, index word, and its location, 
then formats the line to concord 2 standards and returns it.
----------------------------------------------------------------
"""
def format_line(index, line, index_word) -> str:
    temp = line.split(' ')
    j = 0
    fword = temp[j]

    if(index > 29):
        while(index > 29):
            index = line.find(index_word)
            line = line.replace(fword, '', 1)
            j = j+1
            fword = temp[j]
            index = line.find(index_word)

    index = 29 - index
    val = len(line) + index 
    line = line.rjust(val)
    findex = line.find(fword)
    
    if(findex < 9):
        line = line.replace(fword, '', 1)
        val = len(line) + len(fword)
        line = line.rjust(val)

    temp = line[8:61]
    temp_line = temp.split()

    if(temp[0] != ' '):
        fword = temp_line[0]
        temp_line.remove(fword)

    lword = temp_line[len(temp_line)-1]
    
    if(temp.rfind(lword) + len(lword) > 52):
        temp_line.remove(lword)
        

    temp_line = ' '.join(temp_line)
    index = temp_line.find(index_word)
    index = 29 - index
    val = len(temp_line) + index 
    temp_line = temp_line.rjust(val)


    return temp_line


"""
---------------------------------------------------
comparator() acts as the compairison key for sort()
---------------------------------------------------
"""
def comparator(i) -> str:
    return i['index_word']


"""
------------------------------------------------------------------------
is_exclusion() takes a word and determines if it is an index word or not
------------------------------------------------------------------------
"""
def is_exclusion(word) -> int:
    for excl in exclusion_words:
        if(word.lower() == excl.lower()):
            return 1

    return 0        

if __name__ == "__main__":
    main()
