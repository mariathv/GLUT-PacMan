#include <GL/gl.h>
#include <GL/glx.h>

#include <GL/glut.h>
#include <SOIL/SOIL.h>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
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

float x = 280.0f;
float y = 195.0f;
float side = 30.0f;

float foody = 60.0f;
float foodx = 20.0f; // Adjusted the size for clarity
float foodside = 30.0f;

int arrFoody[] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 685, 531, 531, 531, 531, 531, 531, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 531, 531, 531, 531, 531, 531, 531, 395, 395, 395, 395, 395, 395, 395, 395, 395, 395, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 195, 195, 195, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 395, 395, 395, 395, 395, 395, 395, 395, 395, 395, 531, 531, 531, 531, 531, 531, 531, 531, 85, 107, 150, 170, 150, 173, 215, 240, 290, 310, 330, 350, 370, 420, 440, 460, 480, 505, 550, 570, 620, 640, 660, 150, 173, 215, 240, 290, 310, 330, 350, 370, 420, 440, 460, 480, 505, 550, 570, 620, 640, 660, 550, 570, 620, 640, 660, 555, 575, 620, 640, 660,620, 640, 660,85,105,150,170,80,100,80,100,220,240,220,240,220,240,150,170,150,170,285,305,350,370,420,440,285,305,350,370,420,440,490,510,490,510,550,570,550,570,620,640,660,220,240};
int arrFoodx[] = { 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 237, 253, 273, 296, 317, 340, 360, 380, 400, 420, 440, 460, 480, 500, 520, 539, 555, 20, 40, 60, 80, 103, 125, 447, 463, 489, 509, 523, 539, 555, 190, 210, 230, 253, 317, 340, 360, 380, 20, 40, 60, 80, 100, 125, 140, 160, 180, 200, 220, 238, 254, 319, 340, 360, 380, 400, 420, 440, 460, 480, 500, 520, 539, 555, 20, 40, 60, 80, 103, 125, 20, 40, 60, 80, 100, 125, 140, 160, 180, 200, 220, 240, 260, 280, 300, 320, 340, 360, 380, 400, 423, 447, 463, 480, 500, 520, 539, 555, 447, 463, 482, 501, 520, 539, 555, 0, 20, 40, 60, 80, 100, 125, 143, 165, 189, 315, 338, 360, 380, 400, 420, 447, 463, 480, 501, 520, 539, 555, 125, 142, 160, 180, 200, 220, 240, 320, 340, 360, 380, 403, 425, 447, 20, 40, 61, 20, 40, 60, 80, 100, 125  , 140, 160, 177, 193, 210, 230, 252, 509, 531, 555, 189, 204, 221, 240, 260, 280, 300, 320, 340, 362, 384, 189, 204, 221, 240, 260, 280, 300, 320, 340, 362, 384, 384, 400, 420, 447, 463, 480, 500, 520, 540, 560, 320, 340, 360, 384, 189, 213, 235, 256, 20, 20, 61, 61, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 447, 20, 20, 254, 254, 254, 555, 555, 555, 555, 555,319,319,319,555,555,509,509,253,253,317,317,20,20,315,315,555,555,190,190,380,380,189,189,189,189,189,189,384,384,384,384,384,384,256,256,320,320,189,189,384,384,20,20,20,252,252};
const int foodXYSize = sizeof(arrFoodx) / sizeof(arrFoodx[0]);

bool *checkFoodEatArr ;

void *gameEngineThread(void *arg);
void *userInterfaceThread(void *arg);

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

    for (int i = 0; i < foodXYSize; ++i)
    {
        if(checkFoodEatArr[i])
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

    glutSwapBuffers();
}

void printPosition()
{
    printf("position: (%.2f, %.2f)\n", x, y);
}

void checkfoodEat()
{
    for(int i = 0 ; i < foodXYSize ; ++i)
    {
        if(arrFoodx[i] == x && arrFoody[i] == y && checkFoodEatArr[i] == false)
        {
            printf("Food Eaten\n");
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

    pthread_t EngineThread, playerThread;
    pthread_create(&EngineThread, NULL, gameEngineThread, NULL);
    pthread_create(&playerThread, NULL, userInterfaceThread, NULL);
    glutMainLoop();

    return 0;
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
            }
        }
        else if (strcmp(keypressed, "up") == 0)
        {
            newY -= 0.5;
            if (isWallCollide(1, newX, newY) == false)
            {
                y -= 0.5;
                printPosition();
            }
        }
        else if (strcmp(keypressed, "left") == 0)
        {
            newX -= 0.5;
            if (isWallCollide(0, newX, newY) == false)
            {
                x -= 0.5;
                printPosition();
            }
        }
        else if (strcmp(keypressed, "right") == 0)
        {
            newX += 0.5;
            if (isWallCollide(0, newX, newY) == false)
            {
                x += 0.5;
                printPosition();
            }
        }

        checkTeleport();
        checkfoodEat();
        glutPostRedisplay(); // Request redisplay
        usleep(5000);
    }
    return NULL;
}

// User Interface Thread
void *userInterfaceThread(void *arg)
{
    // Initialize user interface
    glutSpecialFunc(keyboard);
    while (1)
    {
    }
    return NULL;
}