#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"

// Window dimensions
const GLint WINDOW_WIDTH = 800;
const GLint WINDOW_HEIGHT = 600;
int main() {

    // | 1 | nitialize GLFW
    //
    if (!glfwInit()) {
        spdlog::error("Failed to initialize GLFW\n");
        glfwTerminate();
        return -1;
    }

    // | 2 | Setup GLFW window properties
    // Set the required OpenGL version and profile
    //
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // Core profile = No backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For MacOS compatibility

    // | 3 | Create a GLFW window
    GLFWwindow* mainWindow = glfwCreateWindow(WINDOW_WIDTH,
                                              WINDOW_HEIGHT,
                                              "OpenGL Window",
                                              NULL,         // No shared context
                                              NULL);        // No error
    // | 4 | Check if the window was created successfully
    //
    if (!mainWindow) {
        spdlog::error("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    
    // | 5 | Get Buffer size information
    //
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow,
                           &bufferWidth,
                           &bufferHeight);


    // | 6 | Make the OpenGL context current
    //
    glfwMakeContextCurrent(mainWindow);

    // | 7 | Initialize GLEW
    //
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        spdlog::error("Failed to initialize GLEW\n");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return -1;
    }

    // | 8 | Set the viewport size
    //
    glViewport(0,
               0,
               bufferWidth,
               bufferHeight);

    // Main loop
    while (!glfwWindowShouldClose(mainWindow)) {
        // Render here

        // Poll for and process events
        glfwPollEvents();

        // Clear the window
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        // Swap buffers
        glfwSwapBuffers(mainWindow);

    }

    // Clean up and exit
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;

}