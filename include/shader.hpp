#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

class Shader
{
    private:

        void checkCompilerErrors (unsigned int shader, std::string type);

    public:

        unsigned int programID;

        Shader (const char* vertexShaderPath, const char* fragmentShaderPath);

        void use ();
};

#endif