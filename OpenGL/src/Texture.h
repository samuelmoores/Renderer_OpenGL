#pragma once
#include "Renderer.h"

class Texture {
private:
	unsigned int m_Renderer_ID;
	std::string m_File_Path;
	unsigned char* m_Local_Buffer;
	int m_Width, m_Height, m_BPP;

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0);
	void Unbind();
};
