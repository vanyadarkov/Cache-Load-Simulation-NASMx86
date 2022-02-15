#ifndef CHECKER_H
#define CHECKER_H

#define CACHE_TESTS         10


#define CACHE_LINES  100
#define CACHE_LINE_SIZE 8

#define MAX_SCORE           100.0

void load(char* reg, char** tags, char cache[CACHE_LINES][CACHE_LINE_SIZE], char* address, int to_replace);

#endif