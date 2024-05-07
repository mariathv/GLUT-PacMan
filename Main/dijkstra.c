#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

// Structure to represent a node in the adjacency list
struct AdjListNode {
    int dest;
    int weight;
    struct AdjListNode* next;
};

// Structure to represent an adjacency list
struct AdjList {
    struct AdjListNode* head;
};

// Structure to represent a graph
struct Graph {
    int V;
    struct AdjList* array;
};

// Function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, int weight) {
    struct AdjListNode* newNode = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// Function to create a graph with V vertices
struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->array = (struct AdjList*)malloc(V * sizeof(struct AdjList));
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
    return graph;
}

// Function to add an edge to the graph
void addEdge(struct Graph* graph, int src, int dest, int weight) {
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
}


void printGraph(struct Graph* graph) {
    int V = graph->V;
    for (int v = 0; v < V; ++v) {
        struct AdjListNode* current = graph->array[v].head;
        printf("Adjacency list of vertex %d\n", v);
        while (current) {
            printf("-> %d(weight %d) ", current->dest, current->weight);
            current = current->next;
        }
        printf("\n");
    }
}

void dijkstra(struct Graph* graph, int src) {
    int V = graph->V;
    int dist[V];
    bool sptSet[V];
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
    dist[src] = 0;
    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet, V);
        sptSet[u] = true;
        struct AdjListNode* current = graph->array[u].head;
        while (current != NULL) {
            int v = current->dest;
            if (!sptSet[v] && dist[u] != INT_MAX && dist[u] + current->weight < dist[v])
                dist[v] = dist[u] + current->weight;
            current = current->next;
        }
    }
    printSolution(dist, V);
}



int main() {
    

    return 0;
}