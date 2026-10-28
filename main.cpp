#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.h"

// Window dimensions
const GLint WINDOW_WIDTH = 800;
const GLint WINDOW_HEIGHT = 600;

const float toRadians = 3.14159265f / 180.0f;

GLuint VAO = 0, VBO = 0;

GLint uniformModel = 0;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;

float curAngle = 0.0f;
float angleIncrement = 1.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float MaxSize = 0.8f;
float MinSize = 0.1f;

// Vertex data and buffer setup for a triangle

void CreateTriangle()
{
    GLfloat vertices[] = {
        // Positions
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f};

    // Generate and bind the Vertex Array Object
    //
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind the Vertex Buffer Object
    //
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Fill the buffer with vertex data
    //
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertices),
                 vertices,
                 GL_STATIC_DRAW);

    // Specify the layout of the vertex data
    //
    glVertexAttribPointer(0,            // Attribute index (location = 0 in the vertex shader)
                          3,            // Number of components per vertex (x, y, z)
                          GL_FLOAT,     // Data type of each component
                          GL_FALSE,     // Whether the data should be normalized
                          0,            // Stride (byte offset between consecutive vertices)
                          (GLvoid *)0); // Offset of the first component

    // Enable the vertex attribute array for the specified attribute index
    // (location = 0 in the vertex shader)
    //
    glEnableVertexAttribArray(0);

    // Unbind the VBO and VAO to prevent accidental modification
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int main()
{

    // | 1 | Initialize GLFW
    // TODO: Handle the case where GLFW initialization fails
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
    // TODO: Handle the case where the window creation fails
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

    // | 9 | Create the triangle
    //
    CreateTriangle();

    // | 10 | Compile the shaders
    //
    CompileShaders();
    uniformModel = glGetUniformLocation(shader, "model");

    // Main loop
    while (!glfwWindowShouldClose(mainWindow))
    {

        // | 1 | Poll for and process events
        //
        glfwPollEvents();
        if (direction)
        {
            triOffset += triIncrement;
        }
        else
        {
            triOffset -= triIncrement;
        }

        if (abs(triOffset) >= triMaxOffset)
        {
            direction = !direction;
        }

        curAngle += angleIncrement;
        if (curAngle >= 360.0f)
        {
            curAngle -= 360.0f;
        }

        if (sizeDirection)
        {
            curSize += 0.01f;
        }
        else
        {
            curSize -= 0.01f;
        }
        if (curSize >= MaxSize || curSize <= MinSize)
        {
            sizeDirection = !sizeDirection;
        }

        // | 2 | Clear the window
        //
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // | 3 | Use the shader program for rendering
        //
        glUseProgram(shader);

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model,
                               glm::vec3(triOffset, 0.0f, 0.0f));

        model = glm::rotate(model,
                            curAngle * toRadians,
                            glm::vec3(0.0f, 0.0f, 1.0f));

        model = glm::scale(model,
                           glm::vec3(curSize, curSize, 1.0f));

        glUniformMatrix4fv(uniformModel,
                           1,
                           GL_FALSE,
                           glm::value_ptr(model));

        // | 4 | Bind the vertex array object and draw the triangle
        //
        glBindVertexArray(VAO);           // Bind the vertex array object for the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3); // Draw the triangle

        // | 5 | Unbind the vertex array object and stop using the shader program
        //
        glBindVertexArray(0); // Unbind the vertex array object
        glUseProgram(0);

        // | 6 | Swap buffers
        //
        glfwSwapBuffers(mainWindow);
    }

    // Clean up and exit
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}