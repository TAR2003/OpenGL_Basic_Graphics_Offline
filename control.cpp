/**
 * OpenGL 3D Drawing Demo
 *
 * This program demonstrates basic 3D rendering with OpenGL and GLUT including:
 * - Camera positioning with gluLookAt
 * - Drawing 3D shapes (cube and pyramid)
 * - Keyboard navigation for camera control
 * - Perspective projection
 * - Object toggling
 */

// --- Includes ---
// Standard Headers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// OpenGL / GLUT Headers
#ifdef __APPLE__
#include <GLUT/glut.h> // Use GLUT framework on macOS
#else
#include <GL/glut.h> // Use standard GLUT location on Linux/Windows
#endif

// --- Global Variables ---
// Camera position and orientation
GLfloat eyex = 4, eyey = 4, eyez = 4;          // Camera position coordinates
GLfloat centerx = 0, centery = 0, centerz = 0; // Look-at point coordinates
GLfloat upx = 0, upy = 1, upz = 0;             // Up vector coordinates

// Object visibility flags
bool isAxes = true;     // Toggle for coordinate axes
bool isCube = false;    // Toggle for cube
bool isPyramid = false; // Toggle for pyramid

// --- Function Declarations ---
void initGL();
void display();
void reshapeListener(GLsizei width, GLsizei height);
void keyboardListener(unsigned char key, int x, int y);
void specialKeyListener(int key, int x, int y);
void drawAxes();
void drawCube();
void drawPyramid();

/**
 * Initialize OpenGL settings
 * Sets up background color and enables depth testing
 */
void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
    glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
}

/**
 * Main display function
 * Sets up the camera and renders visible objects
 */
void display()
{
    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up the model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Position camera using the eye, center and up vectors
    gluLookAt(eyex, eyey, eyez,          // Camera position
              centerx, centery, centerz, // Look-at point
              upx, upy, upz);            // Up vector

    // Draw objects based on visibility flags
    if (isAxes)
        drawAxes();
    if (isCube)
        drawCube();
    if (isPyramid)
        drawPyramid();

    // Swap buffers (double buffering)
    glutSwapBuffers();
}

/**
 * Window reshape callback
 * Handles window resizing and maintains aspect ratio
 */
