// 2D PLOTTER
// Author: Mark Daniel G. Dacer
// Date: 9-11-2024

#include <cstdlib>
#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>


// Define a structure to hold point coordinates
struct Point {
    float x, y;
};

// Vector to hold all plotted points
std::vector<Point> points;

// Enumeration for drawing modes
enum DrawMode {
    LINE_LOOP,
    TRIANGLES,
    QUADS,
    LINES,
    POINTS,
    POLYGONS
};

// Current drawing mode
DrawMode currentMode = LINE_LOOP;

// Function to draw text
void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *text++);
    }
}

// Function to convert an integer to a string (workaround for std::to_string)
std::string intToString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

// Function to set up the orthographic projection
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10.0, 10.0, -10.0, 10.0); // 20x20 grid centered at origin
}

// Function to handle display
// Function to handle display
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw grid lines
    glColor3f(0.5, 0.5, 0.5); // Set grid color to gray
    glBegin(GL_LINES);
    for (int i = -10; i <= 10; ++i) {
        glVertex2f(i, -10);
        glVertex2f(i, 10);
        glVertex2f(-10, i);
        glVertex2f(10, i);
    }
    glEnd();

    // Draw the plotted points
    glColor3f(1.0, 0.0, 0.0); // Set point color to red
    glPointSize(5.0);
    glBegin(GL_POINTS);
    for (size_t i = 0; i < points.size(); ++i) {
        glVertex2f(points[i].x, points[i].y);
    }
    glEnd();

    glColor3f(1.0, 1.0, 1.0); // Set text color to white
    for (size_t i = 0; i < points.size(); ++i) {
        std::string coordLabel = "(" + intToString(static_cast<int>(points[i].x)) + ", " + intToString(static_cast<int>(points[i].y)) + ")";
        drawText(points[i].x + 0.2f, points[i].y + 0.2f, coordLabel.c_str());
    }

    // Draw primitives based on the current mode
    glColor3f(0.0, 1.0, 0.0); // Set color for primitives
    switch (currentMode) {
    case LINE_LOOP:
        if (points.size() >= 2) {
            glBegin(GL_LINE_LOOP);
            for (size_t i = 0; i < points.size(); ++i) {
                glVertex2f(points[i].x, points[i].y);
            }
            glEnd();
        }
        break;
    case TRIANGLES:
        for (size_t i = 0; i + 2 < points.size(); i += 3) {
            glBegin(GL_TRIANGLES);
            glVertex2f(points[i].x, points[i].y);
            glVertex2f(points[i + 1].x, points[i + 1].y);
            glVertex2f(points[i + 2].x, points[i + 2].y);
            glEnd();
        }
        break;
    case QUADS:
        for (size_t i = 0; i + 3 < points.size(); i += 4) {
            glBegin(GL_QUADS);
            glVertex2f(points[i].x, points[i].y);
            glVertex2f(points[i + 1].x, points[i + 1].y);
            glVertex2f(points[i + 2].x, points[i + 2].y);
            glVertex2f(points[i + 3].x, points[i + 3].y);
            glEnd();
        }
        break;
    case LINES:
        if (points.size() >= 2) {
            glBegin(GL_LINES);
            for (size_t i = 0; i < points.size() - 1; ++i) {
                glVertex2f(points[i].x, points[i].y);
                glVertex2f(points[i + 1].x, points[i + 1].y);
            }
            glEnd();
        }
        break;
    case POINTS:
        glBegin(GL_POINTS);
        for (size_t i = 0; i < points.size(); ++i) {
            glVertex2f(points[i].x, points[i].y);
        }
        glEnd();
        break;
    case POLYGONS: // Handle new mode
        if (points.size() >= 3) {
            glBegin(GL_POLYGON);
            for (size_t i = 0; i < points.size(); ++i) {
                glVertex2f(points[i].x, points[i].y);
            }
            glEnd();
        }
        break;
    }

    // Draw the origin label
    glRasterPos2f(2.5f, 11.0f); // Position for the origin label
    const char* originLabel = "Press 1-6 to change modes"; // Updated text
    while (*originLabel) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *originLabel++);
    }

    glRasterPos2f(-10.0f, -11.0f); // Position for the origin label
    const char* originLabel1 = "Press MOUSE 1 to plot points";
    while (*originLabel1) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *originLabel1++);
    }

    glRasterPos2f(2.0f, -11.0f); // Position for the origin label
    const char* originLabel3 = "Press MOUSE 2 to remove points";
    while (*originLabel3) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *originLabel3++);
    }

    // Draw y-axis labels from -10 to 10
    glColor3f(1.0, 1.0, 1.0); // Set text color to white
    for (int i = -10; i <= 10; ++i) {
        if (i != 0) { // Skip the origin
            std::string label = intToString(i); // Use the custom function to convert integer to string
            drawText(-0.5f, i - 0.25f, label.c_str()); // Adjust label position slightly
        }
    }

    for (int i = -10; i <= 10; ++i) {
        if (i != 0) { // Skip the origin
            std::string label = intToString(i); // Convert integer to string
            drawText(i - 0.25f, -0.75f, label.c_str()); // Adjust label position slightly
        }
    }

    // Display the current mode in the top left corner
    glColor3f(1.0, 1.0, 1.0); // Set text color to white
    std::string modeText;
    switch (currentMode) {
    case LINE_LOOP:
        modeText = "Mode: LINE_LOOP";
        break;
    case TRIANGLES:
        modeText = "Mode: TRIANGLES";
        break;
    case QUADS:
        modeText = "Mode: QUADS";
        break;
    case LINES:
        modeText = "Mode: LINES";
        break;
    case POINTS:
        modeText = "Mode: POINTS";
        break;
    case POLYGONS: // Add this case
        modeText = "Mode: POLYGONS";
        break;
    }
    drawText(-10.0f, 11.0f, modeText.c_str());

    glFlush();
}


