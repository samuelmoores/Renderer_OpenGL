#include <GL/glew.h>
#include "Renderer.h"
#include "Shader.h"
#include <iostream>
#include <fstream>


Shader::Shader(const std::string& filepath)
    : m_Filepath(filepath), m_Renderer_ID(0)
{
    ShaderSource src = ParseShader(m_Filepath);
    m_Renderer_ID = CreateShader(src.VertexSource, src.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_Renderer_ID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_Renderer_ID));

}

void Shader::Unbind() const
{
    
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float f1, float f2, float f3, float f4)
{
    glUniform4f(GetUniform4fLocation(name), f1, f2, f3, f4);
}

int Shader::GetUniform4fLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_Renderer_ID, name.c_str());

    if (location == -1)
    {
        std::cout << "shader doesn't exist\n";
    }
    m_UniformLocationCache[name] = location;

    return location;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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

unsigned int Shader::CreateShader(const std::string& VertexSrc, const std::string& FragmentSrc)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, VertexSrc);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, FragmentSrc);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


ShaderSource Shader::ParseShader(const std::string path)
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

