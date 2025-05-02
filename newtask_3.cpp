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
#include <cmath>
#include <bits/stdc++.h>
#include <GL/glew.h>
using namespace std;

// OpenGL / GLUT Headers
#ifdef __APPLE__
#include <GLUT/glut.h> // Use GLUT framework on macOS
#else
#include <GL/glut.h> // Use standard GLUT location on Linux/Windows
#endif

// --- Global Variables ---

int animationSpeed = 10;
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
void drawCubeWithCheckeredFloor();

struct Sphere
{
    float positionx, positiony, positionz;
    float velocityx, velocityy, velocityz;
    float accelerationx, accelerationy, accelerationz;
    float radius;
    float mass;
};



// Initialize the sphere
Sphere sphere;

/**
 * Initialize OpenGL settings
 * Sets up background color and enables depth testing
 */
void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
    glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
}


const int slices = 40;
const int stacks = 40;
const float pi = 3.14159;
void renderSphere(float radius = 1.0f)
{
    for (int i = 0; i < stacks; ++i)
    {
        float lat0 = pi * (-0.5 + (float)i / stacks);
        float z0 = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = pi * (-0.5 + (float)(i + 1) / stacks);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; ++j)
        {
            float lng = 2 * pi * (float)(j) / slices;
            float x = cos(lng);
            float y = sin(lng);

            // Alternate stripe color by longitude index
            if (j % 2 == 0)
                glColor3f(1.0f, 0.0f, 0.0f); // Red
            else
                glColor3f(0.0f, 1.0f, 0.0f); // Green

            glVertex3f(radius * x * zr0, radius * y * zr0, radius * z0);
            glVertex3f(radius * x * zr1, radius * y * zr1, radius * z1);
        }
        glEnd();
    }
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
    drawCubeWithCheckeredFloor();
    renderSphere();
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
        double caX = centerx - eyex;
        double caY = centery - eyey;
        double caZ = centerz - eyez;

        double crossX = upy * caZ - upz * caY;
        double crossY = upz * caX - upx * caZ;
        double crossZ = upx * caY - upy * caX;

        double crossMag = sqrt(crossX * crossX + crossY * crossY + crossZ * crossZ);
        double tX = crossX / crossMag;
        double tY = crossY / crossMag;
        double tZ = crossZ / crossMag;

        double daX = tX * vinc;
        double daY = tY * vinc;
        double daZ = tZ * vinc;

        centerx = centerx + daX;
        centery = centery + daY;
        centerz = centerz + daZ;
        break;
    }
    case '2':
    {

        double caX = centerx - eyex;
        double caY = centery - eyey;
        double caZ = centerz - eyez;

        double crossX = upy * caZ - upz * caY;
        double crossY = upz * caX - upx * caZ;
        double crossZ = upx * caY - upy * caX;

        double crossMag = sqrt(crossX * crossX + crossY * crossY + crossZ * crossZ);
        double tX = crossX / crossMag;
        double tY = crossY / crossMag;
        double tZ = crossZ / crossMag;

        double daX = tX * vinc;
        double daY = tY * vinc;
        double daZ = tZ * vinc;

        centerx = centerx - daX;
        centery = centery - daY;
        centerz = centerz - daZ;
        break;
    }
    case '3': // Rotate camera upwards
    {
        float ROTATION_ANGLE = 0.03f;
        float viewDirx = centerx - eyex;
        float viewDiry = centery - eyey;
        float viewDirz = centerz - eyez;

        // Normalize the view direction
        float viewLength = sqrt(viewDirx * viewDirx + viewDiry * viewDiry + viewDirz * viewDirz);
        viewDirx /= viewLength;
        viewDiry /= viewLength;
        viewDirz /= viewLength;

        // Normalize the up vector
        float upLength = sqrt(upx * upx + upy * upy + upz * upz);
        upx /= upLength;
        upy /= upLength;
        upz /= upLength;

        // Calculate the right vector (cross product of view direction and up vector)
        float rightx = viewDiry * upz - viewDirz * upy;
        float righty = viewDirz * upx - viewDirx * upz;
        float rightz = viewDirx * upy - viewDiry * upx;

        // Normalize the right vector
        float rightLength = sqrt(rightx * rightx + righty * righty + rightz * rightz);
        rightx /= rightLength;
        righty /= rightLength;
        rightz /= rightLength;

        // Create rotation matrix around the right vector (pitch up)
        float cosAngle = cos(ROTATION_ANGLE);
        float sinAngle = sin(ROTATION_ANGLE);

        // Rotate the view direction
        float newViewDirx = viewDirx * cosAngle + upx * sinAngle;
        float newViewDiry = viewDiry * cosAngle + upy * sinAngle;
        float newViewDirz = viewDirz * cosAngle + upz * sinAngle;

        // Rotate the up vector
        float newUpx = upx * cosAngle - viewDirx * sinAngle;
        float newUpy = upy * cosAngle - viewDiry * sinAngle;
        float newUpz = upz * cosAngle - viewDirz * sinAngle;

        // Update the center position
        centerx = eyex + newViewDirx * viewLength;
        centery = eyey + newViewDiry * viewLength;
        centerz = eyez + newViewDirz * viewLength;

        // Update the up vector
        upx = newUpx;
        upy = newUpy;
        upz = newUpz;
        break;
    }
    case '4': // Rotate camera downwards
    {
        float ROTATION_ANGLE = -0.03f;
        float viewDirx = centerx - eyex;
        float viewDiry = centery - eyey;
        float viewDirz = centerz - eyez;

        // Normalize the view direction
        float viewLength = sqrt(viewDirx * viewDirx + viewDiry * viewDiry + viewDirz * viewDirz);
        viewDirx /= viewLength;
        viewDiry /= viewLength;
        viewDirz /= viewLength;

        // Normalize the up vector
        float upLength = sqrt(upx * upx + upy * upy + upz * upz);
        upx /= upLength;
        upy /= upLength;
        upz /= upLength;

        // Calculate the right vector (cross product of view direction and up vector)
        float rightx = viewDiry * upz - viewDirz * upy;
        float righty = viewDirz * upx - viewDirx * upz;
        float rightz = viewDirx * upy - viewDiry * upx;

        // Normalize the right vector
        float rightLength = sqrt(rightx * rightx + righty * righty + rightz * rightz);
        rightx /= rightLength;
        righty /= rightLength;
        rightz /= rightLength;

        // Create rotation matrix around the right vector (pitch down)
        float cosAngle = cos(ROTATION_ANGLE);
        float sinAngle = sin(ROTATION_ANGLE);

        // Rotate the view direction
        float newViewDirx = viewDirx * cosAngle + upx * sinAngle;
        float newViewDiry = viewDiry * cosAngle + upy * sinAngle;
        float newViewDirz = viewDirz * cosAngle + upz * sinAngle;

        // Rotate the up vector
        float newUpx = upx * cosAngle - viewDirx * sinAngle;
        float newUpy = upy * cosAngle - viewDiry * sinAngle;
        float newUpz = upz * cosAngle - viewDirz * sinAngle;

        // Update the center position
        centerx = eyex + newViewDirx * viewLength;
        centery = eyey + newViewDiry * viewLength;
        centerz = eyez + newViewDirz * viewLength;

        // Update the up vector
        upx = newUpx;
        upy = newUpy;
        upz = newUpz;
        break;
    }
    case '5':
    {

        float TILT_ANGLE = 0.03f; // Positive for clockwise, negative for counter-clockwise

        // Calculate the view direction vector (from eye to center)
        float viewDirx = centerx - eyex;
        float viewDiry = centery - eyey;
        float viewDirz = centerz - eyez;

        // Normalize the view direction (rotation axis)
        float viewLength = sqrt(viewDirx * viewDirx + viewDiry * viewDiry + viewDirz * viewDirz);
        viewDirx /= viewLength;
        viewDiry /= viewLength;
        viewDirz /= viewLength;

        // Normalize the up vector
        float upLength = sqrt(upx * upx + upy * upy + upz * upz);
        upx /= upLength;
        upy /= upLength;
        upz /= upLength;

        // Calculate the right vector (cross product of view direction and up vector)
        float rightx = viewDiry * upz - viewDirz * upy;
        float righty = viewDirz * upx - viewDirx * upz;
        float rightz = viewDirx * upy - viewDiry * upx;

        // Create rotation matrix using Rodrigues' rotation formula
        float cosAngle = cos(TILT_ANGLE);
        float sinAngle = sin(TILT_ANGLE);

        // Rotate the up vector around the view direction axis
        float newUpx = upx * cosAngle + rightx * sinAngle;
        float newUpy = upy * cosAngle + righty * sinAngle;
        float newUpz = upz * cosAngle + rightz * sinAngle;

        // Update the up vector
        upx = newUpx;
        upy = newUpy;
        upz = newUpz;

        // Optional: Re-normalize the up vector
        upLength = sqrt(upx * upx + upy * upy + upz * upz);
        upx /= upLength;
        upy /= upLength;
        upz /= upLength;
        break;
    }
    case '6':
    {

        float TILT_ANGLE = -0.03f; // Positive for clockwise, negative for counter-clockwise

        // Calculate the view direction vector (from eye to center)
        float viewDirx = centerx - eyex;
        float viewDiry = centery - eyey;
        float viewDirz = centerz - eyez;

        // Normalize the view direction (rotation axis)
        float viewLength = sqrt(viewDirx * viewDirx + viewDiry * viewDiry + viewDirz * viewDirz);
        viewDirx /= viewLength;
        viewDiry /= viewLength;
        viewDirz /= viewLength;

        // Normalize the up vector
        float upLength = sqrt(upx * upx + upy * upy + upz * upz);
        upx /= upLength;
        upy /= upLength;
        upz /= upLength;

        // Calculate the right vector (cross product of view direction and up vector)
        float rightx = viewDiry * upz - viewDirz * upy;
        float righty = viewDirz * upx - viewDirx * upz;
        float rightz = viewDirx * upy - viewDiry * upx;

        // Create rotation matrix using Rodrigues' rotation formula
        float cosAngle = cos(TILT_ANGLE);
        float sinAngle = sin(TILT_ANGLE);

        // Rotate the up vector around the view direction axis
        float newUpx = upx * cosAngle + rightx * sinAngle;
        float newUpy = upy * cosAngle + righty * sinAngle;
        float newUpz = upz * cosAngle + rightz * sinAngle;

        // Update the up vector
        upx = newUpx;
        upy = newUpy;
        upz = newUpz;

        // Optional: Re-normalize the up vector
        upLength = sqrt(upx * upx + upy * upy + upz * upz);
        upx /= upLength;
        upy /= upLength;
        upz /= upLength;
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
        // Calculate the vector from the camera to the reference point
        double cx = centerx - eyex;
        double cy = centery - eyey;
        double cz = centerz - eyez;

        // Calculate the cross product of the head vector and the camera-reference point vector
        double leftx = upy * cz - upz * cy;
        double lefty = upz * cx - upx * cz;
        double leftz = upx * cy - upy * cx;

        // Normalize the left vector
        double length = sqrt(leftx * leftx + lefty * lefty + leftz * leftz);
        leftx /= length;
        lefty /= length;
        leftz /= length;

        // Move the camera and reference point in the left direction
        eyex += leftx * v;
        eyey += lefty * v;
        eyez += leftz * v;

        centerx += leftx * v;
        centery += lefty * v;
        centerz += leftz * v;

        break;
    }

    case GLUT_KEY_RIGHT:
    {
        // Calculate the vector from the camera to the reference point
        double cx = centerx - eyex;
        double cy = centery - eyey;
        double cz = centerz - eyez;

        // Calculate the cross product of the head vector and the camera-reference point vector
        double rightx = upy * cz - upz * cy;
        double righty = upz * cx - upx * cz;
        double rightz = upx * cy - upy * cx;

        // Normalize the right vector
        double length = sqrt(rightx * rightx + righty * righty + rightz * rightz);
        rightx /= length;
        righty /= length;
        rightz /= length;

        // Move the camera and reference point in the right direction
        eyex -= rightx * v;
        eyey -= righty * v;
        eyez -= rightz * v;

        centerx -= rightx * v;
        centery -= righty * v;
        centerz -= rightz * v;

        break;
    }

    case GLUT_KEY_UP:
    {
        // Calculate the vector from the camera to the reference point
        double cx = centerx - eyex;
        double cy = centery - eyey;
        double cz = centerz - eyez;

        // Normalize the vector
        double length = sqrt(cx * cx + cy * cy + cz * cz);
        cx /= length;
        cy /= length;
        cz /= length;

        // Move the camera and reference point along the vector by a single unit
        eyex += cx * v;
        eyey += cy * v;
        eyez += cz * v;

        centerx += cx * v;
        centery += cy * v;
        centerz += cz * v;

        break;
    }

    case GLUT_KEY_DOWN:
    {
        // Calculate the vector from the camera to the reference point
        double cx = centerx - eyex;
        double cy = centery - eyey;
        double cz = centerz - eyez;

        // Normalize the vector
        double length = sqrt(cx * cx + cy * cy + cz * cz);
        cx /= length;
        cy /= length;
        cz /= length;

        // Move the camera and reference point along the opposite direction of the vector by a single unit
        eyex -= cx * v;
        eyey -= cy * v;
        eyez -= cz * v;

        centerx -= cx * v;
        centery -= cy * v;
        centerz -= cz * v;

        break;
    }
    case GLUT_KEY_PAGE_UP:
    {
        // Move the camera and reference point along the head vector by a single unit
        eyex += upx * v;
        eyey += upy * v;
        eyez += upz * v;

        centerx += upx * v;
        centery += upy * v;
        centerz += upz * v;

        break;
    }

    case GLUT_KEY_PAGE_DOWN:
    {
        // Move the camera and reference point along the opposite direction of the head vector by a single unit
        eyex -= upx * v;
        eyey -= upy * v;
        eyez -= upz * v;

        centerx -= upx * v;
        centery -= upy * v;
        centerz -= upz * v;

        break;
    }
    }

    glutPostRedisplay(); // Request a screen refresh
}

