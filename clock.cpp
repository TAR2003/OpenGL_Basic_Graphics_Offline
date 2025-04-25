/**
 It is a analog clock with hours, minutes and seconds.

 */

// Platform-specific includes
#ifdef __linux__
#include <GL/glut.h> // For Linux systems
#elif defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include <GL/glut.h> // For Windows systems
#elif defined(__APPLE__)
#include <GLUT/glut.h> // For macOS systems
#else
#include <GL/glut.h> // Default fallback
#endif

#include <cstdio>
#include <cmath>
#include <time.h>
#include <bits/stdc++.h>
using namespace std;

int animationSpeed = 1000;
float hourReading = 0.2f;
float minuteReading = 0.05f;
int hour = -1;
int minute = -1;
int second = -1;
float hourHandLength = 0.5f;
float minuteHandLength = 0.7f;
float secondHandLength = 0.8f;

// Mouse tracking
int lastMouseX = 0;
int lastMouseY = 0;
bool mouseLeftDown = false; // Track if left mouse button is pressed

/**
 * Initializes OpenGL settings.
 * Called once at the beginning.
 */
void init()
{
    printf("Initialization complete.\n");

    // Set the background (clear) color to dark blue
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

    // Enable depth testing for proper 3D rendering
    glEnable(GL_DEPTH_TEST);
}

/// @brief drawCircle function draws the circle on the screen with the center point at the origin
/// @param radius the radius of the circle
/// @param numSegments num of points we want to draw as a polygon to represent the circle
void drawCircle(double radius, int numSegments)
{
    // Draw the border (white)
    glColor3f(1.0f, 1.0f, 1.0f); // White
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numSegments; i++)
    {
        float angle = i * 3.14159 / 180;
        glVertex2f(radius * cos(angle), radius * sin(angle));
    }
    glEnd();

    float previousPointSize;
    glGetFloatv(GL_POINT_SIZE, &previousPointSize); // Save the previous point size value

    glPointSize(10.0f); // Set the point size to 5.0f for the operation

    // Draw the center point (white)
    glColor3f(1.0f, 1.0f, 1.0f); // White
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();

    glPointSize(previousPointSize); // Restore the previous point size value

    // Draw the inside (blue)
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glBegin(GL_POLYGON);
    for (int i = 0; i < numSegments; i++)
    {
        float angle = i * 3.14159 / 180;
        glVertex2f(radius * cos(angle), radius * sin(angle));
    }
    glEnd();
}

/// @brief drawHourReadingLines function draws the hour reading lines on the clock
void drawHourReadingLines()
{
    int hours = 12;
    for (int i = 0; i < hours; i++)
    {
        float angle = i * 30 * 3.14159 / 180;
        float x1 = (1.0f - hourReading) * cos(angle);
        float y1 = (1.0f - hourReading) * sin(angle);
        float x2 = 1.0 * cos(angle);
        float y2 = 1.0 * sin(angle);
        glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f); // White
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }
}
/// @brief drawMinuteReadingLines function draws the minute reading lines on the clock
void drawMinuteReadingLines()
{
    int minutes = 60;
    for (int i = 0; i < minutes; i++)
    {
        float angle = i * 6 * 3.14159 / 180;
        float x1 = (1.0f - minuteReading) * cos(angle);
        float y1 = (1.0f - minuteReading) * sin(angle);
        float x2 = 1.0 * cos(angle);
        float y2 = 1.0 * sin(angle);
        glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f); // White
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }
}

