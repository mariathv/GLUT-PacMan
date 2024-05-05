#include <GL/glut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Global variables for texture, position, and size
GLuint textureID;
float x = 50.0f;
float y = 50.0f;
float side = 40.0f; // Adjusted the size for clarity
char * pacmantext = "imgs/pacman/right.png";

// Function to load texture from file
void loadTexture(const char* filename) {
    int width, height, numChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &numChannels, 0);
    if (!data) {
        printf("Failed to load image: %s\n", stbi_failure_reason());
        exit(1);
    }

    // Generate texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Specify texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Free image data
    stbi_image_free(data);
}

// Function to display the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Draw a quad with the loaded texture
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(x, y);
    glTexCoord2f(1, 0); glVertex2f(x + side, y);
    glTexCoord2f(1, 1); glVertex2f(x + side, y + (side * 1.0f)); // Adjusted the height of the quad
    glTexCoord2f(0, 1); glVertex2f(x, y + (side * 1.0f)); // Adjusted the height of the quad
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

// Function to handle keyboard input
void keyboard(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            x += 10;
            pacmantext = "imgs/pacman/right.png";
            break;
        case GLUT_KEY_LEFT:
            x -= 10;
            pacmantext = "imgs/pacman/left.png";
            break;
        case GLUT_KEY_UP:
            y -= 10;
            pacmantext = "imgs/pacman/up.png";
            break;
        case GLUT_KEY_DOWN:
            y += 10;
            pacmantext = "imgs/pacman/down.png";
            break;
    }
    // Reload the texture with the new filename
    loadTexture(pacmantext);
    glutPostRedisplay();
}
// Function to initialize OpenGL settings
void initOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 600, 800, 0);
    glMatrixMode(GL_MODELVIEW);

    // Load the image texture
    printf("%s", pacmantext);
    loadTexture(pacmantext);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Setup Test");
    initOpenGL();
    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);
    glutMainLoop();
    return 0;
}