// Simulate physics
void updatePhysics(float deltaTime)
{
    // Update sphere position and velocity
    sphere.positionx += sphere.velocityx * deltaTime;
    sphere.positiony += sphere.velocityy * deltaTime;
    sphere.positionz += sphere.velocityz * deltaTime;
    sphere.velocityx += sphere.accelerationx * deltaTime;
    sphere.velocityy += sphere.accelerationy * deltaTime;
    sphere.velocityz += sphere.accelerationz * deltaTime;

    // Check for collisions with floor
    if (sphere.positiony - sphere.radius < 0)
    {
        // Collision detected, apply response
        sphere.velocityy = -sphere.velocityy * 0.7; // bounce with damping
        sphere.positiony = 0 + sphere.radius;       // move sphere to floor
    }

    // Add rolling and spinning behavior
    if (sphere.velocityx != 0 || sphere.velocityz != 0)
    {
        // Calculate rolling velocity
        float rollingVelocity = sqrt(sphere.velocityx * sphere.velocityx + sphere.velocityz * sphere.velocityz);

        // Apply rolling friction
        sphere.velocityx *= 0.9;
        sphere.velocityz *= 0.9;

        // Update sphere position based on rolling velocity
        sphere.positionx += sphere.velocityx * deltaTime;
        sphere.positionz += sphere.velocityz * deltaTime;
    }

    // Check if sphere has come to rest
    // if (sphere.velocity.length() < 0.01)
    // {
    //     sphere.velocity = Vec3(0, 0, 0); // set velocity to zero
    // }
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
 * Draws a cube with a checkered floor pattern.
 *
 * The function sets up a 3D scene with a cube of fixed dimensions and colors.
 * It renders a checkered floor beneath the cube using alternating black and
 * white squares. The walls of the cube are colored with a specific color,
 * and the ceiling is colored differently. The cube is centered at the origin.
 */
void drawCubeWithCheckeredFloor()
{
    // Set up the cube's dimensions
    float cubeSize = 10.0f;

    // Set up the colors
    GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat wallColor[] = {0.5f, 0.5f, 1.0f, 1.0f};
    GLfloat ceilingColor[] = {1.0f, 0.5f, 0.5f, 1.0f};

    // Draw the floor
    glBegin(GL_QUADS);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if ((i + j) % 2 == 0)
            {
                glColor4fv(white);
            }
            else
            {
                glColor4fv(black);
            }
            glVertex3f(-cubeSize / 2 + i * cubeSize / 8, -cubeSize / 2, -cubeSize / 2 + j * cubeSize / 8);
            glVertex3f(-cubeSize / 2 + (i + 1) * cubeSize / 8, -cubeSize / 2, -cubeSize / 2 + j * cubeSize / 8);
            glVertex3f(-cubeSize / 2 + (i + 1) * cubeSize / 8, -cubeSize / 2, -cubeSize / 2 + (j + 1) * cubeSize / 8);
            glVertex3f(-cubeSize / 2 + i * cubeSize / 8, -cubeSize / 2, -cubeSize / 2 + (j + 1) * cubeSize / 8);
        }
    }
    glEnd();

    // Draw the walls
    glBegin(GL_QUADS);
    glColor4fv(wallColor);
    glVertex3f(-cubeSize / 2, -cubeSize / 2, -cubeSize / 2);
    glVertex3f(cubeSize / 2, -cubeSize / 2, -cubeSize / 2);
    glVertex3f(cubeSize / 2, cubeSize / 2, -cubeSize / 2);
    glVertex3f(-cubeSize / 2, cubeSize / 2, -cubeSize / 2);

    glVertex3f(-cubeSize / 2, -cubeSize / 2, cubeSize / 2);
    glVertex3f(cubeSize / 2, -cubeSize / 2, cubeSize / 2);
    glVertex3f(cubeSize / 2, cubeSize / 2, cubeSize / 2);
    glVertex3f(-cubeSize / 2, cubeSize / 2, cubeSize / 2);

    glVertex3f(-cubeSize / 2, -cubeSize / 2, -cubeSize / 2);
    glVertex3f(-cubeSize / 2, -cubeSize / 2, cubeSize / 2);
    glVertex3f(-cubeSize / 2, cubeSize / 2, cubeSize / 2);
    glVertex3f(-cubeSize / 2, cubeSize / 2, -cubeSize / 2);

    glVertex3f(cubeSize / 2, -cubeSize / 2, -cubeSize / 2);
    glVertex3f(cubeSize / 2, -cubeSize / 2, cubeSize / 2);
    glVertex3f(cubeSize / 2, cubeSize / 2, cubeSize / 2);
    glVertex3f(cubeSize / 2, cubeSize / 2, -cubeSize / 2);
    glEnd();

    // Draw the ceiling
    glBegin(GL_QUADS);
    glColor4fv(ceilingColor);
    glVertex3f(-cubeSize / 2, cubeSize / 2, -cubeSize / 2);
    glVertex3f(cubeSize / 2, cubeSize / 2, -cubeSize / 2);
    glVertex3f(cubeSize / 2, cubeSize / 2, cubeSize / 2);
    glVertex3f(-cubeSize / 2, cubeSize / 2, cubeSize / 2);
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
 * Timer function for animation.
 * This demonstrates the use of a timer instead of idle function.
 * Timer functions provide better control over animation speed.
 *
 * @param value Value passed to the timer function (not used here)
 */
void timerFunction(int value)
{
    // Update animation values
    cout << "Timer function called" << endl;
    time_t currentTime = time(NULL);
    struct tm *timeInfo = localtime(&currentTime);
    updatePhysics((float)(animationSpeed) / 1000.0f);

    // Request a redisplay
    glutPostRedisplay();

    // Register the timer again
    glutTimerFunc(animationSpeed, timerFunction, 0);
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
    glutTimerFunc(animationSpeed, timerFunction, 0);

    // Initialize OpenGL settings
    initGL();
    sphere.positionx = 0.5f;
    sphere.positiony = 0;
    sphere.positionz = 0.5f;
    sphere.velocityx = 10.0f;
    sphere.velocityy = 10.0f;
    sphere.velocityz = 10.0f;
    sphere.accelerationx = 0;
    sphere.accelerationy = -9.8;
    sphere.accelerationz = 0;
    sphere.radius = 0.5;
    sphere.mass = 1.0;

    // Enter the GLUT event loop
    glutMainLoop();

    return 0;
}