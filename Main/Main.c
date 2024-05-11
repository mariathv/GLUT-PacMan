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
// YAAANNNN
//  Global variables for texture, position, and size
// using my code here Hussnain

int animationtimer = 10;
bool isWallCollision = false;

bool stoppac;
GLuint pacmanRight[3];
GLuint pacmanLeft[3];
GLuint pacmanDown[3];
GLuint pacmanUp[3];
GLuint backgroundTextureID;
GLuint foodTextureID;

GLuint powerupTexture;
GLuint ghostTextureID[4];

float x = 280.0f;
float y = 195.0f;
float side = 30.0f;

float ghostX[4] = {285, 245, 330, 285};
float ghostY[4] = {405, 395, 395, 395}; //(285,395)
bool ghostChase[4] = {false, false, false, false};
int numGhost = 4;
char ghostMovement[4][10];

// ghost house mechanics
int ghostHouseEnterance = 0; // 0 = ghost 1(pinky), 1 = ghost2(clyde), 2= ghost 3(inky)
int houseYcoords[2] = {380, 410};
int inHouse[4] = {true, true, true, true}; // checks which ghosts are in the house
int ghostEnteranceTimer[2] = {0, 1000};
int ghostTimer = 0;
sem_t exit_permit[3];
bool exit_perm[3] = {false, false, false};
bool key[3] = {false, false, false};
// sem_t mainkey;

// will increase timer by 1000 (per ghost)

float foodside = 30.0f;
float powerupSide = 24.0f;

int arrPowerupx[] = {20, 555, 20, 555};
int arrPowerupy[] = {90, 90, 662, 662};
bool *checkPowerupEatArr;

int arrFoodx[] = {20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 237, 253, 273, 296, 317, 340, 360, 380, 400, 420, 440, 460, 480, 500, 520, 539, 555, 20, 40, 60, 80, 103, 125, 447, 463, 489, 509, 523, 539, 555, 190, 210, 230, 253, 317, 340, 360, 380, 20, 40, 60, 80, 100, 125, 140, 160, 180, 200, 220, 238, 254, 319, 340, 360, 380, 400, 420, 440, 460, 480, 500, 520, 539, 555, 20, 40, 60, 80, 103, 125, 20, 40, 60, 80, 100, 125, 140, 160, 180, 200, 220, 240, 260, 280, 300, 320, 340, 360, 380, 400, 423, 447, 463, 480, 500, 520, 539, 555, 447, 463, 482, 501, 520, 539, 555, 0, 20, 40, 60, 80, 100, 125, 143, 165, 189, 315, 338, 360, 380, 400, 420, 447, 463, 480, 501, 520, 539, 555, 125, 142, 160, 180, 200, 220, 240, 320, 340, 360, 380, 403, 425, 447, 20, 40, 61, 20, 40, 60, 80, 100, 125, 140, 160, 177, 193, 210, 230, 252, 509, 531, 555, 189, 204, 221, 240, 260, 280, 300, 320, 340, 362, 384, 189, 204, 221, 240, 260, 280, 300, 320, 340, 362, 384, 384, 400, 420, 447, 463, 480, 500, 520, 540, 560, 320, 340, 360, 384, 189, 213, 235, 256, 20, 20, 61, 61, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 20, 20, 254, 254, 254, 555, 555, 555, 555, 555, 319, 319, 319, 555, 555, 509, 509, 253, 253, 317, 317, 20, 20, 315, 315, 555, 555, 190, 190, 380, 380, 189, 189, 189, 189, 189, 189, 384, 384, 384, 384, 384, 384, 256, 256, 320, 320, 189, 189, 384, 384, 20, 20, 20, 252, 252};
int arrFoody[] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 531, 531, 531, 531, 531, 531, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 531, 531, 531, 531, 531, 531, 531, 395, 395, 395, 395, 395, 395, 395, 395, 395, 395, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 195, 195, 195, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 395, 395, 395, 395, 395, 395, 395, 395, 395, 395, 531, 531, 531, 531, 531, 531, 531, 531, 85, 107, 150, 170, 150, 173, 215, 240, 290, 310, 330, 350, 370, 420, 440, 460, 480, 505, 550, 570, 620, 640, 660, 150, 173, 215, 240, 290, 310, 330, 350, 370, 420, 440, 460, 480, 505, 550, 570, 620, 640, 660, 550, 570, 620, 640, 660, 555, 575, 620, 640, 660, 620, 640, 660, 85, 105, 150, 170, 80, 100, 80, 100, 220, 240, 220, 240, 220, 240, 150, 170, 150, 170, 285, 305, 350, 370, 420, 440, 285, 305, 350, 370, 420, 440, 490, 510, 490, 510, 550, 570, 550, 570, 620, 640, 660, 220, 240};

