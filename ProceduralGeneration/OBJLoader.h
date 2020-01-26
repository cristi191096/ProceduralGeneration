#pragma once
#include "glm.hpp"
#include <string>
#include <vector>
#include <fstream>
#include "Mesh.h"


class OBJLoader
{
	
	// Loaded Vertex Objects
	static std::vector<Vertex> LoadedVertices;
	// Loaded Index Positions
	static std::vector<unsigned int> LoadedIndices;
	// Loaded Material Objects
	static std::vector<Material> LoadedMaterials;


	//Split a string into a list of strings at a given token (eg. if we have a vertex coordinates into one string, split it into x,y,z) 
	static void split(const std::string& in, std::vector<std::string>& out, std::string token);
	//Get tail of string after first token
	static std::string tail(const std::string& in);
	//Get first token of string
	static std::string firstToken(const std::string& in);


	//Load materials from .mtl
	static bool LoadMaterials(std::string path);

	static void GenVerticesFromRawOBJ(std::vector<Vertex>& oVerts,
		const std::vector<glm::vec3>& iPositions,
		const std::vector<glm::vec2>& iTCoords,
		const std::vector<glm::vec3>& iNormals,
		std::string icurline);

	template <class T>
	static const T& getElement(const std::vector<T>& elements, std::string& index);

	static float angleBetween2Vec(const glm::vec3 a, const glm::vec3 b);

	// Check to see if a Vector3 Point is within a 3 Vector3 Triangle
	static bool inTriangle(glm::vec3 point, glm::vec3 tri1, glm::vec3 tri2, glm::vec3 tri3);

	// Triangulate a list of vertices into a face by printing
	//	inducies corresponding with triangles within it
	static void VertexTriangluation(std::vector<unsigned int>& oIndices,
		const std::vector<Vertex>& iVerts);

public:
	// Loaded Mesh Objects
	static std::vector<Mesh> LoadedMeshes;

	OBJLoader();
	~OBJLoader();

	static bool LoadFile(std::string Path);
};

template<class T>
inline const T & OBJLoader::getElement(const std::vector<T>& elements, std::string & index)
{
	// TODO: insert return statement here
	int idx = std::stoi(index);
	if (idx < 0)
		idx = int(elements.size()) + idx;
	else
		idx--;
	return elements[idx];
}
