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

/// @brief the force which is applied to the sphere towards land
float gravity = -9.8f;
/// @brief the coefficient of friction when it rolls on the ground
float friction = 0.98f; 
/// @brief  the percentage of force that helps the ball to bounce back from the floor after a collision
float restitution = 0.8f;
/// @brief defines the size of the cube with checkered floor
float cubeSize = 20.0f;
/// @brief the value of pi used in calculations
float pi = 3.14159f;
/// @brief increase of ball velocity per plus key press
float increasePerPlus = 10.0f;
GLUquadricObj *quadric;

float originalSphereRadius = 0.5f;
float originalSphereMass = 1.0f;
float originalSphereVelocity[] = {1.0f, 2.0f, 2.0f};
float originalSpherePosition[] = {5.0f, 5.0f, 5.0f};
float originalSphereAngularVelocity[] = {0.0f, 0.0f, 0.0f};
float originalSphereRotationAngle[] = {0.0f, 0.0f, 0.0f};
float originalSphereColor[] = {0.8f, 0.2f, 0.2f};

// --- Global Variables ---

int animationSpeed = 10;
// Camera position and orientation
GLfloat eyex = 4, eyey = 4, eyez = 4;          // Camera position coordinates
GLfloat centerx = 0, centery = 0, centerz = 0; // Look-at point coordinates
GLfloat upx = 0, upy = 1, upz = 0;             // Up vector coordinates

float movementSpeed = 0.3f;
float rotationSpeed = 0.1f;

// Object visibility flags
bool isAxes = true;     // Toggle for coordinate axes
bool isCube = false;    // Toggle for cube
bool isPyramid = false; // Toggle for pyramid
bool paused = true;

float ballSpeedOriginal = 0.1f;

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

/**
 * Initialize OpenGL settings
 * Sets up background color and enables depth testing
 */
void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
    glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
}

/// @brief struct which represents a sphere
typedef struct
{
    float radius;
    float mass;
    float position[3];
    float velocity[3];
    float angularVelocity[3];
    float rotationAngle[3];
    float color[3];
} Sphere;

Sphere sphere;

void initSphere()
{
    sphere.radius = originalSphereRadius;
    sphere.mass = originalSphereMass;
    sphere.position[0] = originalSpherePosition[0];
    sphere.position[1] = originalSpherePosition[1];
    sphere.position[2] = originalSpherePosition[2];
    sphere.velocity[0] = originalSphereVelocity[0];
    sphere.velocity[1] = originalSphereVelocity[1];
    sphere.velocity[2] = originalSphereVelocity[2];
    sphere.angularVelocity[0] = originalSphereAngularVelocity[0];
    sphere.angularVelocity[1] = originalSphereAngularVelocity[1];
    sphere.angularVelocity[2] = originalSphereAngularVelocity[2];
    sphere.rotationAngle[0] = originalSphereRotationAngle[0];
    sphere.rotationAngle[1] = originalSphereRotationAngle[1];
    sphere.rotationAngle[2] = originalSphereRotationAngle[2];
    sphere.color[0] = originalSphereColor[0];
    sphere.color[1] = originalSphereColor[1];
    sphere.color[2] =originalSphereColor[2];

    quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
}

void stripeColor(GLfloat* color, float angle) {
    int stripe = (int)(angle / (2 * pi / 6)) % 2;
    if (stripe == 0)
    {
        color[0] = 0.8f; // Red
        color[1] = 0.2f;
        color[2] = 0.2f;
    }
    else
    {
        color[0] = 0.2f; // Green
        color[1] = 0.8f;
        color[2] = 0.2f;
    }
}