// 0   1    2    3    4     5    6    7    8    9    10    11    12    13   14   15   16   17   18   19   20   21   22   23   24    25    26    27   28    29  30   31   32   33    34   35   36   37  38   39  40    41   42   43   44   45  46   47  48   49  50  51  52  53  54  55  56   57  58  59  60  61  62  63  64   65
int xCoords[] = {20, 20, 125, 61, 20, 20, 190, 253, 253, 317, 317, 380, 380, 315, 315, 447, 447, 509, 509, 555, 555, 555, 555, 252, 252, 190, 125, 189, 189, 384, 384, 315, 189, 189, 384, 320, 256, 256, 189, 189, 254, 254, 125, 20, 20, 20, 125, 319, 319, 384, 320, 447, 555, 555, 555, 447, 384, 384, 61, 125, 447, 447, 125, 125, 447, 384};
int yCoords[] = {60, 128, 128, 195, 195, 262, 128, 128, 60, 128, 60, 128, 195, 195, 262, 195, 128, 195, 128, 128, 60, 195, 262, 195, 262, 195, 195, 262, 330, 330, 262, 262, 395, 466, 466, 466, 466, 531, 531, 596, 685, 596, 685, 696, 531, 596, 531, 685, 596, 531, 531, 685, 685, 596, 531, 531, 596, 395, 128, 262, 262, 395, 395, 596, 596, 395};

int graphi[] = {0, 0, 8, 8, 8, 10, 10, 10, 20, 20, 1, 1, 58, 58, 58, 2, 2, 6, 6, 7, 7, 9, 9, 11, 11, 16, 16, 18, 18, 18, 19, 19, 4, 4, 3, 3, 26, 26, 26, 25, 25, 25, 23, 23, 23, 13, 13, 13, 12, 12, 12, 15, 15, 15, 17, 17, 21, 21, 5, 5, 59, 59, 59, 59, 27, 27, 27, 24, 24, 14, 14, 60, 60, 60, 60, 22, 22, 62, 62, 62, 32, 32, 32, 57, 57, 57, 61, 61, 61, 33, 33, 36, 36, 36, 35, 35, 35, 44, 44, 46, 46, 46, 38, 38, 37, 37, 50, 50, 49, 49, 55, 55, 55, 54, 54, 45, 45, 45, 63, 63, 63, 63, 39, 39, 39, 41, 41, 41, 48, 48, 48, 56, 56, 56, 64, 64, 64, 64, 53, 53, 53, 43, 43, 42, 42, 42, 40, 40, 47, 47, 51, 51, 51, 52, 52, 28, 28, 28, 29, 29, 29, 30, 30, 30, 31, 31, 34, 34};
int graphj[] = {1, 8, 0, 7, 10, 8, 9, 20, 10, 19, 52, 0, 1, 2, 3, 52, 26, 7, 25, 8, 6, 11, 10, 9, 12, 15, 18, 16, 19, 17, 18, 20, 3, 5, 52, 4, 2, 59, 25, 6, 26, 23, 25, 24, 13, 23, 12, 14, 11, 15, 13, 12, 16, 60, 18, 21, 17, 22, 4, 59, 5, 26, 27, 62, 59, 28, 24, 27, 23, 30, 13, 30, 15, 22, 61, 60, 21, 59, 32, 46, 62, 28, 33, 29, 61, 34, 60, 57, 55, 32, 36, 33, 37, 35, 36, 50, 34, 45, 46, 62, 44, 63, 37, 39, 36, 38, 35, 49, 50, 56, 61, 54, 64, 55, 53, 44, 63, 43, 46, 39, 42, 45, 63, 38, 41, 39, 48, 40, 47, 41, 56, 48, 49, 64, 56, 55, 53, 51, 54, 64, 52, 45, 42, 43, 63, 40, 41, 42, 48, 51, 47, 64, 52, 51, 53, 27, 29, 32, 30, 57, 28, 14, 60, 29, 13, 30, 35, 57};

