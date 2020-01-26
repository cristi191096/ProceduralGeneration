#pragma once
#ifdef __APPLE__
#  include <GL/glew.h>

#else
#  include <GL/glew.h>


#pragma comment(lib, "glew32.lib") 
#endif

#include <vector>

struct VBElements {
	unsigned int type;
	unsigned int count;
	unsigned char normalised;

	static unsigned int GetTypeSize(unsigned int type) {
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
	}
};

class VertexBufferLayout
{
	std::vector<VBElements> Elements;
	unsigned int stride;

public:
	VertexBufferLayout() : stride(0) {}
	~VertexBufferLayout(){}

	template <typename T>
	void Push(unsigned int count) {
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count) {
		Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += count * VBElements::GetTypeSize(GL_FLOAT);
	}

	

	template<>
	void Push<unsigned int>(unsigned int count) {
		Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += count * VBElements::GetTypeSize(GL_UNSIGNED_INT);
	}

	inline const std::vector<VBElements>& GetElements() const { return Elements; }
	inline unsigned int GetStride() const { return stride; }
};

