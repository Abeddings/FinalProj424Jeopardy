#pragma once
#include <iostream>
#include <cctype>
#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <cmath>
#include <limits>
#include <random>
#include <queue>
#include <mutex>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using namespace std;

class Button {
public:
    float x, y, size;
    Button(float x, float y, float size)
        : x(x), y(y), size(size) {}

    bool isHovered(float mouseX, float mouseY) const { // checks where mouse is and if it is over the button
        return mouseX >= x - size / 2 && mouseX <= x + size / 2 &&
            mouseY >= y - size / 2 && mouseY <= y + size / 2;
    }

    void draw() const {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y); // Center of the star
        float angle = 3.14159 / 5; // 5 points for the star
        float innerRadius = size * 0.5f;
        float outerRadius = size;
        bool outer = true;
        for (int i = 0; i < 5 * 2; i++) { //attempt to create the star
            float currentRadius = outer ? outerRadius : innerRadius;
            float currentAngle = angle * i;
            glVertex2f(x + cos(currentAngle) * currentRadius, y + sin(currentAngle) * currentRadius);
            outer = !outer;
        }
        glEnd();
    }
};