int xMidCoords[] = {447, 125, 447, 125, 447, 254, 125, 447, 125, 555};
int yMidCoords[] = {262, 330, 330, 395, 395, 466, 596, 596, 262, 596};

int xDownMidCoords[] = {555};
int yDownMidCoords[] = {596};

int coordsXYSize = sizeof(yCoords) / sizeof(yCoords[0]);
const int foodXYSize = sizeof(arrFoodx) / sizeof(arrFoodx[0]);
int powerupXYsize = sizeof(arrPowerupx) / sizeof(arrPowerupx[0]);

bool *checkFoodEatArr;
bool *checkPowerupEatArr;

void *gameEngineThread(void *arg);
void *userInterfaceThread(void *arg);
void *ghostThread(void *arg);

char triedKeyPressed[10];
int delayTimer = 0;
char keypressed[10] = "up";
char delayKey[10];
char prevkeypressed[10];
int parent[100];

pthread_mutex_t lock;

bool delayFlag = false;

int size;

bool isWallCollide(bool moveAxis, float xx, float yy);
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
}

void createGrapha()
{
    size = 66;
    int arrSize = sizeof(graphi) / sizeof(graphi[0]);

    graph = createGraph(66);

    for (int i = 0; i < arrSize; ++i)
    {
        int x = xCoords[graphj[i]] - xCoords[graphi[i]];
        int y = yCoords[graphj[i]] - yCoords[graphi[i]];
        int ans = sqrt((x * x) + (y * y));
        addEdge(graph, graphi[i], graphj[i], ans);
    }
}

void print()
{
    int size = sizeof(xCoords) / sizeof(xCoords[0]);
    for (int i = 0; i < size; ++i)
    {
        printf("index : %d    --- vertex %d         %d\n", i, xCoords[i], yCoords[i]);
        // printf("index =  %d \n",i );
    }
}

int checkCorner(float x, float y)
{
    int size = sizeof(xCoords) / sizeof(xCoords[0]);
    for (int i = 0; i < size; ++i)
    {
        if (x == xCoords[i] && y == yCoords[i])
        {
            return i;
        }
    }
    return -1;
}

