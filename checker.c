#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "checker.h"

double score;

void prepare_files(char *file_prefix, char infile_name[20], char outfile_name[20], char reffile_name[20], int i, int task4)
{
    int len = strlen(file_prefix);

    if (!task4)
        memset(infile_name, 0, 20);

    memset(outfile_name, 0, 20);
    memset(reffile_name, 0, 20);

    file_prefix[len - 1] = '0' + i;

    if (!task4)
    {
        strncpy(infile_name, "input/", 6);
        strncat(infile_name, file_prefix, len);
        strncat(infile_name, "_in\0", 4);
    }

    strncpy(outfile_name, "output/", 7);
    strncat(outfile_name, file_prefix, len);
    strncat(outfile_name, "_out\0", 5);

    strncpy(reffile_name, "ref/", 4);
    strncat(reffile_name, file_prefix, len);
    strncat(reffile_name, "_ref\0", 5);
}


void test_cache()
{
    FILE *infile, *outfile, *reffile;
    char file_prefix[7] = "cachex",
         infile_name[20],
         outfile_name[20],
         reffile_name[20];
    unsigned long i = 0, j = 0;
    char reg;
    unsigned long memory0_start = 0, memory1_start = 0;
    char memory0[16 * 16 + 8];
    char memory1[48 * 48 + 8];

    // Make sure the first valid address is always ending in 000 (divisible with 8).
    for (i = 0; i < 8; i++)
    {
        if (((unsigned long)&(memory0[i])) % 8 == 0) {
            memory0_start = i;
        }
        if (((unsigned long)&(memory1[i])) % 8 == 0)
        {
            memory1_start = i;
        }

    }

    // Read the memory content.
    infile = fopen("input/cacheA_in", "r");
    for (j = memory0_start; j < 16 * 16 + memory0_start; j++)
    {
        fscanf(infile, "%hhu", &(memory0[j]));
    }
    fclose(infile);
    infile = fopen("input/cacheB_in", "r");
    for (j = memory1_start; j < 48 * 48 + memory1_start; j++)
    {
        fscanf(infile, "%hhu", &(memory1[j]));
    }
    fclose(infile);

    // Initialize empty tags vector and create empty cache
    char **tags = malloc(CACHE_LINES * sizeof(char *));
    for (i = 0; i < CACHE_LINES; i++)
        tags[i] = 0;

    // Initialize empty cache
    char cache[CACHE_LINES][CACHE_LINE_SIZE];
    for (i = 0; i < CACHE_LINES; i++)
    {
        for (j = 0; j < CACHE_LINE_SIZE; j++)
        {
            cache[i][j] = 0;
        }
    }

    // Build the address we want to get data from. CHANGE x and y to move trough memory.
    int x[10] = {0, 2, 2, 0, 14, 47, 22, 10, 10, 47};
    int y[10] = {1, 4, 4, 1, 14, 3, 25, 0, 0, 47};
    int to_replace[10] = {7, 7, 17, 12, 13, 14, 2, 15, 10, 11};

    /*
     Testing. We are interested in checking the value brought in the register.
     Also, we need to check that the tag was modified and a line was brought
     from memory to cache in case of a CACHE MISS.
     */
    int pass;
    char *address;
    for (i = 0; i < 10; i++)
    {
        prepare_files(file_prefix, infile_name, outfile_name, reffile_name, i, 1);

        outfile = fopen(outfile_name, "w+");
        reffile = fopen(reffile_name, "r");

        int verif = 0;
        int check;
        pass = 0;
        if (i < 5)
            address = (char *)(memory0 + memory0_start + 16 * x[i] + y[i]);
        else
            address = (char *)(memory1 + memory1_start + 48 * x[i] + y[i]);
        load(&reg, tags, cache, address, to_replace[i]);

        fprintf(outfile, "%hhu\n", reg);
        fscanf(reffile, "%u", &check);
        if (check != reg)
            verif = 1;
        for (j = 0; j < CACHE_LINE_SIZE; j++)
        {
            fprintf(outfile, "%hhu", cache[to_replace[i]][j]);
            fscanf(reffile, "%u", &check);
            if (check != cache[to_replace[i]][j])
                verif = 1;
            if (j < CACHE_LINE_SIZE - 1)
                fprintf(outfile, " ");
        }
        fprintf(outfile, "\n");
        fclose(outfile);
        fclose(reffile);

        if (!verif)
        {
            printf("CACHE test %ld\t\t\t\t\tPASSED                 \n", i);
        }
        else
        {
            printf("CACHE test %ld\t\t\t\t\tFAILED                 \n", i);
        }
    }
}

int main(int argc, char *argv[])
{
    printf("---------------- Introduction to assembly language -----------------\n");
    printf("---------------------- Cache load simulation -----------------------\n");
    printf("--------------------------------------------------------------------\n");

    if (argc == 1 || !strncmp(argv[1], "4", 1))
    {
        test_cache();
        printf("--------------------------------------------------------------------\n");
    }

    return 0;
}
