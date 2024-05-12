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
#include "dijkstra.h"
#include "utility.h"

int currAnimation = 0;

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

bool isWallCollide(bool moveAxis, float xx, float yy);

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

    for (int i = 0; i < fruitCount; ++i)
    {
        if (i < 2)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, strawberryTexture);
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex2f(fruitX[i], fruitY[i]);
            glTexCoord2f(1, 0);
            glVertex2f(fruitX[i] + fruitSide, fruitY[i]);
            glTexCoord2f(1, 1);
            glVertex2f(fruitX[i] + fruitSide, fruitY[i] + (fruitSide * 1.0f));
            glTexCoord2f(0, 1);
            glVertex2f(fruitX[i], fruitY[i] + (fruitSide * 1.0f));
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
        else
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, appleTexture);
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex2f(fruitX[i], fruitY[i]);
            glTexCoord2f(1, 0);
            glVertex2f(fruitX[i] + fruitSide, fruitY[i]);
            glTexCoord2f(1, 1);
            glVertex2f(fruitX[i] + fruitSide, fruitY[i] + (fruitSide * 1.0f));
            glTexCoord2f(0, 1);
            glVertex2f(fruitX[i], fruitY[i] + (fruitSide * 1.0f));
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
    }

    // Draw Pacman
    glEnable(GL_TEXTURE_2D);
    if (strcmp(keypressed, "left") == 0)
        glBindTexture(GL_TEXTURE_2D, pacmanLeft[currAnimation]);
    else if (strcmp(keypressed, "up") == 0)
        glBindTexture(GL_TEXTURE_2D, pacmanDown);
    else if (strcmp(keypressed, "down") == 0)
        glBindTexture(GL_TEXTURE_2D, pacmanUp);
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
    glVertex2f(ghostX[1] + fruitSide, ghostY[1]);
    glTexCoord2f(0, 0);
    glVertex2f(ghostX[1] + fruitSide, ghostY[1] + (fruitSide * 1.0f)); // Adjusted the height of the quad
    glTexCoord2f(1, 0);
    glVertex2f(ghostX[1], ghostY[1] + (fruitSide * 1.0f)); // Adjusted the height of the quad
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

