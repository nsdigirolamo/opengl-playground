#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

constexpr unsigned int WINDOW_WIDTH = 768;
constexpr unsigned int WINDOW_HEIGHT = 432;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";


void errorCallback (int error, const char* description)
{
    std::cerr << "GLFW Error: " << description << std::endl;
    glfwTerminate();
    exit(-1);
}

void framebufferSizeCallback (GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void keyCallback (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main ()
{
    glfwSetErrorCallback(errorCallback);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "opengl-solitaire", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "GLFW Error: Failed to create a window." << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);

    if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)))
    {
        std::cerr << "GLFW Error: Failed to initialize GLAD.";
        glfwTerminate();
        exit(-1);
    }

    int compilerSuccess;
    char compilerInfo [512];

    const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compilerSuccess);
    if (!compilerSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, compilerInfo);
        std::cerr << "Vertex Shader Compilation Failed: " << compilerInfo << std::endl;
    }

    const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compilerSuccess);
    if (!compilerSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, compilerInfo);
        std::cerr << "Fragment Shader Compilation Failed: " << compilerInfo << std::endl;
    }

    const unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &compilerSuccess);
    if (!compilerSuccess) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, compilerInfo);
        std::cerr << "Shader Program Linkage Failed: " << compilerInfo << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    const float vertices [] =
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    unsigned int vertexArray, vertexBuffer;

    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vertexArray);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    exit(0);
}