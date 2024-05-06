#include <GL/gl.h>
#include <GL/glx.h>

#include <GL/glut.h>
#include <SOIL/SOIL.h>

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
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
GLuint ghostTextureID[2];

float x = 60.0f;
float y = 60.0f;
float side = 30.0f; // Adjusted the size for clarity

float ghostX[2] = {
    256,
    270};
float ghostY[2] = {466, 466};
int numGhost = 2;

char ghostMovement[2][10];

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
    {
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
    {
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

    pthread_t EngineThread, playerThread, GhostThread;
    pthread_create(&EngineThread, NULL, gameEngineThread, NULL);
    pthread_create(&playerThread, NULL, userInterfaceThread, NULL);
    pthread_create(&GhostThread, NULL, ghostThread, NULL);
    glutMainLoop();

    return 0;
}

void *gameEngineThread(void *arg)
{
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
        checkTeleport();
        glutPostRedisplay(); // Request redisplay
        usleep(5000);
    }
    pthread_exit(NULL);
}

void ghostMovementTry(int ghostNum)
{
    int random = rand() % 4;
    char tempMove[10];
    if (random == 1)
    {
        float tempX = ghostX[ghostNum];
        float tempY = ghostY[ghostNum];

        tempY += 1;
        if (isWallCollide(1, tempX, tempY) == false)
        {
            ghostY[ghostNum] += 1;
        }
        else
        {
            tempY -= 2;
            if (isWallCollide(1, tempX, tempY) == false)
            {
                ghostY[ghostNum] -= 1;
            }
            else
            {
                tempX += 1;
                if (isWallCollide(1, tempX, tempY) == false)
                {
                    ghostX[ghostNum] += 1;
                }
                else
                {
                    tempX -= 1;
                    if (isWallCollide(1, tempX, tempY) == false)
                    {
                        ghostX[ghostNum] -= 2;
                    }
                }
            }
        }
    }
}
void changeGhostMovement(int ghostNum)
{
    int randomMove = rand() % 4; // 0 is UP, 1 is DOWN, 2 is LEFT, 3 is RIGHT
    printf("%d\n", randomMove);
    if (randomMove == 0)
    {
        strcpy(ghostMovement[ghostNum], "up");
    }
    else if (randomMove == 1)
    {
        strcpy(ghostMovement[ghostNum], "down");
    }
    else if (randomMove == 2)
    {
        strcpy(ghostMovement[ghostNum], "left");
    }
    else if (randomMove == 3)
    {
        strcpy(ghostMovement[ghostNum], "right");
    }
}
void *ghostThread(void *arg)
{
    srand(time(0));
    for (int i = 0; i < numGhost; i++)
        changeGhostMovement(i);
    while (1)
    {
        for (int i = 0; i < numGhost; i++)
        {
            float newX = ghostX[i];
            float newY = ghostY[i];

            if (strcmp(ghostMovement[i], "down") == 0)
            {
                newY += 1;
                if (isWallCollide(1, newX, newY) == false)
                {
                    ghostY[i] += 1;
                }
                else
                {
                    changeGhostMovement(i);
                }
            }
            else if (strcmp(ghostMovement[i], "up") == 0)
            {
                newY -= 1;
                if (isWallCollide(1, newX, newY) == false)
                {
                    ghostY[i] -= 1;
                }
                else
                    changeGhostMovement(i);
            }
            else if (strcmp(ghostMovement[i], "left") == 0)
            {
                newX -= 1;
                if (isWallCollide(0, newX, newY) == false)
                {
                    ghostX[i] -= 1;
                }
                else
                {
                    changeGhostMovement(i);
                }
            }
            else if (strcmp(ghostMovement[i], "right") == 0)
            {
                newX += 1;
                if (isWallCollide(0, newX, newY) == false)
                {
                    ghostX[i] += 1;
                }
                else
                    changeGhostMovement(i);
            }
            // printPosition();
            // ghostMovementTry(i);
            glutPostRedisplay(); // Request redisplay
            usleep(5000);
        }
    }
    pthread_exit(NULL);
}

// User Interface Thread
void *userInterfaceThread(void *arg)
{
    // Initialize user interface
    glutSpecialFunc(keyboard);
    while (1)
    {
    }
    pthread_exit(NULL);
}