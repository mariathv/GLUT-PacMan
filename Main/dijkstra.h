#include <GL/gl.h>
#include <GL/glx.h>

#include <GL/glut.h>
#include <SOIL/SOIL.h>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <semaphore.h>

struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode *next;
};

struct AdjList
{
    struct AdjListNode *head;
};

struct Graph
{
    int V;
    struct AdjList *array;
};

struct AdjListNode *newAdjListNode(int dest, int weight)
{
    struct AdjListNode *newNode = (struct AdjListNode *)malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

struct Graph *createGraph(int V)
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->array = (struct AdjList *)malloc(V * sizeof(struct AdjList));
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
    return graph;
}



void addEdge(struct Graph *graph, int src, int dest, int weight)
{
    struct AdjListNode *newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
}

void printGraph(struct Graph *graph)
{
    int V = graph->V;
    for (int v = 0; v < V; ++v)
    {
        struct AdjListNode *current = graph->array[v].head;
        printf("Adjacency list of vertex %d\n", v);
        while (current)
        {
            printf("-> %d(weight %d) ", current->dest, current->weight);
            current = current->next;
        }
        printf("\n");
    }
}

int minDistance(int dist[], bool sptSet[], int V)
{
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}

// Function to print the constructed distance array
void printSolution(int dist[], int V, int start, int end)
{
    printf("Shortest distance from vertex %d to vertex %d is: %d\n", start, end, dist[end]);
}

int* dijkstra(struct Graph *graph, int start, int end)
{
    int V = graph->V;
    int dist[V];
    bool sptSet[V];
    int *parent = malloc(60 * sizeof(int));
    printf("V size = %d ", V);
    // Array to store the shortest path tree
    for (int i = 0; i < V; i++)
    {
        dist[i] = INT_MAX;
        sptSet[i] = false;
        parent[i] = -1;
    }
    dist[start] = 0;
    for (int count = 0; count < V - 1; count++)
    {
        int u = minDistance(dist, sptSet, V);
        sptSet[u] = true;
        struct AdjListNode *current = graph->array[u].head;
        while (current != NULL)
        {
            int v = current->dest;
            if (!sptSet[v] && dist[u] != INT_MAX && dist[u] + current->weight < dist[v])
            {
                dist[v] = dist[u] + current->weight;
                parent[v] = u;
            }
            current = current->next;
        }
    }
    // printSolution(dist, V, start, end);
    // printf("End %d \n", end);
    //  Print the shortest path if it exists
    if (dist[end] != INT_MAX)
    {
        printf("Shortest path from vertex %d to vertex %d: ", start, end);
        int current = end;
        while (current != -1)
        {
            printf("%d ", current);
            current = parent[current];
        }
        printf("\n");
    }
    else
    {
        printf("No path from vertex %d to vertex %d\n", start, end);
    }
    return parent;
}

int* dijkstra2(struct Graph *graph, int start, int end)
{
    int V = graph->V;
    int dist[V];
    bool sptSet[V];
    int *parent = malloc(V * sizeof(int));

    // Initialize distances, shortest path set, and parent array
    for (int i = 0; i < V; i++)
    {
        dist[i] = INT_MAX;
        sptSet[i] = false;
        parent[i] = -1;
    }

    // Distance from start to itself is 0
    dist[start] = 0;

    // Calculate shortest path tree
    for (int count = 0; count < V - 1; count++)
    {
        int u = minDistance(dist, sptSet, V);
        sptSet[u] = true;
        struct AdjListNode *current = graph->array[u].head;
        while (current != NULL)
        {
            int v = current->dest;
            int weight = current->weight;

            // Modify weight if power-up is active

            weight = -weight;

            // Update distance if a shorter path is found
            if (!sptSet[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
            current = current->next;
        }
    }

    // Print the shortest path if it exists
    if (dist[end] != INT_MAX)
    {
        printf("Shortest path from vertex %d to vertex %d: ", start, end);
        int current = end;
        while (current != -1)
        {
            printf("%d ", current);
            current = parent[current];
        }
        printf("\n");
    }
    else
    {
        printf("No path from vertex %d to vertex %d\n", start, end);
    }

    return parent;
}