// Function to handle mouse input
void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        float xpos = (float)x / (glutGet(GLUT_WINDOW_WIDTH) / 24.0) - 12.0;
        float ypos = 12.0 - (float)y / (glutGet(GLUT_WINDOW_HEIGHT) / 24.0);

        xpos = round(xpos);
        ypos = round(ypos);

        if (button == GLUT_LEFT_BUTTON) {
            // Add point if left button is clicked
            bool pointExists = false;
            for (size_t i = 0; i < points.size(); ++i) {
                if (points[i].x == xpos && points[i].y == ypos) {
                    pointExists = true;
                    break;
                }
            }

            if (!pointExists) {
                Point p = { xpos, ypos };
                points.push_back(p);
            }
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            // Remove the closest point if right button is clicked
            float minDist = 1.0f; // Threshold distance to consider a click close to a point
            size_t closestPointIndex = -1;
            for (size_t i = 0; i < points.size(); ++i) {
                float dist = std::sqrt(std::pow(points[i].x - xpos, 2) + std::pow(points[i].y - ypos, 2));
                if (dist < minDist) {
                    minDist = dist;
                    closestPointIndex = i;
                }
            }

            if (closestPointIndex != -1) {
                points.erase(points.begin() + closestPointIndex);
            }
        }

        glutPostRedisplay();
    }
}

// Function to handle keyboard input for mode switching
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        currentMode = LINE_LOOP;
        break;
    case '2':
        currentMode = TRIANGLES;
        break;
    case '3':
        currentMode = QUADS;
        break;
    case '4':
        currentMode = LINES;
        break;
    case '5':
        currentMode = POINTS;
        break;
    case '6': // Add this case
        currentMode = POLYGONS;
        break;
    }
    glutPostRedisplay();
}


// Function to handle window resizing
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-12.0, 12.0, -12.0, 12.0); // Maintain 20x20 projection centered at origin
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("2D Point Plotter by Mark Daniel G. Dacer");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
