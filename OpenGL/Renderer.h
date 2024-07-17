#pragma once
#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearErrors(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearErrors();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
public:
	void Clear() const;
	void Draw(VertexArray& va, IndexBuffer& ib, Shader& shader) const;
};

