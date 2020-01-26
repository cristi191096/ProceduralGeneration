#version 330 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;


out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out vec3 pass_T;
out mat3 TangentSpace;
out float Time;

out float vHeight;


mat3 TBN;
mat3 normalMat;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

uniform vec4 clippingPlane;

void main()
{

	vec4 worldPosition = model * vec4(aPos, 1.0);

	gl_ClipDistance[0] = dot(worldPosition, clippingPlane);
	
	FragPos = vec3(model * vec4(aPos, 1.0));
	vHeight = aPos.y;
    TexCoords = aTexCoords;    
	normalMat = transpose(inverse(mat3(model)));
	Normal = normalMat * aNormal;
	
	vec3 T = normalize(normalMat * aTangent);
	vec3 N = normalize(normalMat * aNormal);
	vec3 B = normalize(normalMat * aBitangent);
	T = normalize(T - dot(T, N) * N); //Gram-Schmidt (reorganise vectors to be perpendicular to each other again.)
	
	
	TBN = transpose(mat3(
	T.x, B.x, N.x, 
	T.y, B.y, N.y, 
	T.z, B.z, N.z ));
	pass_T = T;
	

	TangentSpace = TBN;

	Time = time;
	//Calculate tangent light positions
	//Calculate tangent viewPos

    gl_Position = projection * view * worldPosition;
}