int checkClosest(char *mov, float xposi, float yposi, char *type)
{
    int yup = yposi;
    int ydown = yposi;
    int xleft = xposi;
    int xright = xposi;

    int index = -1;
    index = checkCorner(xposi, yposi);
    if (index != -1)
        return index;

    while (1)
    {
        yup -= 1;
        ydown += 1;

        index = checkCorner(xposi, yup);
        if (index != -1)
        {
            if (type == "ghost")
                strcpy(mov, "up");
            return index;
        }

        index = index = checkCorner(xposi, ydown);
        if (index != -1)
        {
            if (type == "ghost")
                strcpy(mov, "down");
            return index;
        }

        xleft -= 1;
        xright += 1;

        index = checkCorner(xleft, yposi);
        if (index != -1)
        {
            if (type == "ghost")
                strcpy(mov, "left");
            return index;
        }

        index = checkCorner(xright, yposi);
        if (index != -1)
        {
            if (type == "ghost")
                strcpy(mov, "right");
            return index;
        }
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

int currAnimation = 0;

int score = 0;
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

    for (int i = 0; i < powerupXYsize; ++i)
    {
        if (checkPowerupEatArr[i] == true)
            continue;
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, powerupTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2f(arrPowerupx[i], arrPowerupy[i]);
        glTexCoord2f(1, 0);
        glVertex2f(arrPowerupx[i] + powerupSide, arrPowerupy[i]);
        glTexCoord2f(1, 1);
        glVertex2f(arrPowerupx[i] + powerupSide, arrPowerupy[i] + (powerupSide * 1.0f));
        glTexCoord2f(0, 1);
        glVertex2f(arrPowerupx[i], arrPowerupy[i] + (powerupSide * 1.0f));
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }

    // Draw Pacman
    glEnable(GL_TEXTURE_2D);
    if (strcmp(keypressed, "left") == 0)
        glBindTexture(GL_TEXTURE_2D, pacmanLeft[currAnimation]);
    else if (strcmp(keypressed, "up") == 0)
        glBindTexture(GL_TEXTURE_2D, pacmanDown[currAnimation]);
    else if (strcmp(keypressed, "down") == 0)
        glBindTexture(GL_TEXTURE_2D, pacmanUp[currAnimation]);
    else
        glBindTexture(GL_TEXTURE_2D, pacmanRight[currAnimation]);
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
    // Draw Ghost INKY
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ghostTextureID[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(1, 1);
    glVertex2f(ghostX[2], ghostY[2]);
    glTexCoord2f(0, 1);
    glVertex2f(ghostX[2] + side, ghostY[2]);
    glTexCoord2f(0, 0);
    glVertex2f(ghostX[2] + side, ghostY[2] + (side * 1.0f)); // Adjusted the height of the quad
    glTexCoord2f(1, 0);
    glVertex2f(ghostX[2], ghostY[2] + (side * 1.0f)); // Adjusted the height of the quad
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

    // DRAW GHOST BLINKY
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ghostTextureID[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(1, 1);
    glVertex2f(ghostX[3], ghostY[3]);
    glTexCoord2f(0, 1);
    glVertex2f(ghostX[3] + side, ghostY[3]);
    glTexCoord2f(0, 0);
    glVertex2f(ghostX[3] + side, ghostY[3] + (side * 1.0f)); // Adjusted the height of the quad
    glTexCoord2f(1, 0);
    glVertex2f(ghostX[3], ghostY[3] + (side * 1.0f)); // Adjusted the height of the quad
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
            score++;
            checkFoodEatArr[i] = true;
        }
    }
}

void checkPowerupEat()
{
    for (int i = 0; i < powerupXYsize; ++i)
    {
        if (arrPowerupx[i] == x && arrPowerupy[i] == y && checkPowerupEatArr[i] == false)
        {
            checkPowerupEatArr[i] = true;
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
    printPosition();
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

    loadTexture("imgs/pacman/right1.png", &pacmanRight[0]);
    loadTexture("imgs/pacman/common.png", &pacmanRight[1]);
    loadTexture("imgs/pacman/right3.png", &pacmanRight[2]);
    loadTexture("imgs/pacman/left1.png", &pacmanLeft[0]);
    loadTexture("imgs/pacman/common.png", &pacmanLeft[1]);
    loadTexture("imgs/pacman/left3.png", &pacmanLeft[2]);
    loadTexture("imgs/pacman/up1.png", &pacmanUp[0]);
    loadTexture("imgs/pacman/common.png", &pacmanUp[1]);
    loadTexture("imgs/pacman/up3.png", &pacmanUp[2]);
    loadTexture("imgs/pacman/down1.png", &pacmanDown[0]);
    loadTexture("imgs/pacman/common.png", &pacmanDown[1]);
    loadTexture("imgs/pacman/down3.png", &pacmanDown[2]);
    loadTexture("imgs/map/map.png", &backgroundTextureID);
    loadTexture("imgs/food/dot.png", &foodTextureID);
    loadTexture("imgs/ghosts/pinky.png", &ghostTextureID[0]);
    loadTexture("imgs/ghosts/clyde.png", &ghostTextureID[1]);
    loadTexture("imgs/food/Pellet_Medium.png", &powerupTexture);
    loadTexture("imgs/ghosts/inky.png", &ghostTextureID[2]);
    loadTexture("imgs/ghosts/blinky.png", &ghostTextureID[3]);
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

    for (int i = 0; i < 3; i++)
    {
        sem_init(&exit_permit[i], 0, 1);
    }

    createGrapha();
    printGraph(graph);

    int ghostNumber1 = 0;
    int ghostNumber2 = 1;
    int ghostNumber3 = 2;
    int ghostNumber4 = 3;

    pthread_t EngineThread, playerThread, Ghost1, Ghost2, Ghost3, Ghost4;
    pthread_create(&EngineThread, NULL, gameEngineThread, NULL);
    pthread_create(&playerThread, NULL, userInterfaceThread, NULL);
    pthread_create(&Ghost1, NULL, ghostThread, (void *)&ghostNumber1);
    pthread_create(&Ghost2, NULL, ghostThread, (void *)&ghostNumber2);
    pthread_create(&Ghost3, NULL, ghostThread, (void *)&ghostNumber3);
    pthread_create(&Ghost4, NULL, ghostThread, (void *)&ghostNumber4);
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
        int totalVertices = sizeof(xCoords) / sizeof(xCoords[0]);
        for (int i = 0; i < 8; i++)
        {
            if (ghostX[ghostNum] == xMidCoords[i] && ghostY[ghostNum] == yMidCoords[i])
            {
                if ((strcmp(ghostMovement[ghostNum], "left") == 0) || (strcmp(ghostMovement[ghostNum], "right") == 0))
                {

                    float newX = ghostX[ghostNum];
                    float newY = ghostY[ghostNum];
                    newX = ghostX[ghostNum];
                    newY = ghostY[ghostNum] + 1;
                    if (isWallCollide(1, newX, newY) == false)
                    {
                        ghostY[ghostNum] += 1;
                        strcpy(ghostMovement[ghostNum], "down");
                        return;
                    }
                    newX = ghostX[ghostNum];
                    newY = ghostY[ghostNum] - 1;
                    if (isWallCollide(1, newX, newY) == false)
                    {
                        ghostY[ghostNum] -= 1;
                        strcpy(ghostMovement[ghostNum], "up");
                        return;
                    }
                }
                else if ((strcmp(ghostMovement[ghostNum], "up") == 0) || (strcmp(ghostMovement[ghostNum], "down") == 0))
                {
                    float newX = ghostX[ghostNum];
                    float newY = ghostY[ghostNum];
                    newX = ghostX[ghostNum] + 1;
                    newY = ghostY[ghostNum];
                    if (isWallCollide(0, newX, newY) == false)
                    {
                        ghostX[ghostNum] += 1;
                        strcpy(ghostMovement[ghostNum], "right");
                        return;
                    }
                    newX = ghostX[ghostNum] - 1;
                    newY = ghostY[ghostNum];
                    if (isWallCollide(0, newX, newY) == false)
                    {
                        ghostX[ghostNum] -= 1;
                        strcpy(ghostMovement[ghostNum], "left");
                        return;
                    }
                }
            }
        }
    }
}

void keyPermitCheck(int i)
{
    float newX = ghostX[i];
    float newY = ghostY[i];
    int EnteranceTimerCheck = 0;

    int left = i - 1;
    int right = (i) % 3;

    if (i != 0)
    {
        EnteranceTimerCheck = 1;
    }
    if (ghostTimer >= ghostEnteranceTimer[EnteranceTimerCheck])
    {
        if (i != 0)
        {
            key[i - 1] = true;
        }

        if (key[i - 1] == true && exit_perm[i - 1] == false)
        {
            sem_wait(&exit_permit[left]);
            sem_wait(&exit_permit[right]);
            exit_perm[i - 1] = true;
        }

        // strcpy(ghostMovement[i], "down");
        if (ghostX[i] == 285) // 285 = the x position of Enterance
            ghostY[i]++;
        else
        {
            if (exit_perm[i - 1] == true)
            {
                if (i == 1)
                    ghostX[i]++;
                else if (i == 2)
                    ghostX[i]--;
            }
        }
        if (ghostY[i] == 466) // 466 = the y position where the actual ghost movement will start
        {
            if (i != 0)
            {
                printf("ghost %d releasing permit\n", i + 1);
                sem_post(&exit_permit[left]);
                sem_post(&exit_permit[right]);
            }
            inHouse[i] = false;
        }
    }
    else
    {
        if (strcmp(ghostMovement[i], "down") == 0)
        {
            newY += 1;
            if (newY == houseYcoords[1])
            {
                strcpy(ghostMovement[i], "up");
            }
            else
                ghostY[i]++;
        }
        else if (strcmp(ghostMovement[i], "up") == 0)
        {
            newY -= 1;
            if (newY == houseYcoords[0])
            {
                strcpy(ghostMovement[i], "down");
            }
            else
                ghostY[i]--;
        }
    }
}

void *gameEngineThread(void *arg)
{
    checkFoodEatArr = malloc(foodXYSize * sizeof(bool));
    checkPowerupEatArr = malloc(powerupXYsize * sizeof(bool));
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
            }
        }
        else if (strcmp(keypressed, "up") == 0)
        {
            newY -= 0.5;
            if (isWallCollide(1, newX, newY) == false)
            {
                y -= 0.5;
            }
        }
        else if (strcmp(keypressed, "left") == 0)
        {
            newX -= 0.5;
            if (isWallCollide(0, newX, newY) == false)
            {
                x -= 0.5;
            }
        }
        else if (strcmp(keypressed, "right") == 0)
        {
            newX += 0.5;
            if (isWallCollide(0, newX, newY) == false)
            {
                x += 0.5;
            }
        }

        // printPosition();
        checkPowerupEat();
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
        // printf("SCORE : %d\n", score);
        animationtimer--;
        if (animationtimer <= 0)
        {
            animationtimer = 30;
            currAnimation = (currAnimation + 1) % 3;
        }
        usleep(5000);
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
            // printf("ghost %d trying to move down\n", ghostNum);
            float newX = ghostX[ghostNum];
            float newY = ghostY[ghostNum];
            newY += 1;
            if (isWallCollide(1, newX, newY) == true)
            {
                // printf("WALL COLLIDE STOP\n");
                strcpy(ghostMovement[ghostNum], "up");
                return;
            }
            strcpy(ghostMovement[ghostNum], "down");
            // }
        }
        else if (randomMove == 1)
        {
            // printf("ghost %d trying to move up\n", ghostNum);
            float newX = ghostX[ghostNum];
            float newY = ghostY[ghostNum];
            newY -= 1;
            if (isWallCollide(1, newX, newY) == true)
            {
                // printf("WALL COLLIDE STOP\n");
                strcpy(ghostMovement[ghostNum], "down");
                return;
            }
            strcpy(ghostMovement[ghostNum], "up");
        }
    }
    else if (currAxis == 1)
    {
        int randomMove = rand() % 2;
        if (randomMove == 1)
        {
            // printf("ghost %d trying to move right\n", ghostNum);
            float newX = ghostX[ghostNum];
            float newY = ghostY[ghostNum];
            newX += 1;
            if (isWallCollide(0, newX, newY) == true)
            {
                // printf("WALL COLLIDE STOP\n");
                strcpy(ghostMovement[ghostNum], "left");
                return;
            }

            strcpy(ghostMovement[ghostNum], "right");
        }
        else if (randomMove == 0)
        {
            // printf("ghost %d trying to move left\n", ghostNum);
            float newX = ghostX[ghostNum];
            float newY = ghostY[ghostNum];
            newX -= 1;
            if (isWallCollide(0, newX, newY) == true)
            {
                // printf("WALL COLLIDE STOP\n");
                strcpy(ghostMovement[ghostNum], "right");
                return;
            }
            strcpy(ghostMovement[ghostNum], "left");
        }
    }
}

