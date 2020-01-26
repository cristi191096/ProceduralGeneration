#shader vertex
#version 330 core



layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoords;

//struct Object {
//	string tag = "Ball";
//
//};
struct Material {
	vec3 ambientColour;
	vec3 DiffuseColour;
	vec3 SpecularColour;
	float Dissolve;
	int illum;
};

uniform Material u_Material;

//uniform vec3 ambientColour;
uniform mat4 u_ProjectionMat;
uniform mat4 u_ModelMat;
uniform mat4 u_ViewMat;

out vec2 v_UV;
out vec3 v_Normal;
out vec4 MatAmbient;
out vec4 MatDiffuse;
out vec4 MatSpecular;
//out Material v_Material;

void main()
{
	gl_Position = u_ProjectionMat  * u_ViewMat * u_ModelMat * vec4(position, 1.0);
	MatAmbient = vec4(u_Material.ambientColour, u_Material.Dissolve);
	MatDiffuse = vec4(u_Material.DiffuseColour, u_Material.Dissolve);
	MatSpecular = vec4(u_Material.SpecularColour, 1.0);

	v_Normal = (u_ViewMat * u_ModelMat * vec4(normals, 0.0)).xyz;
	v_UV = texCoords;
}										

#shader fragment
#version 330 core

in vec2 v_UV;
in vec3 v_Normal;
in vec4 MatAmbient;
in vec4 MatDiffuse;
in vec4 MatSpecular;

out vec4 colour;

uniform sampler2D texture_diffuse1;

void main()
{
	vec4 col = MatAmbient + MatDiffuse + MatSpecular;
	colour = texture(texture_diffuse1, v_UV)
		* clamp(dot(-vec3(0,0,1), v_Normal), 0.0, 1.0);
	//				light direction

}						  