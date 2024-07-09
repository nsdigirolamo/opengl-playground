#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include "glm/glm.hpp"

class Shader
{
    private:

        void checkCompilerErrors (unsigned int shader, std::string type);

    public:

        unsigned int programID;

        Shader (const char* vertexShaderPath, const char* fragmentShaderPath);

        void use ();

        void setMat4 (const std::string &name, const glm::mat4 &mat) const;
};

#endif