void drawSphere() {
    glPushMatrix();
    glTranslatef(sphere.position[0], sphere.position[1], sphere.position[2]);
    glRotatef(sphere.rotationAngle[0], 1.0f, 0.0f, 0.0f);
    glRotatef(sphere.rotationAngle[1], 0.0f, 1.0f, 0.0f);
    glRotatef(sphere.rotationAngle[2], 0.0f, 0.0f, 1.0f);


    // glColor3f(sphere.color[0], sphere.color[1], sphere.color[2]);

    glEnable(GL_COLOR_MATERIAL);
    gluQuadricCallback(quadric, GLU_ERROR, NULL);
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricTexture(quadric, GL_TRUE);

    // Set up the color callback
    GLfloat color[3];
    glBegin(GL_TRIANGLE_STRIP);
    for (float phi = 0; phi < pi; phi += pi / 20)
    {
        for (float theta = 0; theta <= 2 * pi; theta += pi / 20)
        {
            stripeColor(color, theta);
            glColor3fv(color);
            glVertex3f(
                sphere.radius * sin(phi) * cos(theta),
                sphere.radius * cos(phi),
                sphere.radius * sin(phi) * sin(theta));

            stripeColor(color, theta);
            glColor3fv(color);
            glVertex3f(
                sphere.radius * sin(phi + pi / 20) * cos(theta),
                sphere.radius * cos(phi + pi / 20),
                sphere.radius * sin(phi + pi / 20) * sin(theta));
        }
    }
    glEnd();

    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
}

void checkCollisions()
{
    const float rollMatchFactor = 0.5f;  // How quickly to align angular velocity to ideal rolling
    const float maxAngularSpeed = 50.0f; // Optional angular velocity clamp

    // Floor collision (Y-axis)
    if (sphere.position[1] - sphere.radius <= 0.0f)
    {
        sphere.position[1] = sphere.radius; // Prevent sinking
        sphere.velocity[1] = -sphere.velocity[1] * restitution;

        // Apply friction to horizontal velocities
        sphere.velocity[0] *= friction;
        sphere.velocity[2] *= friction;

        // Calculate ideal angular velocity for rolling
        float idealAngularX = sphere.velocity[2] / sphere.radius;
        float idealAngularZ = -sphere.velocity[0] / sphere.radius;

        // Smoothly transition to rolling angular velocity
        sphere.angularVelocity[0] += (idealAngularX - sphere.angularVelocity[0]) * rollMatchFactor;
        sphere.angularVelocity[2] += (idealAngularZ - sphere.angularVelocity[2]) * rollMatchFactor;
    }

    // Wall collisions (X-axis)
    if (sphere.position[0] - sphere.radius <= 0.0f)
    {
        sphere.position[0] = sphere.radius;
        sphere.velocity[0] = -sphere.velocity[0] * restitution;

        // Smoothly adjust Y-axis rotation
        float idealAngularY = -sphere.velocity[2] / sphere.radius;
        sphere.angularVelocity[1] += (idealAngularY - sphere.angularVelocity[1]) * rollMatchFactor;
    }
    if (sphere.position[0] + sphere.radius >= cubeSize)
    {
        sphere.position[0] = cubeSize - sphere.radius;
        sphere.velocity[0] = -sphere.velocity[0] * restitution;

        float idealAngularY = -sphere.velocity[2] / sphere.radius;
        sphere.angularVelocity[1] += (idealAngularY - sphere.angularVelocity[1]) * rollMatchFactor;
    }

    // Wall collisions (Z-axis)
    if (sphere.position[2] - sphere.radius <= 0.0f)
    {
        sphere.position[2] = sphere.radius;
        sphere.velocity[2] = -sphere.velocity[2] * restitution;

        float idealAngularY = sphere.velocity[0] / sphere.radius;
        sphere.angularVelocity[1] += (idealAngularY - sphere.angularVelocity[1]) * rollMatchFactor;
    }
    if (sphere.position[2] + sphere.radius >= cubeSize)
    {
        sphere.position[2] = cubeSize - sphere.radius;
        sphere.velocity[2] = -sphere.velocity[2] * restitution;

        float idealAngularY = sphere.velocity[0] / sphere.radius;
        sphere.angularVelocity[1] += (idealAngularY - sphere.angularVelocity[1]) * rollMatchFactor;
    }

    // Ceiling collision (Y-axis top)
    if (sphere.position[1] + sphere.radius >= cubeSize)
    {
        sphere.position[1] = cubeSize - sphere.radius;
        sphere.velocity[1] = -sphere.velocity[1] * restitution;
    }

    // Optional: Clamp angular velocity
    for (int i = 0; i < 3; ++i)
    {
        if (sphere.angularVelocity[i] > maxAngularSpeed)
            sphere.angularVelocity[i] = maxAngularSpeed;
        else if (sphere.angularVelocity[i] < -maxAngularSpeed)
            sphere.angularVelocity[i] = -maxAngularSpeed;
    }
}

