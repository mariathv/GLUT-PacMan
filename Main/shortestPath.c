#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>

int xCoords[] = {20, 555, 20, 125, 20, 254, 20, 125, 20, 555, 319, 555, 447, 555, 447, 555, 0, 189, 315, 555, 125, 447, 190, 253, 317, 380, 20, 61, 20, 252, 509, 555, 189, 384, 189, 384, 384, 560, 320, 384, 189, 256, 20, 20, 61, 61, 125, 125, 20, 20, 254, 254, 555, 555, 319, 319, 447, 447, 555, 555, 509, 509, 253, 253, 317, 317, 20, 20, 315, 315, 555, 555, 252, 252, 190, 190, 380, 380, 189, 189, 384, 384, 256, 256, 320, 320, 189, 189, 384, 384, 447, 125, 447, 125, 447, 254, 125, 447, 125};
int yCoords[] = {60, 60, 128, 128, 685, 685, 531, 531, 596, 596, 685, 685, 531, 531, 128, 128, 395, 395, 262, 262, 195, 195, 128, 128, 128, 128, 195, 195, 262, 262, 195, 195, 466, 466, 330, 330, 395, 395, 531, 531, 531, 531, 60, 128, 128, 195, 128, 685, 531, 685, 596, 685, 531, 685, 596, 685, 128, 685, 60, 128, 128, 195, 60, 128, 60, 128, 195, 262, 195, 262, 195, 262, 195, 262, 128, 195, 128, 195, 262, 466, 262, 466, 466, 531, 466, 531, 531, 596, 531, 596, 262, 330, 330, 395, 395, 466, 596, 596, 262};


int graphi[] = {0 ,0 ,1 ,1 ,2 ,2 ,3 ,3 ,4 ,4 ,5 ,5 ,6 ,6 ,7 ,7 ,7 ,8 ,8 ,8 ,9 ,9 ,9 ,10 ,10 ,11 ,11 ,12 ,12 ,12 ,13 ,13 ,14 ,14 ,15 ,15 ,17 ,17 ,17 ,18 ,18 ,19 ,19 ,20 ,20 ,20 ,21 ,21 ,21 ,98 ,98 ,98 ,98 ,23 ,23 ,24 ,24 ,25 ,25 ,26 ,26 ,27 ,27 ,28 ,28 ,30 ,30 ,31 ,31 ,32 ,32 ,33 ,33 ,34 ,34 ,34 ,35 ,35 ,35 ,36 ,36 ,36 ,38 ,38 ,39 ,39 ,40 ,40 ,41 ,41 ,44 ,44 ,44 ,47 ,47 ,47 ,62 ,62 ,62 ,64 ,64 ,64 ,60 ,60 ,60 ,75 ,75 ,75 ,72 ,72 ,72 ,68 ,68 ,68 ,77 ,77 ,77 ,78 ,78 ,78 ,80 ,80 ,80 ,90 ,90 ,90 ,90 ,91 ,91 ,92 ,92 ,93 ,93 ,93 ,94 ,94 ,94 ,95 ,95 ,95 ,84 ,84 ,84 ,96 ,96 ,96 ,96 ,87 ,87 ,87 ,50 ,50 ,50 ,54 ,54 ,54 ,89 ,89 ,89 ,97 ,97 ,97 ,97 ,57 ,57 ,57,29,29};
int graphj[] = {2 ,62 ,15 ,64 ,0 ,44 ,44 ,20 ,8 ,47 ,47 ,50 ,8 ,7 ,96 ,6 ,93 ,6 ,96 ,4 ,13 ,11 ,97 ,54 ,57 ,9 ,57 ,94 ,13 ,97 ,12 ,9 ,60 ,21 ,1 ,60 ,93 ,34 ,32 ,68 ,80 ,31 ,90 ,3 ,75 ,98 ,14 ,77 ,90 ,20 ,28 ,78 ,91 ,22 ,62 ,64 ,25 ,24 ,77 ,28 ,27 ,26 ,44 ,26 ,98 ,60 ,31 ,30 ,19 ,17 ,95 ,36 ,84 ,78 ,17 ,35 ,80 ,34 ,36 ,35 ,94 ,33 ,84 ,39 ,38 ,89 ,87 ,41 ,40 ,95 ,2 ,3 ,27 ,4 ,96 ,5 ,0 ,23 ,64 ,62 ,24 ,1 ,14 ,30 ,15 ,20 ,21 ,72 ,75 ,29 ,68 ,72 ,18 ,77 ,68 ,25 ,21 ,98 ,34 ,29 ,18 ,35 ,90 ,80 ,21 ,19 ,92 ,98 ,93 ,90 ,94 ,91 ,17 ,7 ,92 ,36 ,12 ,32 ,41 ,84 ,95 ,38 ,33 ,8 ,7 ,87 ,47 ,96 ,40 ,50 ,87 ,54 ,5 ,50 ,10 ,89 ,54 ,39 ,97 ,89 ,12 ,9 ,57 ,10 ,97 ,11,72,78};

int **graph2D;
// Structure to represent a point in the maze

int size;
struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode *next;
};

// Structure to represent an adjacency list
struct AdjList
{
    struct AdjListNode *head;
};

// Structure to represent a graph
struct Graph
{
    int V;
    struct AdjList *array;
};

// Function to create a new adjacency list node
struct AdjListNode *newAdjListNode(int dest, int weight)
{
    struct AdjListNode *newNode = (struct AdjListNode *)malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// Function to create a graph with V vertices
struct Graph *createGraph(int V)
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->array = (struct AdjList *)malloc(V * sizeof(struct AdjList));
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
    return graph;
}

// Function to add an edge to the graph
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

void dijkstra(struct Graph *graph, int start, int end)
{
    int V = graph->V;
    int dist[V];
    bool sptSet[V];
    int parent[V]; // Array to store the shortest path tree
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
    printSolution(dist, V, start, end);

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
}

void createGrapha()
{
    size = 99;
    int arrSize = sizeof(graphi) / sizeof(graphi[0]);

    struct Graph *graph = createGraph(size);

    for (int i = 0; i < arrSize; ++i)
    {
        printf("x2  = %d     x1  = %d\n" , xCoords[graphj[i]],xCoords[graphi[i]]);
        printf("y2  = %d     y1  = %d\n" , yCoords[graphj[i]],yCoords[graphi[i]]);
        int x = xCoords[graphj[i]] - xCoords[graphi[i]];
        int y = yCoords[graphj[i]] - yCoords[graphi[i]];
        int ans = sqrt((x * x) + (y * y));
        printf("from %d   to  %d  ----     %d\n\n\n" , graphi[i],graphj[i] , ans);
        addEdge(graph, graphi[i], graphj[i], ans);
        // graph2D[graphi[i]][graphj[i]] = ans;
    }
    printGraph(graph);
    dijkstra(graph, 29 , 3);
    // printGraph(graph);
}

int main()
{
    // Initialize graph nodes
    createGrapha();

    return 0;
}