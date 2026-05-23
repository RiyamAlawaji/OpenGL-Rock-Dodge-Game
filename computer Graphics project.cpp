#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <string>
#include <cstdio>

// Game constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float PLAYER_SIZE = 0.1f;
const float OBSTACLE_SIZE = 0.05f;
const float PLAYER_SPEED = 0.04f;
const float OBSTACLE_SPEED = 0.03f;
const int MAX_OBSTACLES = 10;
const float SPAWN_CHANCE = 0.04f; // Probability of spawning a new obstacle each frame

// Game state variables
float playerX = 0.0f;
bool gameOver = false;
int score = 0;
std::vector<float> obstacleX;
std::vector<float> obstacleY;
std::vector<int> obstacleType; // 0-2 for different rock types

// Initialize the game
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    srand(static_cast<unsigned int>(time(0)));
}

// Convert int to string (C++98 compatible)
std::string intToString(int num) {
    char buffer[32];
    sprintf(buffer, "%d", num);
    return std::string(buffer);
}

// Add a new obstacle at the top of the screen
void addObstacle() {
    if (obstacleX.size() < MAX_OBSTACLES) {
        // Random x position between -1 and 1
        float x = (float)(rand() % 200 - 100) / 100.0f;
        obstacleX.push_back(x);
        obstacleY.push_back(1.0f); // Start at the top of the screen
        obstacleType.push_back(rand() % 3); // Random rock type (0-2)
    }
}

// Draw a human figure
void drawPlayer() {
    float x = playerX;
    float y = -0.9f;
    float size = PLAYER_SIZE;
    
    // Body (blue)
    glColor3f(0.0f, 0.3f, 0.8f);
    glBegin(GL_POLYGON);
    glVertex2f(x - size/4, y - size/2);
    glVertex2f(x + size/4, y - size/2);
    glVertex2f(x + size/6, y + size/4);
    glVertex2f(x - size/6, y + size/4);
    glEnd();
    
    // Head (skin tone)
    glColor3f(0.9f, 0.75f, 0.6f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y + size/2); // Center of head
    for (int i = 0; i <= 20; i++) {
        float angle = 3.14159f * 2.0f * i / 20;
        glVertex2f(x + cos(angle) * size/5, y + size/2 + sin(angle) * size/5);
    }
    glEnd();
    
    // Arms
    glColor3f(0.0f, 0.3f, 0.8f);
    glBegin(GL_LINES);
    glVertex2f(x - size/6, y + size/8);
    glVertex2f(x - size/2, y);
    glVertex2f(x + size/6, y + size/8);
    glVertex2f(x + size/2, y);
    glEnd();
    glLineWidth(3.0f);
    
    // Legs
    glBegin(GL_LINES);
    glVertex2f(x - size/8, y - size/2);
    glVertex2f(x - size/4, y - size);
    glVertex2f(x + size/8, y - size/2);
    glVertex2f(x + size/4, y - size);
    glEnd();
    glLineWidth(1.0f);
}

// Draw rock obstacles
void drawRock(float x, float y, float size, int type) {
    // Different rock shapes based on type
    if (type == 0) {
        // Jagged rock
        glColor3f(0.6f, 0.6f, 0.6f);
        glBegin(GL_POLYGON);
        glVertex2f(x - size*0.8f, y - size*0.3f);
        glVertex2f(x - size*0.4f, y - size*0.8f);
        glVertex2f(x + size*0.5f, y - size*0.6f);
        glVertex2f(x + size*0.8f, y + size*0.1f);
        glVertex2f(x + size*0.3f, y + size*0.7f);
        glVertex2f(x - size*0.5f, y + size*0.5f);
        glEnd();
        
        // Rock details
        glColor3f(0.4f, 0.4f, 0.4f);
        glBegin(GL_LINES);
        glVertex2f(x - size*0.3f, y - size*0.2f);
        glVertex2f(x + size*0.2f, y - size*0.1f);
        glVertex2f(x, y);
        glVertex2f(x + size*0.3f, y + size*0.3f);
        glEnd();
    }
    else if (type == 1) {
        // Round boulder
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y); // Center
        for (int i = 0; i <= 20; i++) {
            float angle = 3.14159f * 2.0f * i / 20;
            float rad = size * (0.8f + 0.2f * sin(angle * 3));
            glVertex2f(x + cos(angle) * rad, y + sin(angle) * rad);
        }
        glEnd();
        
        // Boulder details
        glColor3f(0.3f, 0.3f, 0.3f);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i <= 8; i++) {
            float angle = 3.14159f * 2.0f * i / 8;
            float rad = size * 0.4f;
            glVertex2f(x + cos(angle) * rad, y + sin(angle) * rad);
        }
        glEnd();
    }
    else {
        // Crystal-like rock
        glColor3f(0.7f, 0.3f, 0.3f);
        glBegin(GL_POLYGON);
        glVertex2f(x - size*0.2f, y - size*0.7f);
        glVertex2f(x + size*0.6f, y - size*0.3f);
        glVertex2f(x + size*0.1f, y + size*0.8f);
        glVertex2f(x - size*0.5f, y + size*0.2f);
        glEnd();
        
        // Crystal details
        glColor3f(0.9f, 0.4f, 0.4f);
        glBegin(GL_LINES);
        glVertex2f(x - size*0.1f, y - size*0.3f);
        glVertex2f(x + size*0.1f, y + size*0.4f);
        glVertex2f(x - size*0.2f, y);
        glVertex2f(x + size*0.3f, y - size*0.1f);
        glEnd();
    }
}

