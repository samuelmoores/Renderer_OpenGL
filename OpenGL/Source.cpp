#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

enum type { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

struct ShaderSource {
    std::string VertexSource;
    std::string FragmentSource;
};

 ShaderSource ParseShader(const std::string path)
{
    std::ifstream fin(path);
    std::string currentLine;
    type ShaderType = NONE;
    ShaderSource src;

    while (getline(fin, currentLine))
    {
        if (currentLine.find("#shader VERTEX") != std::string::npos)
            ShaderType = VERTEX;
        else if (currentLine.find("#shader FRAGMENT") != std::string::npos)
            ShaderType = FRAGMENT;
        else
        {
            switch (ShaderType)
            {
            case VERTEX:
                src.VertexSource += (currentLine + "\n");
                break;
            case FRAGMENT:
                src.FragmentSource += (currentLine + "\n");
                break;
            default:
                std::cout << "No shader type in shader file\n";
            }
        }
    }

    return src;
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //Error Handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int message_length; //not used outside gl functions
        char message[1024];
        
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &message_length);
        glGetShaderInfoLog(id, 1024, &message_length, message);
       
        std::cout << (type == GL_VERTEX_SHADER ? "vertex " : "fragment ");
        std::cout << "shader compilation fail: ";
        std::cout << "message length: " << message_length << std::endl;
        std::cout << message << std::endl;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glewInit();

    //Vertex Buffers
    float positions[8] = {
        -1.0f, -1.0f, //0
         1.0f, -1.0f, //1
         1.0f,  1.0f, //2
        -1.0f,  1.0f  //3
    };

    unsigned int indeces[] = {
        0, 1, 2,
        2, 3, 0
    };

    for (int i = 0; i < 8; i++)
    {
        positions[i] /= 10.0f;
    }

    //Buffers
    unsigned int buffer_vertex;
    glGenBuffers(1, &buffer_vertex);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_vertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, positions, GL_STATIC_DRAW);

    unsigned int buffer_index;
    glGenBuffers(1, &buffer_index);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indeces, GL_STATIC_DRAW);

    //Attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

    //Shaders
    ShaderSource src;
    src = ParseShader("Shaders.txt");
    unsigned int shader = CreateShader(src.VertexSource, src.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}