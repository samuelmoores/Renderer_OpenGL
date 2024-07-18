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
#include "Texture.h"


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
    float positions[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, //0
         0.5f, -0.5f, 1.0f, 0.0f, //1
         0.5f,  0.5f, 1.0f, 1.0f, //2
        -0.5f,  0.5f, 0.0f, 1.0f  //3
    };

    unsigned int indeces[] = {
        0, 1, 2,
        2, 3, 0
    };

    //blending
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND))

    //----------------------------Vertex Array------------------------------------
    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
 
    //------------------------------Index Buffer-------------------------------------------
    IndexBuffer ib(indeces, 6);

    //-----------------------------Shaders-------------------------------------------------------------
    Shader shader("res/shaders/Shaders.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.0f, 0.5f, 0.5f, 1.0f);

    //Texture
    Texture texture("img/Logo.png");
    texture.Bind();
    //shader.SetUniform1i("u_Texture", 0);
    
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