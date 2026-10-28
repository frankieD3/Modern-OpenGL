#include "shaders.h"
#include "spdlog/spdlog.h"
#include <GL/glew.h>
#include <string.h>

GLuint shader = 0;
GLuint Uniforms::UniformModel = 0;

const char *vShader = "  \n\
#version 330 core                                                           \n\
    layout(location = 0) in vec3 pos;                                       \n\
    uniform mat4 model;                                                    \n\
    void main()                                                                \n\
{                                                                           \n\
        gl_Position = model * vec4( pos , 1.0);         \n\
} ";

const char *fShader = "  \n\
#version 330 core                                                           \n\
    out vec4 color;                                                         \n\
 void main()                                                                \n\
{                                                                           \n\
        color = vec4(1.0, 0.0, 0.0, 1.0);                                   \n\
} ";

void AddShader(GLuint program,
               const char *shaderCode,
               GLenum shaderType)
{
    // | 1 | Create the shader object
    //
    GLuint theShader = glCreateShader(shaderType);
    const char *theCode[1];
    theCode[0] = shaderCode;

    GLint codelength = static_cast<GLint>(strlen(shaderCode));

    // | 2 | Attach the shader source code to the shader object
    // Attach the shader source code to the shader object and compile it
    glShaderSource(theShader,
                   1,
                   theCode,
                   &codelength);
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

    uniforms.SetUniformModel(glGetUniformLocation(shader, "model"));
}