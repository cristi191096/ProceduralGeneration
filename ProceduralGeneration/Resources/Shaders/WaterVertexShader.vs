#version 330 core


const float PI = 3.1415926535897932384626433832795;

const float waveLength = 500.0;
const float waveAmplitude = 1.0;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;


out float Time;
out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;




out float vHeight;

mat3 TBN;
mat3 normalMat;

uniform float time;
uniform vec4 adjacentVertices;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


uniform vec4 clippingPlane;

float generateOffset(float x, float z, float val1, float val2){
	float radiansX = ((mod(x+z*x*val1, waveLength)/waveLength) + time * mod(x * 0.8 + z, 1.5)) * 2.0 * PI;
	float radiansZ = ((mod(val2 * (z*x +x*z), waveLength)/waveLength) + time * 2.0 * mod(x , 2.0) ) * 2.0 * PI;
	return waveAmplitude * 0.5 * (sin(radiansZ) + cos(radiansX));
}

vec3 applyDistortion(vec3 vertex){
	float xDistortion = generateOffset(vertex.x, vertex.z, 0.2, 0.1);
	float yDistortion = generateOffset(vertex.x, vertex.z, 0.15, 0.3);
	float zDistortion = generateOffset(vertex.x, vertex.z, 0.3, 0.2);
	return vertex + vec3(xDistortion, yDistortion, zDistortion);
}

vec3 calculateNormal(vec3 vertex1, vec3 vertex2){
	//vec3 tangent = vertex1 - vertex0;
	//vec3 bitangent = vertex2 - vertex0;
	return normalize(cross(vertex1, vertex2));
}


void main()
{
	vec3 currentVertex = aPos;
	//vec3 vertex1 = currentVertex + vec3(adjacentVertices.x, 0.0, adjacentVertices.y);
	//vec3 vertex2 = currentVertex + vec3(adjacentVertices.z, 0.0, adjacentVertices.w);

	currentVertex = applyDistortion(currentVertex);
	//vec3 Tangent = applyDistortion(aTangent);
	//vec3 Bitangent = applyDistortion(aBitangent);



	vec4 worldPosition = model * vec4(currentVertex, 1.0);

	gl_ClipDistance[0] = dot(worldPosition, clippingPlane);
	
	FragPos = vec3(model * vec4(aPos, 1.0));
	vHeight = aPos.y;
    TexCoords = aTexCoords;    
	
	Normal = calculateNormal(aTangent, aBitangent);

	//Calculate tangent light positions
	//Calculate tangent viewPos

    gl_Position = projection * view * worldPosition;
}