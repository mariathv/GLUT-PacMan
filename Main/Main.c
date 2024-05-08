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
// YAAANNNN
//  Global variables for texture, position, and size
// using my code here Hussnain
bool stoppac;
GLuint pacmanRight;
GLuint pacmanLeft;
GLuint pacmanDown;
GLuint pacmanUp;
GLuint backgroundTextureID;
GLuint foodTextureID;
GLuint ghostTextureID[2];

float x = 280.0f;
float y = 195.0f;
float side = 30.0f;

float ghostX[2] = {
    20,
    270};
float ghostY[2] = {68, 466};
int numGhost = 2;
char ghostMovement[2][10];
char ghostPrevMovement[2][10];
bool isWallTurn[2];

float foody = 60.0f;
float foodx = 20.0f; // Adjusted the size for clarity
float foodside = 30.0f;

int arrFoody[] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 531, 531, 531, 531, 531, 531, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 531, 531, 531, 531, 531, 531, 531, 395, 395, 395, 395, 395, 395, 395, 395, 395, 395, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 195, 195, 195, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 395, 395, 395, 395, 395, 395, 395, 395, 395, 395, 531, 531, 531, 531, 531, 531, 531, 531, 85, 107, 150, 170, 150, 173, 215, 240, 290, 310, 330, 350, 370, 420, 440, 460, 480, 505, 550, 570, 620, 640, 660, 150, 173, 215, 240, 290, 310, 330, 350, 370, 420, 440, 460, 480, 505, 550, 570, 620, 640, 660, 550, 570, 620, 640, 660, 555, 575, 620, 640, 660, 620, 640, 660, 85, 105, 150, 170, 80, 100, 80, 100, 220, 240, 220, 240, 220, 240, 150, 170, 150, 170, 285, 305, 350, 370, 420, 440, 285, 305, 350, 370, 420, 440, 490, 510, 490, 510, 550, 570, 550, 570, 620, 640, 660, 220, 240};
int arrFoodx[] = {20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 237, 253, 273, 296, 317, 340, 360, 380, 400, 420, 440, 460, 480, 500, 520, 539, 555, 20, 40, 60, 80, 103, 125, 447, 463, 489, 509, 523, 539, 555, 190, 210, 230, 253, 317, 340, 360, 380, 20, 40, 60, 80, 100, 125, 140, 160, 180, 200, 220, 238, 254, 319, 340, 360, 380, 400, 420, 440, 460, 480, 500, 520, 539, 555, 20, 40, 60, 80, 103, 125, 20, 40, 60, 80, 100, 125, 140, 160, 180, 200, 220, 240, 260, 280, 300, 320, 340, 360, 380, 400, 423, 447, 463, 480, 500, 520, 539, 555, 447, 463, 482, 501, 520, 539, 555, 0, 20, 40, 60, 80, 100, 125, 143, 165, 189, 315, 338, 360, 380, 400, 420, 447, 463, 480, 501, 520, 539, 555, 125, 142, 160, 180, 200, 220, 240, 320, 340, 360, 380, 403, 425, 447, 20, 40, 61, 20, 40, 60, 80, 100, 125, 140, 160, 177, 193, 210, 230, 252, 509, 531, 555, 189, 204, 221, 240, 260, 280, 300, 320, 340, 362, 384, 189, 204, 221, 240, 260, 280, 300, 320, 340, 362, 384, 384, 400, 420, 447, 463, 480, 500, 520, 540, 560, 320, 340, 360, 384, 189, 213, 235, 256, 20, 20, 61, 61, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 20, 20, 254, 254, 254, 555, 555, 555, 555, 555, 319, 319, 319, 555, 555, 509, 509, 253, 253, 317, 317, 20, 20, 315, 315, 555, 555, 190, 190, 380, 380, 189, 189, 189, 189, 189, 189, 384, 384, 384, 384, 384, 384, 256, 256, 320, 320, 189, 189, 384, 384, 20, 20, 20, 252, 252};

