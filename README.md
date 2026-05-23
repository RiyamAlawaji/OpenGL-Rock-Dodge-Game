# OpenGL 2D Rock Dodge Game 

An interactive 2D arcade game built using **C++** and the **OpenGL (GLUT)** library. The project demonstrates real-time computer graphics concepts, user input handling, and collision detection algorithms.

## Game Mechanics & Features
* **Player Controller:** Move a custom-drawn human figure left and right using the keyboard arrow keys to avoid obstacles.
* **Dynamic Obstacle Spawning:** Randomly generates 3 unique structural types of falling rocks (Jagged Rocks, Round Boulders, and Crystal-like Crystals) with distinct visuals and color properties.
* **Score System:** Tracks your survival score in real-time, incrementing with each rock successfully dodged.
* **Collision Detection:** Computes Euclidean bounding radius distances between the player and falling assets to trigger a specialized "Game Over" screen.
* **Smooth Animation:** Operates on an inner timer loop achieving a consistent ~60 FPS update loop.

## Tech Stack
* **Language:** C++
* **Graphics Library:** OpenGL Utility Toolkit (GLUT)

## How to Play
1. Use the **LEFT** and **RIGHT** Arrow keys to move the player.
2. Avoid the falling rocks from the top.
3. If you crash, press **'R'** or **'r'** to restart the game and reset your score.
4. Press **ESC** at any time to exit the simulation.
