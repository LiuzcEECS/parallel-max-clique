#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

#define DATA_NAME "data"
char first[100];
char second[100];
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Usage: gendata <vertex num> <edge num>\n");
    }
    int graphSize = atoi(argv[1]);
    strcpy(first, argv[1]);
    strcat(first, "_");
    strcat(first, argv[2]);
    int edgeNum = atoi(argv[2]);
    if (edgeNum * 2 + graphSize > graphSize * graphSize)
    {
        printf("Too many edges\n");
    }
    int* graph = calloc(graphSize * graphSize, sizeof(int));
    FILE* writeFile = fopen(first, "w");
    fprintf(writeFile, "%d\n", graphSize);
    srand(time(NULL));
    int i, j;
    for (i = 0; i < edgeNum; i++)
    {
        int a, b;
        a = rand() % graphSize;
        b = rand() % graphSize;
        if (graph[a * graphSize + b])
        {
            i--;
            continue;
        }
        if (a != b)
        {
            graph[a * graphSize + b] = 1;
            graph[b * graphSize + a] = 1;
        }
        else
        {
            i--;
        }
    }
    for (i = 0; i < graphSize; i++)
    {
        for (j = 0; j < graphSize; j++)
        {
            fprintf(writeFile, "%d ", graph[i * graphSize + j]);
        }
        fprintf(writeFile, "\n");
    }
    fclose(writeFile);
    free(graph);
    return 0;
}