int xCoords[] = {20, 555, 20, 125, 20, 254, 20, 125, 20, 555, 319, 555, 447, 555, 447, 555, 0, 189, 315, 555, 125, 447, 190, 253, 317, 380, 20, 61, 20, 252, 509, 555, 189, 384, 189, 384, 384, 560, 320, 384, 189, 256, 20, 20, 61, 61, 125, 125, 20, 20, 254, 254, 555, 555, 319, 319, 447, 447, 555, 555, 509, 509, 253, 253, 317, 317, 20, 20, 315, 315, 555, 555, 252, 252, 190, 190, 380, 380, 189, 189, 384, 384, 256, 256, 320, 320, 189, 189, 384, 384, 447, 125, 447, 125, 447, 254, 125, 447, 125};
int yCoords[] = {60, 60, 128, 128, 685, 685, 531, 531, 596, 596, 685, 685, 531, 531, 128, 128, 395, 395, 262, 262, 195, 195, 128, 128, 128, 128, 195, 195, 262, 262, 195, 195, 466, 466, 330, 330, 395, 395, 531, 531, 531, 531, 60, 128, 128, 195, 128, 685, 531, 685, 596, 685, 531, 685, 596, 685, 128, 685, 60, 128, 128, 195, 60, 128, 60, 128, 195, 262, 195, 262, 195, 262, 195, 262, 128, 195, 128, 195, 262, 466, 262, 466, 466, 531, 466, 531, 531, 596, 531, 596, 262, 330, 330, 395, 395, 466, 596, 596, 262};

int graphi[] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 12, 12, 12, 13, 13, 14, 14, 15, 15, 17, 17, 17, 18, 18, 19, 19, 20, 20, 20, 21, 21, 21, 99, 99, 99, 99, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 34, 35, 35, 35, 36, 36, 36, 38, 38, 39, 39, 40, 40, 41, 41, 44, 44, 44, 47, 47, 47, 63, 63, 63, 65, 65, 65, 61, 61, 61, 76, 76, 76, 73, 73, 73, 69, 69, 69, 78, 78, 78, 79, 79, 79, 81, 81, 81, 91, 91, 91, 91, 92, 92, 93, 93, 94, 94, 94, 95, 95, 95, 96, 96, 96, 85, 85, 85, 97, 97, 97, 97, 88, 88, 88, 50, 50, 50, 54, 54, 54, 90, 90, 90, 98, 98, 98, 98, 57, 57, 57};
int graphj[] = {2, 63, 15, 65, 0, 44, 44, 20, 8, 47, 47, 50, 8, 7, 97, 6, 94, 6, 97, 4, 13, 11, 98, 54, 57, 9, 57, 95, 13, 98, 12, 9, 61, 21, 1, 61, 94, 34, 32, 69, 81, 31, 91, 3, 76, 99, 14, 78, 91, 20, 28, 79, 92, 22, 63, 65, 25, 24, 78, 28, 27, 26, 44, 26, 99, 61, 31, 30, 19, 17, 96, 36, 85, 79, 17, 35, 81, 34, 36, 35, 95, 33, 85, 39, 38, 90, 88, 41, 40, 96, 2, 3, 27, 4, 97, 5, 0, 23, 65, 63, 24, 1, 14, 30, 15, 20, 21, 73, 76, 29, 69, 73, 18, 78, 69, 25, 21, 99, 34, 29, 18, 35, 91, 81, 21, 19, 93, 99, 94, 91, 95, 92, 17, 7, 93, 36, 12, 32, 41, 85, 96, 38, 33, 8, 7, 88, 47, 97, 40, 50, 88, 54, 5, 50, 10, 90, 54, 39, 98, 90, 12, 9, 57, 10, 98, 11};

int xMidCoords[] = {447, 125, 447, 125, 447, 254, 125, 447, 125, 555};
int yMidCoords[] = {262, 330, 330, 395, 395, 466, 596, 596, 262, 596};

int xDownMidCoords[] = {555};
int yDownMidCoords[] = {596};
int coordsXYSize = sizeof(yCoords) / sizeof(yCoords[0]);
const int foodXYSize = sizeof(arrFoodx) / sizeof(arrFoodx[0]);

bool *checkFoodEatArr;

void *gameEngineThread(void *arg);
void *userInterfaceThread(void *arg);
void *ghostThread(void *arg);

char triedKeyPressed[10];
int delayTimer = 0;
char keypressed[10];
char delayKey[10];
char prevkeypressed[10];

pthread_mutex_t lock;

bool delayFlag = false;

int size;
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

struct Graph *graph;

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

    graph = createGraph(size);

    for (int i = 0; i < arrSize; ++i)
    {
        int x = xCoords[graphj[i]] - yCoords[graphi[i]];
        int y = xCoords[graphj[i]] - yCoords[graphi[i]];
        int ans = sqrt((x * x) + (y * y));

        addEdge(graph, graphi[i], graphj[i], ans);
    }
}

