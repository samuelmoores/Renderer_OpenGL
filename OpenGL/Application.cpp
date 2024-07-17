#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

// 
//
//static unsigned int CompileShader(unsigned int type, const std::string& source)
//{
//    unsigned int id = glCreateShader(type);
//    const char* src = source.c_str();
//    glShaderSource(id, 1, &src, nullptr);
//    glCompileShader(id);
//
//    //Error Handling
//    int result;
//    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
//
//    if (result == GL_FALSE)
//    {
//        int message_length; //not used outside gl functions
//        char message[1024];
//        
//        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &message_length);
//        glGetShaderInfoLog(id, 1024, &message_length, message);
//       
//        std::cout << (type == GL_VERTEX_SHADER ? "vertex " : "fragment ");
//        std::cout << "shader compilation fail: ";
//        std::cout << "message length: " << message_length << std::endl;
//        std::cout << message << std::endl;
//    }
//
//    return id;
//}
//
//static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
//{
//    unsigned int program = glCreateProgram();
//    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
//    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
//
//    glAttachShader(program, vs);
//    glAttachShader(program, fs);
//    glLinkProgram(program);
//    glValidateProgram(program);
//    glDeleteShader(vs);
//    glDeleteShader(fs);
//
//    return program;
//}

int main(void)
{
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    std::cout << glGetString(GL_VERSION) << std::endl;

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

    //----------------------------Vertex Array------------------------------------
    VertexArray va;
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
 
    //------------------------------Index Buffer-------------------------------------------
    IndexBuffer ib(indeces, 6);

    //-----------------------------Shaders-------------------------------------------------------------
    Shader shader("Shaders.txt");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.0f, 0.5f, 0.5f, 1.0f);
    
    //-----------------------------Animation--------------------
    float g = 0.0f;
    float inc = 0.01f;

    //-----------------------Unbind Everything-----------------------------
    vb.Unbind();
    ib.Unbind();
    va.Unbind();
    shader.Unbind();

    //Renderer
    Renderer renderer;

    /* -------------------------Loop until the user closes the window----------------------------------- */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        //Bind Shader
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.0f, g, 0.5f, 1.0f);

        //Issue Draw Call
        renderer.Draw(va, ib, shader);

        //Shader Logic
        if (g > 1.0f)
            inc *= -1;
        else if (g < 0.0f)
            inc *= -1;

        g += inc;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    
    glfwTerminate();
    return 0;
}