/// @brief updatePhysics of the ball
/// @param deltaTime it is the millisecond time after when the function is called
void updatePhysics(int deltaTime)
{
    float dt = deltaTime / 1000.0f; // Convert to seconds

    // Apply gravity
    sphere.velocity[1] += gravity * dt;

    // Update position
    sphere.position[0] += sphere.velocity[0] * dt;
    sphere.position[1] += sphere.velocity[1] * dt;
    sphere.position[2] += sphere.velocity[2] * dt;

    // Update rotation
    sphere.rotationAngle[0] += sphere.angularVelocity[0] * dt * 180.0f / pi;
    sphere.rotationAngle[1] += sphere.angularVelocity[1] * dt * 180.0f / pi;
    sphere.rotationAngle[2] += sphere.angularVelocity[2] * dt * 180.0f / pi;

    // Dampen angular velocity
    sphere.angularVelocity[0] *= friction;
    sphere.angularVelocity[1] *= friction;
    sphere.angularVelocity[2] *= friction;

    // Check for collisions
    cout << "Sphere position: (" << sphere.position[0] << ", " << sphere.position[1] << ", " << sphere.position[2] << ")" << endl;
    checkCollisions();
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
    drawSphere();
    if (isAxes)
        drawAxes();

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

        double daX = tX * rotationSpeed;
        double daY = tY * rotationSpeed;
        double daZ = tZ * rotationSpeed;

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

        double daX = tX * rotationSpeed;
        double daY = tY * rotationSpeed;
        double daZ = tZ * rotationSpeed;

        centerx = centerx - daX;
        centery = centery - daY;
        centerz = centerz - daZ;
        break;
    }
    case '3': // Rotate camera upwards
    {
        float ROTATION_ANGLE = rotationSpeed;
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
        float ROTATION_ANGLE = -rotationSpeed;
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

        float TILT_ANGLE = rotationSpeed; // Positive for clockwise, negative for counter-clockwise

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

        float TILT_ANGLE = -rotationSpeed; // Positive for clockwise, negative for counter-clockwise

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
        eyey += movementSpeed;
        break;
    }
    case 's':
    {
        // Move camera down
        eyey -= movementSpeed;
        break;
    }

    case 't':
        centerz += movementSpeed;
        break; // Move look-at point forward
    case 'y':
        centerz -= movementSpeed;
        break; // Move look-at point backward
    case '*':
        movementSpeed += 0.1f;
        rotationSpeed += 0.1f;
        break;
    case '/':
        movementSpeed -= 0.1f;
        rotationSpeed -= 0.1f;
        break;
    case '+':
        sphere.velocity[0] += increasePerPlus;
        sphere.velocity[1] += increasePerPlus;
        sphere.velocity[2] += increasePerPlus;
        break;
    case '-':
        sphere.velocity[0] -= increasePerPlus;
        sphere.velocity[1] -= increasePerPlus;
        sphere.velocity[2] -= increasePerPlus;
        break;
    case ' ':
        paused = !paused;
        break;
    case 'r':
        initSphere();
        paused = true;
        break;

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
        eyex += leftx * movementSpeed;
        eyey += lefty * movementSpeed;
        eyez += leftz * movementSpeed;

        centerx += leftx * movementSpeed;
        centery += lefty * movementSpeed;
        centerz += leftz * movementSpeed;

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
        eyex -= rightx * movementSpeed;
        eyey -= righty * movementSpeed;
        eyez -= rightz * movementSpeed;

        centerx -= rightx * movementSpeed;
        centery -= righty * movementSpeed;
        centerz -= rightz * movementSpeed;

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
        eyex += cx * movementSpeed;
        eyey += cy * movementSpeed;
        eyez += cz * movementSpeed;

        centerx += cx * movementSpeed;
        centery += cy * movementSpeed;
        centerz += cz * movementSpeed;

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
        eyex -= cx * movementSpeed;
        eyey -= cy * movementSpeed;
        eyez -= cz * movementSpeed;

        centerx -= cx * movementSpeed;
        centery -= cy * movementSpeed;
        centerz -= cz * movementSpeed;

        break;
    }
    case GLUT_KEY_PAGE_UP:
    {
        // Move the camera and reference point along the head vector by a single unit
        eyex += upx * movementSpeed;
        eyey += upy * movementSpeed;
        eyez += upz * movementSpeed;

        centerx += upx * movementSpeed;
        centery += upy * movementSpeed;
        centerz += upz * movementSpeed;

        break;
    }

    case GLUT_KEY_PAGE_DOWN:
    {
        // Move the camera and reference point along the opposite direction of the head vector by a single unit
        eyex -= upx * movementSpeed;
        eyey -= upy * movementSpeed;
        eyez -= upz * movementSpeed;

        centerx -= upx * movementSpeed;
        centery -= upy * movementSpeed;
        centerz -= upz * movementSpeed;

        break;
    }
    }

    glutPostRedisplay(); // Request a screen refresh
}

