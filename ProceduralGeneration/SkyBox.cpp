#include "SkyBox.h"



SkyBox::SkyBox()
{

	

	//material = new Material("Resources/Shaders/SkyboxShader.vs", "Resources/Shaders/SkyboxShader.fs");
	//material->textures.push_back(Texture(textureFiles));
	skyboxShader = new Shader("Resources/Shaders/SkyboxShader.vs", "Resources/Shaders/SkyboxShader.fs");
	cubeMap = new Texture(textureFiles);
	skyboxShader->Bind();
	skyboxShader->SetUniform1i("skybox", 0);
	//skyboxVAO = new VertexArray();
	//skyboxVBO = new VertexBuffer(vetices);
	//layout = new VertexBufferLayout();
	//layout->Push<float>(3);
	////skyboxVAO->Bind();
	//skyboxVAO->AddBuffer(*skyboxVBO, *layout);
	//skyboxVAO->Unbind();
	//skyboxShader->Unbind();
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void SkyBox::Draw() const
{
	//glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL); //depth test passes when values are equal to depth buffer's content
		
	//skyboxVAO->Bind();
	glBindVertexArray(skyboxVAO);
	cubeMap->Bind(0, GL_TEXTURE_CUBE_MAP);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//skyboxVAO->Unbind();
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); //set it back to default
	//glDepthMask(GL_TRUE);
}



SkyBox::~SkyBox()
{
}
