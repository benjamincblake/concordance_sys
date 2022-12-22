#!/usr/bin/env python3
import sys
import fileinput
from itertools import takewhile
import re

class concord:
    '''
    ---------------------------------------------------------------
    A concordance system that takes formatted input from a file or
    stdin and returns/outputs a concordance formatted 
    list to its respective destination (output file/stdout)
    Attributes:
        input - input file (if None; stdin)
        output - output file (if None; stdout)
        input_lines - input lines from input file
        to_file() - method called on init to output to file
    ---------------------------------------------------------------
    '''

    def __init__(self, input=None, output=None):
        self.input = input
        self.output = output
        self.input_lines = self.__get_input()
        self.__to_file()


    def full_concordance(self) -> list:
        '''
        ---------------------------------------------------------------
        full_concordance(self):
        Input: concord object
        Output: list of formatted concordance lines
        ---------------------------------------------------------------
        '''
        if(not(self.input_lines)):
            return self.input_lines

        exclusion_words = list(takewhile(lambda l: l != '""""', self.input_lines))
        exclusion_words = [word.upper() for word in exclusion_words]
        exclusion_words = ' ' + ' '.join(exclusion_words) + ' '

        lines_for_indexing = [line for line in self.input_lines if line not in exclusion_words]
        lines_for_indexing.remove('""""')
        
        index_lines = self.__process_lines(lines_for_indexing, exclusion_words)
        index_lines.sort(key=self.__comparator)
        sorted_lines = [line['index_line'] for line in index_lines]

        return sorted_lines
    
    def __get_input(self) -> list:
        '''
        ---------------------------------------------------------------
        __get_input(self): 
        Input: concord object
        Output: list of input lines 
        ---------------------------------------------------------------
        '''
        input_lines = []
        if(self.input == None):
            sys.stdin.readline()
            sys.stdin.readline()
            input_lines = [line.rstrip() for line in sys.stdin]
        else:
            input_lines = [line.rstrip() for line in fileinput.input(files=(self.input)) if not (fileinput.isfirstline() or fileinput.lineno() == 2)]
            fileinput.close()

        return input_lines

    
    def __to_file(self) -> None:
        '''
        ---------------------------------------------------------------
        __to_file(self) 
        Input: concord object
        Output: None, if output file is specified, writes to file. 
        ---------------------------------------------------------------
        '''
        if(self.input != None):
            input_lines = self.full_concordance()
            with open(self.output, 'w') as f:
                f.write('\n'.join(input_lines))
                f.write('\n')
            f.close()


    def __process_lines(self, lines_for_indexing, exclusion_words) -> list:
        '''
        ---------------------------------------------------------------
        __process_lines(self)
        Input: concord object,lists: lines_for_indexing, excl words
        Output: fully formatted index lines in dictionaries. 
        ---------------------------------------------------------------
        '''
        index_lines = []
        for line in lines_for_indexing:
            curr = line.split()
            for word in curr:
                if(re.search("\s"+word.upper()+"\s", exclusion_words) == None):
                    temp = word
                    temp = temp.upper()
                    temp_line = line
                    temp_line = re.sub("(?:^|\W)"+word+"(?:$|\W)", " "+temp+" ", temp_line)
                    index = temp_line.find(temp) 
                    temp_line = self.__format_line(index, temp_line, temp)
                    index = {
                        "index_word": word.lower(),
                        "capped_index": temp,
                        "index_line": temp_line,
                    }
                    index_lines.append(index)
        
        return index_lines

    def __format_line(self, index, line, index_word) -> str:
        '''
        ---------------------------------------------------------------
        __format_line(self) 
        Input: concord object, int index, str: line, index word
        Output: str line formatted to concord2 specifications 
        ---------------------------------------------------------------
        '''
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

    def __comparator(self, i) -> str:
        '''
        ---------------------------------------------------------------
        __comparator(self) 
        Input: concord object, dict i
        Output: index word field from dict, used in sort()
        ---------------------------------------------------------------
        '''
        return i['index_word']