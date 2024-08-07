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

        void setFloat (const std::string &name, const float &num) const;
        void setMat4 (const std::string &name, const glm::mat4 &mat) const;
        void setVec3 (const std::string &name, const glm::vec3 &vec) const;
};

#endif