bool checkVertexReached(int vertex, int i)
{
    if (ghostX[i] == xCoords[vertex] && ghostY[i] == yCoords[vertex])
        return true;
    return false;
}

void findDirectionPath(int vertex, int i)
{
    float x = ghostX[i];
    float y = ghostY[i];

    printf("Second Vertex Coords %d    %d\n", xCoords[vertex], yCoords[vertex]);
    printf("Ghost Coords         %f    %f\n", ghostX[i], ghostY[i]);

    if (x == xCoords[vertex])
    {
        printf("X are Equal\n");
        float yy = y;
        while (1)
        {
            y += 1;
            yy -= 1;

            // printf("%f           %f\n" , y , yy);
            if (y == yCoords[vertex])
            {
                strcpy(ghostMovement[i], "down");
                printf("Suggest  down\n");
                return;
            }
            if (yy == yCoords[vertex])
            {
                strcpy(ghostMovement[i], "up");
                printf("Suggest  up\n");
                return;
            }
        }
    }
    else
    {
        printf("Y are Equal\n");
        float xx = x;
        while (1)
        {
            x -= 1;
            xx += 1;
            //  printf("%f           %f\n" , x , xx);
            if (x == xCoords[vertex])
            {
                strcpy(ghostMovement[i], "left");
                printf("Suggest  left\n");
                return;
                printf("Suggest  left\n");
                break;
            }
            if (xx == xCoords[vertex])
            {
                strcpy(ghostMovement[i], "right");
                printf("Suggest  right\n");
                return;
            }
        }
    }
}

