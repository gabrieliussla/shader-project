#include "shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {

    std::ifstream vertexFile, fragmentFile;
    std::string vertexCodeS, fragmentCodeS;
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);

        std::stringstream vertexStream, fragmentStream;
        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();

        vertexFile.close();
        fragmentFile.close();

        vertexCodeS = vertexStream.str();
        fragmentCodeS = fragmentStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "Error in getting shader files!" << std::endl;
    }
    const char* vertexCode = vertexCodeS.c_str();
    const char* fragmentCode = fragmentCodeS.c_str();

    unsigned int fragmentShader, vertexShader;
    int success;
    char log[512];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, log);
        std::cout << "Vertex shader compilation failed with: " << std::endl << log << std::endl;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, log);
        std::cout << "Fragment shader compilation failed with: " << std::endl << log << std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, log);
        std::cout << "Program linking failed with: " << std::endl << log << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(char* name, bool value) {
    glUniform1i(glGetUniformLocation(ID, name), (int)value);
}

void Shader::setInt(char* name, int value) {
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::setFloat(char* name, float value) {
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::setMat4(char* name, glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
}