// Simulate physics

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

void drawCheckeredFloor(float size, int tiles)
{
    // Set up the colors
    GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};

    float tileSize = size / tiles;

    // Draw the floor on XZ plane starting from (0,0,0)
    glBegin(GL_QUADS);
    for (int x = 0; x < tiles; x++)
    {
        for (int z = 0; z < tiles; z++)
        {
            if ((x + z) % 2 == 0)
            {
                glColor4fv(white);
            }
            else
            {
                glColor4fv(black);
            }

            glVertex3f(x * tileSize, 0.0f, z * tileSize);
            glVertex3f((x + 1) * tileSize, 0.0f, z * tileSize);
            glVertex3f((x + 1) * tileSize, 0.0f, (z + 1) * tileSize);
            glVertex3f(x * tileSize, 0.0f, (z + 1) * tileSize);
        }
    }
    glEnd();
}

void drawCubeWithCheckeredFloor()
{
    // Set up the cube's dimensions

    int tiles = cubeSize;
    // Number of tiles in each direction

    // Set up the colors
    GLfloat wallColor[] = {0.5f, 1.0f, 0.5f, 1.0f};
    GLfloat ceilingColor[] = {0.5f, 0.5f, 0.5f, 1.0f};

    // Draw the checkered floor starting from origin
    drawCheckeredFloor(cubeSize, tiles);

    // Draw the walls
    glBegin(GL_QUADS);
    glColor4fv(wallColor);
    // Front wall
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(cubeSize, 0.0f, 0.0f);
    glVertex3f(cubeSize, cubeSize, 0.0f);
    glVertex3f(0.0f, cubeSize, 0.0f);

    // Back wall
    glVertex3f(0.0f, 0.0f, cubeSize);
    glVertex3f(cubeSize, 0.0f, cubeSize);
    glVertex3f(cubeSize, cubeSize, cubeSize);
    glVertex3f(0.0f, cubeSize, cubeSize);

    // Left wall
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, cubeSize);
    glVertex3f(0.0f, cubeSize, cubeSize);
    glVertex3f(0.0f, cubeSize, 0.0f);

    // Right wall
    glVertex3f(cubeSize, 0.0f, 0.0f);
    glVertex3f(cubeSize, 0.0f, cubeSize);
    glVertex3f(cubeSize, cubeSize, cubeSize);
    glVertex3f(cubeSize, cubeSize, 0.0f);
    glEnd();

    // Draw the ceiling
    glBegin(GL_QUADS);
    glColor4fv(ceilingColor);
    glVertex3f(0.0f, cubeSize, 0.0f);
    glVertex3f(cubeSize, cubeSize, 0.0f);
    glVertex3f(cubeSize, cubeSize, cubeSize);
    glVertex3f(0.0f, cubeSize, cubeSize);
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
    // cout << "Timer function called" << endl;
    time_t currentTime = time(NULL);
    struct tm *timeInfo = localtime(&currentTime);
    if(paused == false) {updatePhysics((float)(animationSpeed));}
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

    glEnable(GL_DEPTH_TEST);
    // glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glShadeModel(GL_SMOOTH);
    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);
    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshapeListener);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutTimerFunc(animationSpeed, timerFunction, 0);
    initSphere();
    // Initialize OpenGL settings
    initGL();

    // Enter the GLUT event loop
    glutMainLoop();

    return 0;
}