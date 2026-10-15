#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"

// Window dimensions
const GLint WINDOW_WIDTH = 800;
const GLint WINDOW_HEIGHT = 600;

GLuint VAO, VBO, shader;

// Vertex data and buffer setup for a triangle

static const char *vShader = "\n\
#version 330 core                                                           \n\
    layout(location = 0) in vec3 pos;                                       \n\
 void main()                                                                \n\
{                                                                           \n\
        gl_Position = vec4(0.4f * pos.x, 0.4f * pos.y, pos.z, 1.0);         \n\
} ";

static const char *fShader = "\n\
#version 330 core                                                           \n\
    out vec4 color;                                                         \n\
 void main()                                                                \n\
{                                                                           \n\
        color = vec4(1.0, 0.0, 0.0, 1.0);                                   \n\
} ";

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

void AddShader(GLuint program,
               const char *shaderCode,
               GLenum shaderType)
{
    // | 1 | Create the shader object
    //
    GLuint theShader = glCreateShader(shaderType);
    const char *theCode[1];
    theCode[0] = shaderCode;

    GLint codelength[1] = {strlen(shaderCode)};

    // | 2 | Attach the shader source code to the shader object
    // Attach the shader source code to the shader object and compile it
    glShaderSource(theShader,
                   1,
                   theCode,
                   codelength);
    glCompileShader(theShader);

    // | 3 | Check for compilation errors
    // Get the compilation status and log any errors
    GLint result = 0;
    GLchar elog[1024];
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader,
                           sizeof(elog),
                           NULL,
                           elog);
        spdlog::error(" %d Shader compilation failed: {}\n", shaderType, elog);
        return;
    }

    // | 4 | Attach the compiled shader to the program
    // Attach the compiled shader to the program and retur
    glAttachShader(program, theShader);

    return;
}
/// CompileShaders
/// @brief Creates and compiles the vertex and fragment shaders, links them into a shader program, and deletes the individual shaders.
void CompileShaders()
{
    // | 1 | Create the shader program
    //
    shader = glCreateProgram();
    if (!shader)
    {
        spdlog::error("Failed to create shader program\n");
        return;
    }

    // | 2 | Add the vertex and fragment shaders to the program
    //
    AddShader(shader,
              vShader,
              GL_VERTEX_SHADER);

    AddShader(shader,
              fShader,
              GL_FRAGMENT_SHADER);

    GLint result = 1;
    GLchar elog[1024];

    // | 3 | Link and validate the shader program
    //
    glLinkProgram(shader);
    glGetProgramiv(shader,
                   GL_LINK_STATUS,
                   &result);
    if (!result)
    {
        glGetProgramInfoLog(shader,
                            sizeof(elog),
                            NULL,
                            elog);

        spdlog::error("Shader program linking failed: {}\n", elog);
        return;
    }

    // | 4 | Validate the shader program
    //
    glValidateProgram(shader);
    glGetProgramiv(shader,
                   GL_VALIDATE_STATUS,
                   &result);
    if (!result)
    {
        glGetProgramInfoLog(shader,
                            sizeof(elog),
                            NULL,
                            elog);

        spdlog::error("Shader program validation failed: {}\n", elog);
        return;
    }
}

int main() {

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

    // Main loop
    while (!glfwWindowShouldClose(mainWindow))
    {

        // | 1 | Poll for and process events
        //
        glfwPollEvents();

        // | 2 | Clear the window
        //
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // | 3 | Use the shader program for rendering
        //
        glUseProgram(shader);

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