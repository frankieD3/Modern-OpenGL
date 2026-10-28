#pragma once

#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

extern GLuint shader;
extern const char *vShader;
extern const char *fShader;

void AddShader(GLuint program,
               const char *shaderCode,
               GLenum shaderType);

void CompileShaders();

class Uniforms
{
public:
    Uniforms() {
        UniformModel = 0;
    }

    static void SetUniformModel(GLuint location) {
        UniformModel = location;
    }
    static GLuint GetUniformModel() {
        return UniformModel;
    }

private:
    static GLuint UniformModel;
};

extern Uniforms uniforms;
