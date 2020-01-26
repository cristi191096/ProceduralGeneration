#version 330 core

#define MAX_LIGHTS 10

const float Eta = 0.15; // Water

out vec4 FragColor;

//In vars
in float Time;
in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;
in vec3 pass_T;

//in Lights!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//Structs
struct Material {

vec3 diffuseCol;
vec3 specularCol;
float shininess;
};



//Lights
struct DirLight{
vec3 direction;

vec3 ambient;
vec3 diffuse;
vec3 specular;
};

struct PointLight {
vec3 position;

vec3 ambient;
vec3 diffuse;
vec3 specular;

float intensity;

float constant;
    float linear;
    float quadratic;
};




//Uniforms
//uniform sampler2D texture_diffuse1;
//uniform vec3 colour;

uniform Material material;
uniform PointLight pointLight[MAX_LIGHTS];
uniform DirLight dirLight;
uniform vec3 viewPos;
uniform samplerCube skybox;

vec4 mixedFresnelColor;

//Prototypes
vec4 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir);


void main()
{   

    //DirLight someLight;
    //someLight.direction = vec3(-0.2, -1.0, -0.3);
    //someLight.ambient = vec3(0.2,0.2,0.2);
    //someLight.diffuse = vec3(1.0,1.0,1.0);
    //someLight.specular = vec3(1.0,1.0,1.0);
//	vec3 norm = (255.0/128.0 ) * (2.0 * Normal - 1.0); //in tangent space (-1, 1).
vec3 norm = normalize(Normal);

    vec3 I = normalize(FragPos - viewPos);

    vec3 reflectionVector = reflect(I, norm);
    vec3 refractionVector = refract(I, norm, Eta);
    //FragColor = vec4(texture(skybox, R).rgb, 1.0);

    vec4 reflectionColor = texture(skybox, reflectionVector);
    vec4 refractionColor = texture(skybox, refractionVector);

    float fresnel = Eta + (1.0 - Eta) * pow(max(0.0, 1.0 - dot(-I, norm)), 5.0);

vec3 viewDir = normalize(viewPos - FragPos);
     mixedFresnelColor = mix(refractionColor, reflectionColor, fresnel);

vec4 lighting = CalculateDirLight(dirLight, Normal, viewDir);

   // lighting = mix(lighting, vec3(0.0, 0.3, 0.5), fresnel);
   // vec4 result = vec4(lighting.r + mixedFresnelColor.r, lighting.g + mixedFresnelColor.g, lighting.b + mixedFresnelColor.b, mixedFresnelColor.a);
 
    FragColor =  vec4(lighting.rgb, distance(FragPos, viewPos) / 200.0);


}



vec4 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
vec3 lightDir = normalize(-light.direction);

//ambient
vec4 ambient = vec4( light.ambient, 1.0) * mixedFresnelColor;
//diffuse
float diff = max(dot(normal, lightDir), 0.0);

vec4 diffuse = vec4(light.diffuse, 1.0) * diff * mixedFresnelColor;

//Specular
vec3 reflectDir = reflect(-lightDir, normal);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
vec4 specular = vec4( light.specular, 1.0) * spec  * vec4(material.specularCol,1.0);

return (ambient + diffuse + specular);
}

