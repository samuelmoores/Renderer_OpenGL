#pragma once
#include <string>
#include <unordered_map>

enum type { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

struct ShaderSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	unsigned int m_Renderer_ID;
	const std::string m_Filepath;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, float f1, float f2, float f3, float f4);
	int GetUniform4fLocation(const std::string& name);

private:
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& VertexSrc, const std::string& FragmentSrc);
	ShaderSource ParseShader(const std::string filepath);

};
