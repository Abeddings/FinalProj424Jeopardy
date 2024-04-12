#pragma once
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
#include "Button.h"

using namespace std;

const GLuint WIDTH = 800, HEIGHT = 600; // creates window dimensions

class ClickableButtonApp {
public:
    static int clickCount;
    ClickableButtonApp() {// constructor
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            std::exit(-1);
        }

        window = glfwCreateWindow(WIDTH, HEIGHT, "Clickable Button", NULL, NULL);
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            std::exit(-1);
        }

        glfwMakeContextCurrent(window);
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            std::exit(-1);
        }

        glfwSetMouseButtonCallback(window, mouseButtonCallback);
        glViewport(0, 0, WIDTH, HEIGHT);
    }

    ~ClickableButtonApp() { //destructor
        glfwTerminate();
    }

    void run() { 
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            button.draw(); // Draw the button

            glfwSwapBuffers(window);
        }
    }

private:
    GLFWwindow* window;
    static Button button;

    static void mouseButtonCallback(GLFWwindow* window, int _button, int action, int mods) {
        if (_button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            // Convert to normalized device coordinates
            mouseX = 2.0 * mouseX / WIDTH - 1.0;
            mouseY = 1.0 - 2.0 * mouseY / HEIGHT;
            if (button.isHovered(mouseX, mouseY)) {
                cout << "Winner!" << endl;
                clickCount++; // Increment click count
                if (clickCount >= 3) {
                    // Set window to close
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                }
            }
        }
    }
};