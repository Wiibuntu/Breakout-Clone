#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>  // Add this line for sin() and cos()


// Window size
const int width = 800;
const int height = 600;

// Paddle properties
float paddleWidth = 100;
float paddleHeight = 20;
float paddleX = width / 2 - paddleWidth / 2;
float paddleY = 50;
float paddleSpeed = 20;

// Ball properties
float ballRadius = 10;
float ballX = width / 2;
float ballY = paddleY + paddleHeight + ballRadius;
float ballSpeedX = 5;
float ballSpeedY = 5;

// Block properties
const int rows = 5;
const int cols = 8;
float blockWidth = 80;
float blockHeight = 30;
std::vector<std::vector<bool> > blocks(rows, std::vector<bool>(cols, true));

// Color Palette
float colorPalette[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
int currentPalette = 0;

void drawRectangle(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawCircle(float x, float y, float radius) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 360; i++) {
        float angle = i * 3.14159 / 180;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw blocks
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (blocks[i][j]) {
                glColor3fv(colorPalette[currentPalette]);
                drawRectangle(j * blockWidth, height - (i + 1) * blockHeight, blockWidth, blockHeight);
            }
        }
    }

    // Draw paddle
    glColor3f(1, 1, 1);
    drawRectangle(paddleX, paddleY, paddleWidth, paddleHeight);

    // Draw ball
    glColor3f(1, 1, 0);
    drawCircle(ballX, ballY, ballRadius);

    glutSwapBuffers();
}

void update(int value) {
    ballX += ballSpeedX;
    ballY += ballSpeedY;

    // Ball collision with walls
    if (ballX - ballRadius < 0 || ballX + ballRadius > width) {
        ballSpeedX = -ballSpeedX;
    }
    if (ballY + ballRadius > height) {
        ballSpeedY = -ballSpeedY;
    }

    // Ball collision with paddle
    if (ballX > paddleX && ballX < paddleX + paddleWidth && ballY - ballRadius < paddleY + paddleHeight) {
        ballSpeedY = -ballSpeedY;
    }

    // Ball collision with blocks
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (blocks[i][j]) {
                float blockX = j * blockWidth;
                float blockY = height - (i + 1) * blockHeight;
                if (ballX > blockX && ballX < blockX + blockWidth && ballY + ballRadius > blockY && ballY - ballRadius < blockY + blockHeight) {
                    ballSpeedY = -ballSpeedY;
                    blocks[i][j] = false;
                }
            }
        }
    }

    // Check if all blocks are destroyed
    bool allDestroyed = true;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (blocks[i][j]) {
                allDestroyed = false;
                break;
            }
        }
    }
    if (allDestroyed) {
        blocks.assign(rows, std::vector<bool>(cols, true));
        currentPalette = (currentPalette + 1) % 3;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void handleKeypress(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT && paddleX > 0) {
        paddleX -= paddleSpeed;
    }
    if (key == GLUT_KEY_RIGHT && paddleX + paddleWidth < width) {
        paddleX += paddleSpeed;
    }
}

void initialize() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glClearColor(0, 0, 0, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("3D Breakout Clone");
    initialize();
    glutDisplayFunc(display);
    glutSpecialFunc(handleKeypress);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}

