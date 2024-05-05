#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
// YAAANNNN
//  Global variables for texture, position, and size
// using my code here Hussnain
bool stoppac;
GLuint pacmanTextureID;
GLuint backgroundTextureID;
float x = 60.0f;
float y = 60.0f;
float side = 30.0f;                                      // Adjusted the size for clarity
const char *pacmanTexturePath = "imgs/pacman/right.png"; // Changed from char* to const char*
const char *backgroundTexturePath = "imgs/map/map.png";  // Path to the background texture

void *gameEngineThread(void *arg);
void *userInterfaceThread(void *arg);

char keypressed[10];
char prevkeypressed[10];

// Function to load texture from file
void loadTexture(const char *filename, GLuint *textureID)
{
    // Load image texture using SOIL
    *textureID = SOIL_load_OGL_texture(
        filename,           // Image file path
        SOIL_LOAD_AUTO,     // Automatically choose format
        SOIL_CREATE_NEW_ID, // Generate a new texture ID
        SOIL_FLAG_INVERT_Y  // Invert Y-axis (OpenGL's origin is at the bottom-left corner)
    );

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
    glBindTexture(GL_TEXTURE_2D, pacmanTextureID);
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

bool isWallCollide(float xx, float yy)
{
    int mapBordersY[2] = {60, 685};
    int mapBordersX[2] = {20, 555};

    if (xx >= mapBordersX[0] && xx <= mapBordersX[1])
    {
        if (yy >= mapBordersY[0] && yy <= mapBordersY[1])
        {
            float xHorizontalWalls[] = {20, 251.50, 320, 550, 20, 120, 450, 555, 125, 255, 325, 440, 195, 375, 195, 375, 130, 250, 325, 440, 195, 375, 25, 120, 450, 545, 25, 120, 130, 245, 325, 440, 450, 545};
            float yHorizontalWalls[] = {60.5, 134, 60.5, 134, 200, 264, 200, 264, 200, 264, 200, 264, 135, 199, 265, 325, 470, 534, 470, 534, 535, 599, 535, 599, 535, 599, 600, 680, 600, 680, 600, 680, 600, 680};

            int numElements = sizeof(xHorizontalWalls) / sizeof(xHorizontalWalls[0]);
            printf("%d", numElements);
            for (int i = 0; i < numElements; i += 2)
            {
                if (xx > xHorizontalWalls[i] && xx < xHorizontalWalls[i + 1])
                {
                    // printf("pass\n");
                    if (yy >= yHorizontalWalls[i] && yy <= yHorizontalWalls[i + 1])
                    {
                        // printf("collide\n");
                        return true;
                    }
                }
            }
        }
        else
            return true;
    }
    else
        return true;

    return false;
}

// Function to handle keyboard input
void keyboard(int key, float xx, float yy)
{
    float newX = x;
    float newY = y;
    switch (key)
    {
    case GLUT_KEY_RIGHT:
        strcpy(prevkeypressed, keypressed);
        strcpy(keypressed, "right");

        break;
    case GLUT_KEY_LEFT:
        strcpy(prevkeypressed, keypressed);
        strcpy(keypressed, "left");

        break;
    case GLUT_KEY_UP:
        strcpy(prevkeypressed, keypressed);
        strcpy(keypressed, "up");

        break;
    case GLUT_KEY_DOWN:
        strcpy(prevkeypressed, keypressed);
        strcpy(keypressed, "down");

        break;
    }
    // Reload the texture with the new filename
    loadTexture(pacmanTexturePath, &pacmanTextureID);
    glutPostRedisplay();
}

// Function to initialize OpenGL settings
void initOpenGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear color with alpha set to 0
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 600, 800, 0);
    glMatrixMode(GL_MODELVIEW);

    // Enable blending for transparent textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load the image textures
    loadTexture(pacmanTexturePath, &pacmanTextureID);
    loadTexture(backgroundTexturePath, &backgroundTextureID);
}

int main(int argc, char **argv)
{
    pthread_t EngineThread, playerThread;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(700, 900);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Pacman Game");
    initOpenGL();

    pthread_create(&EngineThread, NULL, gameEngineThread, NULL);
    pthread_create(&playerThread, NULL, userInterfaceThread, NULL);

    glutMainLoop();

    return 0;
}

void *gameEngineThread(void *arg)
{
    glutSpecialFunc(keyboard);

    // Initialize game engine
    while (1)
    {
        if (!stoppac)
        {
            float newX = x;
            float newY = y;
            if (strcmp(keypressed, "down") == 0)
            {
                newY += 0.5;
                if (isWallCollide(newX, newY) == false)
                {
                    pacmanTexturePath = "imgs/pacman/down.png";
                    y += 0.5;
                }
                else
                    strcpy(keypressed, "left");
            }
            else if (strcmp(keypressed, "up") == 0)
            {
                newY -= 0.5;
                if (isWallCollide(newX, newY) == false)
                {
                    pacmanTexturePath = "imgs/pacman/up.png";
                    y -= 0.5;
                }
                else
                    strcpy(keypressed, "right");
            }
            else if (strcmp(keypressed, "left") == 0)
            {
                newX -= 0.5;
                if (isWallCollide(newX, newY) == false)
                {

                    pacmanTexturePath = "imgs/pacman/left.png";
                    x -= 0.5;
                }
                else
                    strcpy(keypressed, "up");
            }
            else if (strcmp(keypressed, "right") == 0)
            {
                newX += 0.5;
                if (isWallCollide(newX, newY) == false)
                {

                    pacmanTexturePath = "imgs/pacman/right.png";
                    x += 0.5;
                }
                else
                    strcpy(keypressed, "down");
            }

            printPosition();
            glutDisplayFunc(display);
            glutPostRedisplay(); // Request redisplay
        }
        usleep(5000);
    }
    pthread_exit(NULL);
}

// User Interface Thread
void *userInterfaceThread(void *arg)
{
    // Initialize user interface
    while (1)
    {
    }
    pthread_exit(NULL);
}