// Draw all obstacles
void drawObstacles() {
    for (size_t i = 0; i < obstacleX.size(); i++) {
        drawRock(obstacleX[i], obstacleY[i], OBSTACLE_SIZE*2, obstacleType[i]);
    }
}

// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw background (starfield)
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 100; i++) {
        float x = -1.0f + 2.0f * (i % 10) / 9.0f;
        float y = -1.0f + 2.0f * (i / 10) / 9.0f;
        glVertex2f(x + sin(i) * 0.05f, y + cos(i) * 0.05f);
    }
    glEnd();
    
    if (!gameOver) {
        drawPlayer();
        drawObstacles();
        
        // Draw the score
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.9f, 0.9f);
        std::string scoreStr = "Score: " + intToString(score);
        
        // Modified for loop to be C++98 compatible
        for (size_t i = 0; i < scoreStr.length(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreStr[i]);
        }
    } else {
        // Game over message
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(-0.4f, 0.0f);
        std::string gameOverStr = "Game Over! Score: " + intToString(score);
        
        // Modified for loop to be C++98 compatible
        for (size_t i = 0; i < gameOverStr.length(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, gameOverStr[i]);
        }
        
        glRasterPos2f(-0.3f, -0.1f);
        std::string restartStr = "Press 'R' to restart";
        for (size_t i = 0; i < restartStr.length(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, restartStr[i]);
        }
    }
    
    glutSwapBuffers();
}

// Update game state
void update(int value) {
    if (!gameOver) {
        // Maybe spawn a new obstacle
        if ((float)rand() / RAND_MAX < SPAWN_CHANCE) {
            addObstacle();
        }
        
        // Update obstacle positions
        for (size_t i = 0; i < obstacleY.size(); i++) {
            obstacleY[i] -= OBSTACLE_SPEED;
            
            // Check for collision with player
            float dx = playerX - obstacleX[i];
            float dy = -0.9f - obstacleY[i];
            float distance = sqrt(dx * dx + dy * dy);
            
            if (distance < (PLAYER_SIZE + OBSTACLE_SIZE)) {
                gameOver = true;
            }
        }
        
        // Remove obstacles that have moved off the screen
        for (size_t i = 0; i < obstacleY.size(); i++) {
            if (obstacleY[i] < -1.0f) {
                obstacleX.erase(obstacleX.begin() + i);
                obstacleY.erase(obstacleY.begin() + i);
                obstacleType.erase(obstacleType.begin() + i);
                score++;
                i--; // Adjust index after erasing
            }
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // About 60 FPS
}

// Keyboard callback function
void keyboard(unsigned char key, int x, int y) {
    if (key == 'r' || key == 'R') {
        // Reset the game
        playerX = 0.0f;
        obstacleX.clear();
        obstacleY.clear();
        obstacleType.clear();
        score = 0;
        gameOver = false;
    }
    
    if (key == 27) { // ESC key
        exit(0);
    }
}

// Special key callback for arrow keys
void specialKeys(int key, int x, int y) {
    if (!gameOver) {
        if (key == GLUT_KEY_LEFT) {
            playerX -= PLAYER_SPEED;
            if (playerX < -1.0f + PLAYER_SIZE / 2) {
                playerX = -1.0f + PLAYER_SIZE / 2;
            }
        }
        
        if (key == GLUT_KEY_RIGHT) {
            playerX += PLAYER_SPEED;
            if (playerX > 1.0f - PLAYER_SIZE / 2) {
                playerX = 1.0f - PLAYER_SIZE / 2;
            }
        }
    }
}

// Reshape callback function
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Rock Dodge Game");
    
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, update, 0);
    
    glutMainLoop();
    return 0;
}

