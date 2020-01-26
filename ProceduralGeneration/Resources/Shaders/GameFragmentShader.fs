#version 330 core

#define MAX_LIGHTS 10

out vec4 FragColor;

//In vars
in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;
in vec3 pass_T;
in mat3 TangentSpace;
in float vHeight;
in float Time;
//in Lights!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//Structs
struct Material {
sampler2D texture_diffuse1;
   sampler2D texture_diffuse2;
   sampler2D texture_diffuse3;
   sampler2D texture_diffuse4;
   sampler2D texture_diffuse5;
   sampler2D texture_diffuse6;
sampler2D texture_specular1;
sampler2D texture_emission1;
sampler2D texture_normal1;
vec3 diffuseCol;
vec3 specularCol;
int emissionAmp;
int emissionBehaviour;
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
uniform int NUM_LIGHTS;
uniform vec3 viewPos;

uniform samplerCube skybox;

//Prototypes
vec4 CalculateDirLight(DirLight light, vec4 texColor, vec3 normal, vec3 viewDir);
vec4 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float cutoff);
vec4 CalculateTexture();

void main()
{   
   

vec3 norm = (255.0/128.0 * texture(material.texture_normal1, TexCoords).rgb - 1.0) * (2.0 * Normal - 1.0); //in tangent space (-1, 1).
norm = normalize(norm);
//norm = normalize(TangentSpace * norm);
vec3 viewDir = normalize(TangentSpace * viewPos - TangentSpace * FragPos);

   vec4 textureColor = CalculateTexture();

vec4 result = CalculateDirLight(dirLight, textureColor, norm, viewDir);

for(int i=0; i<NUM_LIGHTS; i++)
{
result += CalculatePointLight(pointLight[i], norm, FragPos, viewDir, 0.3);

}

//Emission
//	vec3 emission = vec3(0.0);
//	
//	emission = texture(material.texture_emission1, TexCoords).rgb * material.emissionAmp;	//emissionAmp is 0 if the texture exists and 1 otherwise
//	
//	if(material.emissionBehaviour == 1){ //If 0 do nothing
//		emission = emission * (sin(Time) * 0.5 + 0.5) * 2.0; //fading
//	}
//	else if(material.emissionBehaviour == 2){
//		emission  =texture(material.texture_emission1, TexCoords + vec2(0.0, Time)).rgb * material.emissionAmp; //moving Up
//	}
//	else if(material.emissionBehaviour == 3){
//			emission  =texture(material.texture_emission1, TexCoords + vec2(0.0, -Time)).rgb * material.emissionAmp; //moving Down
//	}
//	else if(material.emissionBehaviour == 4){
//		emission  =texture(material.texture_emission1, TexCoords + vec2(-Time, 0.0)).rgb * material.emissionAmp; //moving Left
//	}
//	else if(material.emissionBehaviour == 5){
//			emission  =texture(material.texture_emission1, TexCoords + vec2(Time, 0.0)).rgb * material.emissionAmp; //moving Right
//	}
//
//	result += emission;
//vec3 col = vec3(texture(material.texture_diffuse1, TexCoords)) * material.diffuseCol;
//vec3 result = ambient + diffuse + specular + emission;
    FragColor =  result;



}

vec4 CalculateTexture()
{
       float levels[7] = float[7](-20.0, -10.0, 5.0, 15.0, 20.0, 25.0, 30.0 );

    vec4 terrainSampler[6];
    terrainSampler[0] = texture(material.texture_diffuse4, TexCoords);
    terrainSampler[1] = texture(material.texture_diffuse2, TexCoords);
    terrainSampler[2] = texture(material.texture_diffuse1, TexCoords);
    terrainSampler[3] = texture(material.texture_diffuse3, TexCoords);
    terrainSampler[4] = texture(material.texture_diffuse5, vec2(TexCoords.x * 7.0, TexCoords.y * 7.0));
    terrainSampler[5] = texture(material.texture_diffuse6, vec2(TexCoords.x - (0.2 * Time), TexCoords.y ));

    vec4 mixture = vec4(0.0);
    bool isMixtureSet = false;

    for(int i = 0; i < levels.length() && !isMixtureSet; i++)
    {
        if(vHeight > levels[i]) {
            continue;
        }

        int currentSamplerIndex = i / 2;

        if(i % 2 == 0) {
            mixture = terrainSampler[currentSamplerIndex];
        }
        else
        {
            int nextSamplerIndex = currentSamplerIndex+1;
            vec4 textureColorCurrent = terrainSampler[currentSamplerIndex];
            vec4 textureColorNext =  terrainSampler[nextSamplerIndex];

            float levelDiff = levels[i] - levels[i-1];
            float factorNext = (vHeight - levels[i-1]) / levelDiff;
            float factorCurrent = 1.0f - factorNext;
         
            mixture = textureColorCurrent*factorCurrent + textureColorNext*factorNext;
        }

        isMixtureSet = true;
    }

    if(!isMixtureSet)
    {
        int lastSamplerIndex = levels.length() / 2;
        mixture = terrainSampler[lastSamplerIndex];
    }

    if(terrainSampler[0].r == 0.0 
        && terrainSampler[0].g == 0.0
        && terrainSampler[0].b == 0.0) {
        mixture = terrainSampler[4];
    }

    if(terrainSampler[0].r == 0.0 
        && terrainSampler[0].g == 0.0
        && terrainSampler[0].b == 0.0
        && terrainSampler[4].r == 0.0 
        && terrainSampler[4].g == 0.0
        && terrainSampler[4].b == 0.0) {
        if( terrainSampler[5].a <= 0.0 ) { discard; }
        else {
            mixture = vec4(1.0, 1.0, 1.0, terrainSampler[5].a / 1.0);
        }
    }

    return mixture;
}

vec4 CalculateDirLight(DirLight light, vec4 texColor, vec3 normal, vec3 viewDir)
{
vec3 lightDir = normalize(TangentSpace * (-light.direction));



//ambient
vec4 ambient = vec4(light.ambient,1.0) * texColor * vec4(material.diffuseCol,1.0);
//diffuse
float diff = max(dot(normal, lightDir), 0.0);

vec4 diffuse = vec4(light.diffuse,1.0) * diff * texColor * vec4(material.diffuseCol,1.0);

//Specular
vec3 reflectDir = reflect(-lightDir, normal);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
vec4 specular = vec4(light.specular,1.0) * spec * texture(material.texture_specular1, TexCoords) * vec4(material.specularCol,1.0);

return (ambient + diffuse + specular);
}

vec4 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float cutoff)
{
vec3 lightDir = normalize(TangentSpace * light.position - fragPos);
//ambient
vec4 ambient = vec4(light.ambient, 1.0) * texture(material.texture_diffuse1, TexCoords) * vec4(material.diffuseCol,1.0);
//diffuse
float diff = max(dot(normal, lightDir), 0.0);
vec4 diffuse = vec4(light.diffuse,1.0) * diff * texture(material.texture_diffuse1, TexCoords) * vec4(material.diffuseCol,1.0);
//specular
vec3 reflectDir = reflect(-lightDir, normal);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
vec4 specular = vec4(light.specular,1.0) * spec * texture(material.texture_specular1, TexCoords) * vec4(material.specularCol,1.0);

//Attenuation
float distance = length(TangentSpace * light.position - fragPos);
float attenuation = light.intensity / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
attenuation = (attenuation - cutoff) / (1 - cutoff);
attenuation = max(attenuation, 0.0);

ambient *= attenuation;
diffuse *= attenuation;
specular *= attenuation;

return (ambient + diffuse + specular);
}