// Function to load texture from file
void loadTexture(const char *filename, GLuint *textureID)
{
    // Load image texture using SOIL
    *textureID = SOIL_load_OGL_texture(
        filename,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

    if (*textureID == 0)
    {
        printf("Failed to load image: %s %s\n", SOIL_last_result(), filename);
        exit(1);
    }
}

// Function to display the scene
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Draw the background map
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, backgroundTextureID);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(0, 0);
    glTexCoord2f(1, 0);
    glVertex2f(600, 0);
    glTexCoord2f(1, 1);
    glVertex2f(600, 800);
    glTexCoord2f(0, 1);
    glVertex2f(0, 800);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    for (int i = 0; i < foodXYSize; ++i)
    {
        if (checkFoodEatArr[i])
            continue;
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, foodTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2f(arrFoodx[i], arrFoody[i]);
        glTexCoord2f(1, 0);
        glVertex2f(arrFoodx[i] + foodside, arrFoody[i]);
        glTexCoord2f(1, 1);
        glVertex2f(arrFoodx[i] + foodside, arrFoody[i] + (foodside * 1.0f));
        glTexCoord2f(0, 1);
        glVertex2f(arrFoodx[i], arrFoody[i] + (foodside * 1.0f));
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }

    // Draw Pacman
    glEnable(GL_TEXTURE_2D);
    if (strcmp(keypressed, "left") == 0)
        glBindTexture(GL_TEXTURE_2D, pacmanLeft);
    else if (strcmp(keypressed, "up") == 0)
        glBindTexture(GL_TEXTURE_2D, pacmanDown);
    else if (strcmp(keypressed, "down") == 0)
        glBindTexture(GL_TEXTURE_2D, pacmanUp);
    else
        glBindTexture(GL_TEXTURE_2D, pacmanRight);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + side, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + side, y + (side * 1.0f)); // Adjusted the height of the quad
    glTexCoord2f(0, 1);
    glVertex2f(x, y + (side * 1.0f)); // Adjusted the height of the quad
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // Draw Ghost PINKY
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ghostTextureID[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(1, 1);
    glVertex2f(ghostX[0], ghostY[0]);
    glTexCoord2f(0, 1);
    glVertex2f(ghostX[0] + side, ghostY[0]);
    glTexCoord2f(0, 0);
    glVertex2f(ghostX[0] + side, ghostY[0] + (side * 1.0f)); // Adjusted the height of the quad
    glTexCoord2f(1, 0);
    glVertex2f(ghostX[0], ghostY[0] + (side * 1.0f)); // Adjusted the height of the quad
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // DRAW GHOST CLYDE
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ghostTextureID[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(1, 1);
    glVertex2f(ghostX[1], ghostY[1]);
    glTexCoord2f(0, 1);
    glVertex2f(ghostX[1] + side, ghostY[1]);
    glTexCoord2f(0, 0);
    glVertex2f(ghostX[1] + side, ghostY[1] + (side * 1.0f)); // Adjusted the height of the quad
    glTexCoord2f(1, 0);
    glVertex2f(ghostX[1], ghostY[1] + (side * 1.0f)); // Adjusted the height of the quad
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

void printPosition()
{
    printf("position: (%.2f, %.2f)\n", x, y);
}

void checkcornerpoint()
{
    for (int i = 0; i < coordsXYSize; ++i)
    {
        if (xCoords[i] == x && yCoords[i] == y)
        {
            printf("corner!\n");
            break;
        }
    }
}

void checkfoodEat()
{
    for (int i = 0; i < foodXYSize; ++i)
    {
        if (arrFoodx[i] == x && arrFoody[i] == y && checkFoodEatArr[i] == false)
        {
            checkFoodEatArr[i] = true;
        }
    }
}

void checkTeleport()
{
    if (x == 0 && y == 395)
    {
        x = 560;
        // face turn also
    }
    else if (x == 560 && y == 395)
    {
        x = 0;
        // face turn alsooo
    }
}

bool isWallCollide(bool moveAxis, float xx, float yy) // 0 for x axis movement
{
    bool errFlag = true;
    if (moveAxis == 0) // Horizontal
    {                  // Y values
        int xPath[] = {60, 128, 685, 531, 596, 685, 531, 128, 395, 262, 195, 128, 128, 195, 262, 195, 466, 330, 395, 531, 531};
        int toFrom[] = {20, 555, 20, 125, 20, 254, 20, 125, 20, 555, 319, 555, 447, 555, 447, 555, 0, 189, 315, 555, 125, 447, 190, 253, 317, 380, 20, 61, 20, 252, 509, 555, 189, 384, 189, 384, 384, 560, 320, 384, 189, 256};
        int numElements = sizeof(xPath) / sizeof(xPath[0]);
        int numElementsToFrom = sizeof(toFrom) / sizeof(toFrom[0]);
        for (int i = 0; i < numElements; i++)
            if (yy == xPath[i])
            {

                int j;
                if (i == 0)
                {
                    j = 0;
                }
                else
                {
                    j = i * 2;
                }

                if (xx >= toFrom[j] && xx <= toFrom[j + 1])
                {
                    return false;
                }
            }
        if (errFlag)
        {
            return true;
        }
    }
    else // Vertical
    {    // X Values
        int YPath[] = {20, 61, 125, 20, 254, 555, 319, 447, 555, 509, 253, 317, 20, 315, 555, 252, 190, 380, 189, 384, 256, 320, 189, 384};
        int toFrom[] = {60, 128, 128, 195, 128, 685, 531, 685, 596, 685, 531, 685, 596, 685, 128, 685, 60, 128, 128, 195, 60, 128, 60, 128, 195, 262, 195, 262, 195, 262, 195, 262, 128, 195, 128, 195, 262, 466, 262, 466, 466, 531, 466, 531, 531, 596, 531, 596};
        int numElements = sizeof(YPath) / sizeof(YPath[0]);
        int numElementsToFrom = sizeof(toFrom) / sizeof(toFrom[0]);
        for (int i = 0; i < numElements; i++)
            if (xx == YPath[i])
            {
                int j;
                if (i == 0)
                {
                    j = 0;
                }
                else
                {
                    j = i * 2;
                }

                if (yy >= toFrom[j] && yy <= toFrom[j + 1])
                {
                    return false;
                }
            }
        if (errFlag)
        {
            return true;
        }
    }
}

void keyboard(int key) // Primary key board function to handle user inputs
{
    float newX = x;
    float newY = y;
    switch (key)
    {
    case GLUT_KEY_RIGHT:
        newX += 0.5;
        if (isWallCollide(0, newX, newY) == false)
        {
            // pacmanTexturePath = "imgs/pacman/right.png";
            strcpy(keypressed, "right");
        }
        else
        {
            strcpy(triedKeyPressed, "right");
            delayFlag = true;
            delayTimer = 100;
        }
        break;
    case GLUT_KEY_LEFT:
        newX -= 0.5;
        if (isWallCollide(0, newX, newY) == false)
        {
            strcpy(keypressed, "left");
        }
        else
        {
            strcpy(triedKeyPressed, "left");
            delayFlag = true;
            delayTimer = 100;
        }
        break;
    case GLUT_KEY_UP:
        newY -= 0.5;
        if (isWallCollide(1, newX, newY) == false)
        {
            strcpy(keypressed, "up");
        }
        else
        {
            strcpy(triedKeyPressed, "up");
            delayFlag = true;
            delayTimer = 100;
        }
        break;
    case GLUT_KEY_DOWN:
        newY += 0.5;
        if (isWallCollide(1, newX, newY) == false)
        {
            strcpy(keypressed, "down");
        }
        else
        {
            strcpy(triedKeyPressed, "down");
            delayFlag = true;
            delayTimer = 100;
        }
        break;
    }
}

void movePacman(const char *direction) // A secondary Pacman move check function to check delay movement
{
    float newX = x;
    float newY = y;

    if (strcmp(direction, "right") == 0)
    {
        newX += 0.5;
        if (isWallCollide(0, newX, newY) == false)
        {
            strcpy(keypressed, "right");
        }
    }
    else if (strcmp(direction, "left") == 0)
    {
        newX -= 0.5;
        if (isWallCollide(0, newX, newY) == false)
        {
            strcpy(keypressed, "left");
        }
    }
    else if (strcmp(direction, "up") == 0)
    {
        newY -= 0.5;
        if (isWallCollide(1, newX, newY) == false)
        {
            strcpy(keypressed, "up");
        }
    }
    else if (strcmp(direction, "down") == 0)
    {
        newY += 0.5;
        if (isWallCollide(1, newX, newY) == false)
        {
            strcpy(keypressed, "down");
        }
    }
}

// Function to initialize OpenGL settings
void initOpenGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 600, 800, 0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    loadTexture("imgs/pacman/right.png", &pacmanRight);
    loadTexture("imgs/pacman/left.png", &pacmanLeft);
    loadTexture("imgs/pacman/up.png", &pacmanUp);
    loadTexture("imgs/pacman/down.png", &pacmanDown);
    loadTexture("imgs/map/map.png", &backgroundTextureID);
    loadTexture("imgs/food/dot.png", &foodTextureID);
    loadTexture("imgs/ghosts/pinky.png", &ghostTextureID[0]);
    loadTexture("imgs/ghosts/clyde.png", &ghostTextureID[1]);
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(700, 900);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Pacman Game");
    initOpenGL();

    glutDisplayFunc(display);

    pthread_mutex_init(&lock, NULL);

    int ghostNumber1 = 0;
    int ghostNumber2 = 1;
    pthread_t EngineThread, playerThread, Ghost1, Ghost2, Ghost3, Ghost4;
    pthread_create(&EngineThread, NULL, gameEngineThread, NULL);
    pthread_create(&playerThread, NULL, userInterfaceThread, NULL);
    pthread_create(&Ghost1, NULL, ghostThread, (void *)&ghostNumber1);
    pthread_create(&Ghost2, NULL, ghostThread, (void *)&ghostNumber2);

    glutMainLoop();

    return 0;
}

void checkGhostCoords(int ghostNum)
{
    srand(time(0));
    // int chance = 1;

    int chance = rand() % 2;
    if (chance == 1)
    {
        // if (strcmp(ghostMovement[ghostNum], "down") == 0 || (strcmp(ghostMovement[ghostNum], "up") == 0))
        // {
        //     for (int i = 0; i < (sizeof(xDownMidCoords) / sizeof(xDownMidCoords[0])); i++)
        //     {
        //         if (ghostX[ghostNum] == xDownMidCoords[i] && ghostY[ghostNum] == yDownMidCoords[i])
        //         {
        //             printf("NEW checking if can move left or right\n");
        //             float newX = ghostX[ghostNum];
        //             float newY = ghostY[ghostNum];
        //             newX = ghostX[ghostNum] + 1;
        //             newY = ghostY[ghostNum];
        //             if (isWallCollide(0, newX, newY) == false)
        //             {
        //                 printf("to right\n");
        //                 ghostX[ghostNum] += 1;
        //                 strcpy(ghostMovement[ghostNum], "right");
        //                 return;
        //             }
        //             newX = ghostX[ghostNum] - 1;
        //             newY = ghostY[ghostNum];
        //             if (isWallCollide(0, newX, newY) == false)
        //             {
        //                 printf("to left\n");
        //                 ghostX[ghostNum] -= 1;
        //                 strcpy(ghostMovement[ghostNum], "left");
        //                 return;
        //             }
        //         }
        //     }
        // }
        int totalVertices = sizeof(xCoords) / sizeof(xCoords[0]);
        for (int i = 0; i < 8; i++)
        {
            if (ghostX[ghostNum] == xMidCoords[i] && ghostY[ghostNum] == yMidCoords[i])
            {
                // int rand=

                if ((strcmp(ghostMovement[ghostNum], "left") == 0) || (strcmp(ghostMovement[ghostNum], "right") == 0))
                {

                    float newX = ghostX[ghostNum];
                    float newY = ghostY[ghostNum];
                    newX = ghostX[ghostNum];
                    newY = ghostY[ghostNum] + 1;
                    if (isWallCollide(1, newX, newY) == false)
                    {
                        //  printf("to down\n");
                        ghostY[ghostNum] += 1;
                        strcpy(ghostMovement[ghostNum], "down");
                        return;
                    }
                    newX = ghostX[ghostNum];
                    newY = ghostY[ghostNum] - 1;
                    if (isWallCollide(1, newX, newY) == false)
                    {
                        // printf("to up\n");
                        ghostY[ghostNum] -= 1;
                        strcpy(ghostMovement[ghostNum], "up");
                        return;
                    }
                }
                else if ((strcmp(ghostMovement[ghostNum], "up") == 0) || (strcmp(ghostMovement[ghostNum], "down") == 0))
                {
                    // printf("checking if can move left or right\n");
                    float newX = ghostX[ghostNum];
                    float newY = ghostY[ghostNum];
                    newX = ghostX[ghostNum] + 1;
                    newY = ghostY[ghostNum];
                    if (isWallCollide(0, newX, newY) == false)
                    {
                        // printf("to right\n");
                        ghostX[ghostNum] += 1;
                        strcpy(ghostMovement[ghostNum], "right");
                        return;
                    }
                    newX = ghostX[ghostNum] - 1;
                    newY = ghostY[ghostNum];
                    if (isWallCollide(0, newX, newY) == false)
                    {
                        // printf("to left\n");
                        ghostX[ghostNum] -= 1;
                        strcpy(ghostMovement[ghostNum], "left");
                        return;
                    }
                }
            }
        }
    }
}

void *gameEngineThread(void *arg)
{
    checkFoodEatArr = malloc(foodXYSize * sizeof(bool));
    while (1)
    {
        float newX = x;
        float newY = y;
        if (delayFlag)
        {
            movePacman(triedKeyPressed);

            delayTimer -= 1;
            if (delayTimer < 0)
            {
                delayTimer = 0;
                strcpy(triedKeyPressed, "");
                delayFlag = false;
            }
        }

        if (strcmp(keypressed, "down") == 0)
        {
            newY += 0.5;
            if (isWallCollide(1, newX, newY) == false)
            {
                y += 0.5;
                printPosition();
                checkcornerpoint();
                printf("\n");
            }
        }
        else if (strcmp(keypressed, "up") == 0)
        {
            newY -= 0.5;
            if (isWallCollide(1, newX, newY) == false)
            {
                y -= 0.5;
                printPosition();
                checkcornerpoint();
                printf("\n");
            }
        }
        else if (strcmp(keypressed, "left") == 0)
        {
            newX -= 0.5;
            if (isWallCollide(0, newX, newY) == false)
            {
                x -= 0.5;
                printPosition();
                checkcornerpoint();
                printf("\n");
            }
        }
        else if (strcmp(keypressed, "right") == 0)
        {
            newX += 0.5;
            if (isWallCollide(0, newX, newY) == false)
            {
                x += 0.5;
                printPosition();
                checkcornerpoint();
                printf("\n");
            }
        }

        // printPosition();

        checkTeleport();
        checkfoodEat();
        glutPostRedisplay(); // Request redisplay
        usleep(5000);
    }
    return NULL;
}

void *userInterfaceThread(void *arg)
{
    // Initialize user interface
    glutSpecialFunc(keyboard);
    while (1)
    {
    }
    return NULL;
}

void changeGhostMovement(int ghostNum, int currAxis)
{
    srand(time(0));
    if (currAxis == 0)
    {                                // horizontal
        int randomMove = rand() % 2; // 0 is UP, 1 is DOWN, 2 is LEFT, 3 is RIGHT
        // printf("%d\n", randomMove);
        if (randomMove == 0)
        {
            printf("ghost %d trying to move down\n", ghostNum);
            // if (strcmp(ghostPrevMovement[ghostNum], "up") != 0)
            // {
            //     strcpy(ghostPrevMovement[ghostNum], ghostMovement);
            //     strcpy(ghostMovement[ghostNum], "up");
            // }
            // else
            // {
            float newX = ghostX[ghostNum];
            float newY = ghostY[ghostNum];
            newY += 1;
            if (isWallCollide(1, newX, newY) == true)
            {
                printf("WALL COLLIDE STOP\n");
                strcpy(ghostMovement[ghostNum], "up");
                return;
            }
            strcpy(ghostMovement[ghostNum], "down");
            // }
        }
        else if (randomMove == 1)
        {
            // if (strcmp(ghostPrevMovement[ghostNum], "down") != 0)
            // {
            //     strcpy(ghostPrevMovement[ghostNum], ghostMovement);
            //     strcpy(ghostMovement[ghostNum], "down");
            // }
            // else
            // {
            printf("ghost %d trying to move up\n", ghostNum);
            float newX = ghostX[ghostNum];
            float newY = ghostY[ghostNum];
            newY -= 1;
            if (isWallCollide(1, newX, newY) == true)
            {
                printf("WALL COLLIDE STOP\n");
                strcpy(ghostMovement[ghostNum], "down");
                return;
            }
            // }
            strcpy(ghostMovement[ghostNum], "up");
            // }
        }
    }
    else if (currAxis == 1)
    {
        int randomMove = rand() % 2;
        if (randomMove == 1)
        {
            printf("ghost %d trying to move right\n", ghostNum);
            // if (strcmp(ghostPrevMovement[ghostNum], "left") != 0)
            // {
            //     strcpy(ghostPrevMovement[ghostNum], ghostMovement);
            //     strcpy(ghostMovement[ghostNum], "left");
            // }
            // else
            // {
            float newX = ghostX[ghostNum];
            float newY = ghostY[ghostNum];
            newX += 1;
            if (isWallCollide(0, newX, newY) == true)
            {
                printf("WALL COLLIDE STOP\n");
                strcpy(ghostMovement[ghostNum], "left");
                return;
            }

            strcpy(ghostMovement[ghostNum], "right");
            //}
        }
        else if (randomMove == 0)
        {
            // if (strcmp(ghostPrevMovement[ghostNum], "right") != 0)
            // {
            //     strcpy(ghostPrevMovement[ghostNum], ghostMovement);
            //     strcpy(ghostMovement[ghostNum], "right");
            // }
            // else
            // {
            printf("ghost %d trying to move left\n", ghostNum);
            float newX = ghostX[ghostNum];
            float newY = ghostY[ghostNum];
            newX -= 1;
            if (isWallCollide(0, newX, newY) == true)
            {
                printf("WALL COLLIDE STOP\n");
                strcpy(ghostMovement[ghostNum], "right");
                return;
            }
            strcpy(ghostMovement[ghostNum], "left");
            // }
        }
    }
}

void *ghostThread(void *arg)
{
    int i = *(int *)arg;
    printf("i : %d\n", i);
    srand(time(0));
    changeGhostMovement(i, 0);
    while (1)
    {
        float newX = ghostX[i];
        float newY = ghostY[i];
        if (strcmp(ghostMovement[i], "down") == 0)
        {
            newY += 1;
            if (isWallCollide(1, newX, newY) == false)
            {
                ghostY[i] += 1;
                checkGhostCoords(i);
            }
            else
            {
                // if (isWallTurn[i] == false)
                // {
                //     //int turnChance = rand()%2;
                //     if (turnChance == 1)
                //     {
                //         isWallTurn[i] = true;
                //     }
                // }
                // else
                //     isWallTurn[i] = false;
                changeGhostMovement(i, 1);
            }
        }
        else if (strcmp(ghostMovement[i], "up") == 0)
        {
            newY -= 1;
            if (isWallCollide(1, newX, newY) == false)
            {
                ghostY[i] -= 1;
                checkGhostCoords(i);
            }
            else
            {
                // if (isWallTurn[i] == false)
                // {
                //     int turnChance = 1;
                //     if (turnChance == 1)
                //     {
                //         isWallTurn[i] = true;
                //     }
                // }
                // else
                //     isWallTurn[i] = false;
                changeGhostMovement(i, 1);
            }
        }
        else if (strcmp(ghostMovement[i], "left") == 0)
        {
            newX -= 1;
            if (isWallCollide(0, newX, newY) == false)
            {
                ghostX[i] -= 1;
                checkGhostCoords(i);
            }
            else
            {
                // int turnChance = 1;
                // if (isWallTurn[i] == false)
                // {
                //     int turnChance = 1;
                //     if (turnChance == 1)
                //     {
                //         isWallTurn[i] = true;
                //     }
                // }
                // else
                //     isWallTurn[i] = false;
                changeGhostMovement(i, 0);
            }
        }
        else if (strcmp(ghostMovement[i], "right") == 0)
        {
            newX += 1;
            if (isWallCollide(0, newX, newY) == false)
            {
                ghostX[i] += 1;
                checkGhostCoords(i);
            }
            else
            {
                // int turnChance = 1;
                // if (isWallTurn[i] == false)
                // {
                //     int turnChance = 1;
                //     if (turnChance == 1)
                //     {
                //         isWallTurn[i] = true;
                //     }
                // }
                // else
                //     isWallTurn[i] = false;
                changeGhostMovement(i, 0);
            }
            // printf("Ghost %d: %d\n", i, isWallTurn[i]);
        }
        // checkgraph();
        //  ghostMovementTry(i);
        glutPostRedisplay(); // Request redisplay
        usleep(10000);
    }
    pthread_exit(NULL);
}

// User Interface Thread
// logic :
// check x y coords of ghost
// if at a vertice -> rand() value to decide where to turn
// need to get mid coords