/// @brief this function draws the hour hand on the clock
void drawHourHand()
{
    if (hour == -1)
        return;
    glLineWidth(10.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex2f(0.0f, 0.0f);      // Center of the clock
    float hourAngle = -(hour % 12 + minute / 60.0f) * 30.0f + 90.0f; // calculating the angle in degrees, taking the (-) as the default angle calculation would be anti clockwise
    float x = hourHandLength * cos(hourAngle * 3.14159f / 180.0f); // now changing the angle to radians from degrees
    float y = hourHandLength * sin(hourAngle * 3.14159f / 180.0f); // now changing the angle to radians from degrees

    glVertex2f(x, y); // End point of the hour hand
    glEnd();
    glLineWidth(1.0f);
}

/// @brief this function draws the minute hand on the clock
void drawMinuteHand()
{
    if (minute == -1)
        return;
    glLineWidth(5.0f);
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex2f(0.0f, 0.0f);      // Center of the clock
    float hourAngle = -(minute + second / 60.0f) * 6.0f + 90.0f; // calculating the angle in degrees taking the (-) as the default angle calculation would be anti clockwise
    float x = minuteHandLength * cos(hourAngle * 3.14159f / 180.0f); // now changing the angle to radians from degrees
    float y = minuteHandLength * sin(hourAngle * 3.14159f / 180.0f); // now changing the angle to radians from degrees

    glVertex2f(x, y); // End point of the minute hand
    glEnd();
    glLineWidth(1.0f);
}

/// @brief this function draws the second hand on the clock
void drawSecondHand()
{
    if (second == -1)
        return;
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 1.0f); // Orange
    glVertex2f(0.0f, 0.0f);      // Center of the clock
    float hourAngle = -second * 6.0f + 90.0f; // calculating the angle in degrees taking the (-) as the default angle calculation would be anti clockwise
    float x = secondHandLength * cos(hourAngle * 3.14159f / 180.0f); // now changing the angle to radians from degrees
    float y = secondHandLength * sin(hourAngle * 3.14159f / 180.0f); // now changing the angle to radians from degrees

    glVertex2f(x, y); // End point of the second hand
    glEnd();
    glLineWidth(1.0f);
}

/// @brief drawClock function draws the clock on the screen
void drawClock()
{
    drawHourHand();
    drawMinuteHand();
    drawSecondHand();
    drawHourReadingLines();
    drawMinuteReadingLines();
    drawCircle(1, 360);
}

/**
 * The display callback function.
 * This is where rendering happens. Called whenever the window needs to be redrawn.
 */
void display()
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Set the camera position (simple 2D view for this example)
    gluLookAt(
        0.0, 0.0, 3.0, // Eye position
        0.0, 0.0, 0.0, // Look at point
        0.0, 1.0, 0.0  // Up vector
    );

    glPushMatrix();
    {
        drawClock();
    }
    glPopMatrix();

    // Display the result using double buffering
    glutSwapBuffers(); // necessary for showing the result
}

/**
 * Called when the window is resized.
 *
 * @param width The new width of the window
 * @param height The new height of the window
 */
void reshape(int width, int height)
{
    // Prevent division by zero
    if (height == 0)
        height = 1;

    // Set the viewport to the entire window
    glViewport(0, 0, width, height);

    // Set the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set perspective projection
    float aspectRatio = (float)width / (float)height;
    gluPerspective(45.0f, aspectRatio, 0.1f, 100.0f);

    // Switch back to modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    printf("Window resized to %d x %d\n", width, height);
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
    hour = timeInfo->tm_hour;
    minute = timeInfo->tm_min;
    second = timeInfo->tm_sec;
    hour %= 12;
    cout << hour << ":" << minute << ":" << second << endl;

    // Request a redisplay
    glutPostRedisplay();

    // Register the timer again
    glutTimerFunc(animationSpeed, timerFunction, 0);
}

/**
 * Main function: Entry point of the program.
 */
int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);

    // Set initial window size and position
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);

    // Set display mode: RGB color, double buffering, and depth buffering
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    // Create a window with the title
    glutCreateWindow("Animated Clock");

    // Register callback functions
    glutDisplayFunc(display);            // Called when screen needs to be redrawn
    glutReshapeFunc(reshape);            // Called when window is resized

    // Register idle function for continuous animation
    // glutIdleFunc(idleFunction); // Uncomment to use idle function for animation

    // Use timer function instead of idle function for better control of animation speed
    glutTimerFunc(animationSpeed, timerFunction, 0);

    // Perform initial setup
    init();

    // Start the GLUT event loop
    printf("Starting GLUT main loop...\n");
    glutMainLoop();

    return 0;
}