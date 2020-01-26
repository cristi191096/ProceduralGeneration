#pragma once
#include <GL\glew.h>
#include <vector>

struct Vertex;

class VertexBuffer
{

	unsigned int bufferID;

public:
	VertexBuffer();
	VertexBuffer(const std::vector<Vertex>& data, unsigned int size);
	VertexBuffer(const float data[]);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};