void *ghostThread(void *arg)
{
    int i = *(int *)arg;
    printf("i : %d\n", i);
    int firstVertex = -1;
    int secondVertex = -1;
    bool secondReached = false;
    bool firstReached = false;
    bool applyShortedPath = false;
    int ghostVertex = -1;
    int playerVertex = -1;
    srand(time(0));
    changeGhostMovement(i, 0);
    strcpy(ghostMovement[0], "down");
    while (1)
    {
        if (ghostChase[i] == true)
        {
            if (applyShortedPath == false)
            {
                printf("--------------------------------------------\n\n");
                int ghostVertex;
                if (firstReached == false)
                {
                    ghostVertex = checkClosest(ghostMovement[i], ghostX[i], ghostY[i], "ghost");
                }
                else
                {
                    ghostVertex = secondVertex;
                }

                int pacmanVertex = checkClosest(keypressed, x, y, "pacman");

                dijkstra(graph, pacmanVertex, ghostVertex);

                secondVertex = -1;
                int j = 0;
                int current = ghostVertex;
                while (current != -1)
                {
                    if (j == 0)
                        firstVertex = current;
                    if (j == 1)
                        secondVertex = current;
                    j++;
                    current = parent[current];
                }

                printf("\n\nFirst Vertex = %d\n", firstVertex);
                printf("Second Vertex = %d\n", secondVertex);
                printf("Current movement = %s\n\n", ghostMovement[i]);

                if (firstReached == true)
                {
                    findDirectionPath(secondVertex, i);
                }

                applyShortedPath = true;
            }

            if (firstReached == false)
            {
                if (checkVertexReached(firstVertex, i))
                {
                    printf("First Vertex Reached\n");
                    firstReached = true;
                    findDirectionPath(secondVertex, i);
                }
            }
            if (firstReached == true && secondReached == false)
            {
                if (checkVertexReached(secondVertex, i))
                    secondReached = true;
            }
            if (secondReached == true)
            {
                printf("Second Vertex Reached\n");
                secondReached = false;
                applyShortedPath = false;
                printf("\n\n");
                continue;
            }
        }

        float newX = ghostX[i];
        float newY = ghostY[i];

        if (inHouse[i] == true)
        {
            keyPermitCheck(i);
        }
        else
        {

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
                    if (ghostChase[i] == false)
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
                    if (ghostChase[i] == false)
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
                    if (ghostChase[i] == false)
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
                    if (ghostChase[i] == false)
                        changeGhostMovement(i, 0);
                }
                // printf("Ghost %d: %d\n", i, isWallTurn[i]);
            }
        }

        // break;
        // checkgraph();
        // ghostMovementTry(i);
        ghostTimer++;
        glutPostRedisplay(); // Request redisplay
        usleep(10000);
    }
    pthread_exit(NULL);
}