void reshapeListener(GLsizei width, GLsizei height)
{
    // Prevent division by zero
    if (height == 0)
        height = 1;

    // Calculate aspect ratio
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set viewport to cover entire window
    glViewport(0, 0, width, height);

    // Set up perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 45-degree field of view, aspect ratio, near and far clipping planes
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/**
 * Keyboard input handler for standard keys
 * Manages camera position, object visibility, and program exit
 */
void keyboardListener(unsigned char key, int x, int y)
{
    float v = 0.1;      // Movement increment
    double vinc = 0.25; // Movement increment

    // Calculate view direction vector
    double lx = centerx - eyex;
    double lz = centerz - eyez;
    double s;

    switch (key)
    {
    // --- Camera Position Controls (eye coordinates) ---
    case '1':
    {
        double lx = centerx - eyex;
        double lz = centerz - eyez;
        eyex = centerx + lx * cos(vinc) - lz * sin(vinc);
        eyez = centerz + lx * sin(vinc) + lz * cos(vinc);
        break;
    }
    case '2':
    {
        lx = centerx - eyex;
        lz = centerz - eyez;
        eyex = centerx + lx * cos(-vinc) - lz * sin(-vinc);
        eyez = centerz + lx * sin(-vinc) + lz * cos(-vinc);
        break;
    }
    case '3':
    {
        eyey += vinc;
        break;
    }
    case '4':
    {
        eyey -= vinc;
        break;
    }
    case '5':
    {

        lx = centerx - eyex;
        lz = centerz - eyez;
        eyex = centerx + lz * sin(vinc) + lx * cos(vinc);
        eyez = centerz - lz * cos(vinc) + lx * sin(vinc);
        break;
    }
    case '6':
    {
        lx = centerx - eyex;
        lz = centerz - eyez;
        eyex = centerx + lz * sin(-vinc) + lx * cos(-vinc);
        eyez = centerz - lz * cos(-vinc) + lx * sin(-vinc);
        break;
    }

        // --- Look-at Point Controls (center coordinates) ---

    case 'w':
    {
        // Move camera up
        eyey += v;
        break;
    }
    case 's':
    {
        // Move camera down
        eyey -= v;
        break;
    }

    case 't':
        centerz += v;
        break; // Move look-at point forward
    case 'y':
        centerz -= v;
        break; // Move look-at point backward

    // --- Object Visibility Toggles ---
    case 'a':
        isAxes = !isAxes;
        break; // Toggle axes
    case 'c':
        isCube = !isCube;
        break; // Toggle cube
    case 'p':
        isPyramid = !isPyramid;
        break; // Toggle pyramid

    // --- Program Control ---
    case 27:
        exit(0);
        break; // ESC key: exit program
    }

    glutPostRedisplay(); // Request a screen refresh
}

/**
 * Special key input handler (arrow keys, function keys)
 * Provides camera orbit functionality
 */
void specialKeyListener(int key, int x, int y)
{
    double v = 0.25; // Movement increment

    // Calculate view direction vector
    double lx = centerx - eyex;
    double lz = centerz - eyez;
    double s;

    switch (key)
    {
    case GLUT_KEY_LEFT:
    {
        eyex -= v;
        break; // Move eye left
    }

    case GLUT_KEY_RIGHT:
    {
        eyex += v;
        break; // Move eye right
    }

    case GLUT_KEY_UP:
    {
        eyez += v;
        break; // Move eye forward
    }

    case GLUT_KEY_DOWN:
    {
        eyez -= v;
        break; // Move eye downward
    }
    case GLUT_KEY_PAGE_UP:
    {
        eyey += v;
        break; // Move eye up
    }
    case GLUT_KEY_PAGE_DOWN:
    {
        eyey -= v;
        break; // Move eye down
    }
    }

    glutPostRedisplay(); // Request a screen refresh
}

/**
 * Draw coordinate axes
 * X axis: red, Y axis: green, Z axis: blue
 */
void drawAxes()
{
    glLineWidth(3); // Set line thickness

    glBegin(GL_LINES);

    // X axis (red)
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    // Y axis (green)
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    // Z axis (blue)
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();
}

/**
 * Draw a colored cube centered at the origin
 * Each face has a different color
 */
void drawCube()
{
    glBegin(GL_QUADS);

    // Top face (y = 1.0f) - Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // Bottom face (y = -1.0f) - Orange
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Front face  (z = 1.0f) - Red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f) - Yellow
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Left face (x = -1.0f) - Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Right face (x = 1.0f) - Magenta
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glEnd();
}

/**
 * Draw a pyramid with color gradients
 * Base at y=-1, apex at y=1
 */
void drawPyramid()
{
    glBegin(GL_TRIANGLES);

    // Front face - Red to green to blue gradient
    glColor3f(1.0f, 0.0f, 0.0f); // Red (apex)
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Green (front-left)
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 1.0f); // Blue (front-right)
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Right face - Red to blue to green gradient
    glColor3f(1.0f, 0.0f, 0.0f); // Red (apex)
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f); // Blue (front-right)
    glVertex3f(1.0f, -1.0f, 1.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Green (back-right)
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Back face - Red to green to blue gradient
    glColor3f(1.0f, 0.0f, 0.0f); // Red (apex)
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Green (back-right)
    glVertex3f(1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 0.0f, 1.0f); // Blue (back-left)
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // Left face - Red to blue to green gradient
    glColor3f(1.0f, 0.0f, 0.0f); // Red (apex)
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f); // Blue (back-left)
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Green (front-left)
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();
}

/**
 * Main function: Program entry point
 */
int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);

    // Configure display mode and window
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 640);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("OpenGL 3D Drawing");

    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshapeListener);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    // Initialize OpenGL settings
    initGL();

    // Enter the GLUT event loop
    glutMainLoop();

    return 0;
}