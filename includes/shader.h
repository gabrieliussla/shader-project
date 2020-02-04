#ifndef SHADER
#define SHADER

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
    private:
        unsigned int ID;
    public:
        Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
        void use();
        void setBool(char* name, bool value);
        void setInt(char* name, int value);
        void setFloat(char* name, float value);
        void setMat4(char* name, glm::mat4 value);
};

#endif