void checkPowerupEat()
{
    for (int i = 0; i < powerupXYsize; ++i)
    {
        if (arrPowerupx[i] == x && arrPowerupy[i] == y && checkPowerupEatArr[i] == false)
        {
            poweUpEaten = true;
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

void keyboard(int key)
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

void movePacman(const char *direction)
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
            float newX = ghostX[ghostNum];
            float newY = ghostY[ghostNum];
            newX += 1;
            if (isWallCollide(0, newX, newY) == true)
            {
                strcpy(ghostMovement[ghostNum], "left");
                return;
            }

            strcpy(ghostMovement[ghostNum], "right");
        }
        else if (randomMove == 0)
        {
            float newX = ghostX[ghostNum];
            float newY = ghostY[ghostNum];
            newX -= 1;
            if (isWallCollide(0, newX, newY) == true)
            {
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
    int right = (i) % 2;

    if (i == 1 || i == 2)
    {
        EnteranceTimerCheck = 1;
    }
    if (ghostTimer >= ghostEnteranceTimer[EnteranceTimerCheck])
    {
        if (i == 1 || i == 2)
        {
            key[i - 1] = true;
        }

        if (key[i - 1] == true && exit_perm[i - 1] == false)
        {
            int left_try = sem_trywait(&exit_permit[left]);
            int right_try = sem_trywait(&exit_permit[right]);
            if (left_try == 0 && right_try == 0)
            {
                exit_perm[i - 1] = true;
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
            if (i == 1 || i == 2)
            {
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

void checkGhostLineOfSight(int i) // function to check if pacman come in line of sight start following
{
    int yup = ghostY[i];
    int ydown = ghostY[i];
    int xleft = ghostX[i];
    int xright = ghostX[i];
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    int index = -1;

    while (1)
    {
        yup -= 1;
        ydown += 1;

        if (isWallCollide(1, ghostX[i], yup))
        {
            up = true;
        }
        if (yup == y && ghostX[i] == x && up == false)
        {
            strcpy(ghostMovement[i], "up");
            return;
        }

        if (isWallCollide(1, ghostX[i], ydown))
        {
            down = true;
        }
        if (ydown == y && ghostX[i] == x && down == false)
        {
            strcpy(ghostMovement[i], "down");
            return;
        }

        xleft -= 1;
        xright += 1;

        if (isWallCollide(0, xleft, ghostY[i]))
        {
            left = true;
        }
        if (ghostY[i] == y && xleft == x && left == false)
        {
            strcpy(ghostMovement[i], "left");
            return;
        }

        if (isWallCollide(0, xright, ghostY[i]))
        {
            right = true;
        }
        if (ghostY[i] == y && xright == x && right == false)
        {
            strcpy(ghostMovement[i], "right");
            return;
        }

        if (up && down && left && right)
            return;
    }
}

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
    loadTexture("imgs/pacman/right2.png", &pacmanRight[1]);
    loadTexture("imgs/pacman/right3.png", &pacmanRight[2]);
    loadTexture("imgs/pacman/left.png", &pacmanLeft[0]);
    loadTexture("imgs/pacman/left1.png", &pacmanLeft[1]);
    loadTexture("imgs/pacman/left2.png", &pacmanLeft[2]);
    loadTexture("imgs/pacman/up.png", &pacmanUp);
    loadTexture("imgs/pacman/down.png", &pacmanDown);
    loadTexture("imgs/map/map.png", &backgroundTextureID);
    loadTexture("imgs/food/dot.png", &foodTextureID);
    loadTexture("imgs/ghosts/pinky.png", &ghostTextureID[0]);
    loadTexture("imgs/ghosts/clyde.png", &ghostTextureID[1]);
    loadTexture("imgs/food/Pellet_Medium.png", &powerupTexture);
    loadTexture("imgs/ghosts/inky.png", &ghostTextureID[2]);
    loadTexture("imgs/food/apple.png", &appleTexture);
    loadTexture("imgs/food/strawberry.png", &strawberryTexture);
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

    printf("size = %d\n", coordsXYSize);
    for (int i = 0; i < 2; i++)
    {
        sem_init(&exit_permit[i], 0, 1);
    }

    createGrapha();
    printGraph(graph);

    int ghostNumber1 = 0;
    int ghostNumber2 = 1;
    int ghostNumber3 = 2;

    pthread_t EngineThread, playerThread, Ghost1, Ghost2, Ghost3;
    pthread_create(&EngineThread, NULL, gameEngineThread, NULL);
    pthread_create(&playerThread, NULL, userInterfaceThread, NULL);
    pthread_create(&Ghost1, NULL, ghostThread, (void *)&ghostNumber1);
    pthread_create(&Ghost2, NULL, ghostThread, (void *)&ghostNumber2);
    pthread_create(&Ghost3, NULL, ghostThread, (void *)&ghostNumber3);

    glutMainLoop();

    return 0;
}

void *gameEngineThread(void *arg)
{
    srand(time(NULL));
    timer = rand() % (10000 - 1000 + 1) + 1000;
    timer = 20;
    checkFoodEatArr = malloc(foodXYSize * sizeof(bool));
    checkPowerupEatArr = malloc(powerupXYsize * sizeof(bool));
    while (1)
    {
        animationtimer--;
        if (animationtimer <= 0)
        {
            animationtimer = 20;
            currAnimation = (currAnimation + 1) % 3;
        }

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
        if (timer < 0)
        {
            if (fruitCount < 3)
            {
                while (true)
                {
                    printf("--1\n");
                    int randomvalue = rand() % 66;
                    bool check = false;
                    for (int i = 0; i < fruitCount; ++i)
                    {
                        if (xCoords[randomvalue] == fruitX[i] && yCoords[randomvalue] == fruitY[i])
                        {
                            check = true;
                            break;
                        }
                    }
                    if (check)
                        continue;
                    fruitX[fruitCount] = xCoords[randomvalue];
                    fruitY[fruitCount] = yCoords[randomvalue];
                    fruitCount++;
                    break;
                    printf("--2\n");
                }
            }
            timer = rand() % (10000 - 1000 + 1) + 1000;
        }
        printf("%d\n" , timer);
        // printPosition();
        timer -= 1;
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
    }
    return NULL;
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

        float newX = ghostX[i];
        float newY = ghostY[i];

        if (inHouse[i] == true)
        {
            keyPermitCheck(i);
        }
        else
        {

            if (ghostChase[i] == true)
            {
                if (applyShortedPath == false)
                {
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

                    parent = dijkstra(graph, pacmanVertex, ghostVertex);

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
                    secondReached = false;
                    applyShortedPath = false;
                    printf("\n\n");
                    continue;
                }
            }

            if (strcmp(ghostMovement[i], "down") == 0)
            {
                newY += 1;
                if (isWallCollide(1, newX, newY) == false)
                {
                    ghostY[i] += 1;
                    if (ghostChase[i] == false)
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
                    if (ghostChase[i] == false)
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
                    if (ghostChase[i] == false)
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
                    if (ghostChase[i] == false)
                        checkGhostCoords(i);
                }
                else
                {
                    if (ghostChase[i] == false)
                        changeGhostMovement(i, 0);
                }
            }
            checkGhostLineOfSight(i);
        }

        ghostTimer++;
        glutPostRedisplay(); // Request redisplay
        usleep(10000);
    }
    pthread_exit(NULL);
}
