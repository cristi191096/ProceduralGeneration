#include "GameEngine.h"
#include <stdlib.h>
#include <time.h>

FrameBuffer* GameEngine::waterBuffers = nullptr;
Shader* GameEngine::currentShader = nullptr;
std::vector<Shader*> GameEngine::allShaders;
Camera GameEngine::camera(glm::vec3(-215.0f, 527.0f, 1753.0f));
SkyBox* GameEngine::skybox = nullptr;
std::vector<GameObject*> GameEngine::gameobjects;
glm::mat4 GameEngine::projectionMat = glm::mat4();
glm::mat4 GameEngine::modelView = glm::mat4();
int GameEngine::oldTimeSinceStart;
int GameEngine::newTimeSinceStart;
int GameEngine::deltaTime;
int GameEngine::SCREEN_WIDTH;
int GameEngine::SCREEN_HEIGHT;
int GameEngine::pointLights = 0;
float GameEngine::fadingTimer = 0;


bool  GameEngine::firstMouse = true;



#pragma region INIT
void GameEngine::initEngine(int argc, char **argv, const char* windowTitle, bool debug, int width, int height)
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	camera.SetMovementSpeed(400.0);
	camera.SetMouseSensitivity(0.4);
	GameObject::debugMode = debug;
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(38, 0);
	glutCreateWindow(windowTitle);

	glutDisplayFunc(displayFunc);

	glutReshapeFunc(reshapeFunc);

	glutKeyboardFunc(KeyboardEvent);
	glutKeyboardUpFunc([](unsigned char key, int x, int y) {
		GameObject::keys[key] = false;
	});
	glutSpecialFunc(KeyboardSpecialEvent);
	glutSpecialUpFunc([](int key, int x, int y) {
		GameObject::specialKeys[key] = false;
	});

	glutMouseFunc(MouseEventTrigger);

	glutMotionFunc([](int x, int y) {
		//if (firstMouse)
		//{
		//	lastX = x;
		//	lastY = y;
		//	firstMouse = false;
		//}

		//float xoffset = x - lastX;
		//float yoffset = y - lastY; 
		//
		////std::cout << "X: " << xoffset << std::endl;
		////std::cout << "Y: " << yoffset << std::endl;
		//lastX = x;
		//lastY = y;


	});

	glutIdleFunc(updateGame);

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_RGBA);
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
	GameEngine::startEngine();
}
#pragma endregion Initialise glut, glew and some variables

void GameEngine::displayFunc() 
{
	Renderer::Clear();

	for (int i = 0; i < allShaders.size(); i++) {
		allShaders[i]->Bind();
		allShaders[i]->SetUniformMat4("view", camera.GetView());
		allShaders[i]->SetUniformVec3("viewPos", camera.Position);
		allShaders[i]->SetUniform1f("time", fadingTimer);
	}
	
	//Material::shader->SetUniformMat4("u_ModelViewMat", modelView);

	//Don't do frame buffers. Calculate the normals and move verts.

	//for (int i = 0; i < gameobjects.size(); i++) {
	//	if (gameobjects[i]->waterObject)
	//	{
	//		GameEngine::waterBuffers->bindReflectionFrameBuffer();
	//		float distance = 2 * (GameEngine::camera.Position.y - this->vertices[0].position.y);
	//		GameEngine::camera.Position.y -= distance;
	//		GameEngine::camera.InvertPitch();
	//		this->meshMaterial->shader->SetUniformVec4("clippingPlane", glm::vec4(0, -1, 0, -this->vertices[0].position.y));
	//		//		Renderer::Draw(VAO, indexBuffer, vertices.size(), DrawType::ARRAYS, true);
	//		GameEngine::camera.Position.y += distance;
	//		GameEngine::camera.InvertPitch();
	//
	//
	//
	//
	//		GameEngine::waterBuffers->bindRefractionFrameBuffer();
	//		this->meshMaterial->shader->SetUniformVec4("clippingPlane", glm::vec4(0, 1, 0, this->vertices[0].position.y));
	//		//	Renderer::Draw(VAO, indexBuffer, vertices.size(), DrawType::ARRAYS, true);
	//
	//
	//		GameEngine::waterBuffers->unbindCurrentFrameBuffer();
	//
	//		//		glActiveTexture(GL_TEXTURE0);
	//		//	glBindTexture(GL_TEXTURE_2D, GameEngine::waterBuffers->getReflectionTexture());
	//		//	this->meshMaterial->shader->SetUniform1i("material.texture_diffuse7", 0);
	//		//
	//		//		glActiveTexture(GL_TEXTURE1);
	//		//	glBindTexture(GL_TEXTURE_2D, GameEngine::waterBuffers->getRefractionTexture());
	//		//	this->meshMaterial->shader->SetUniform1i("material.texture_diffuse8", 1);
	//
	//	}
	//}
		
	glm::mat4 view = glm::mat4(glm::mat3(camera.GetView())); // remove translation from the view matrix
	skybox->skyboxShader->Bind();
	skybox->skyboxShader->SetUniformMat4("view", view);
	skybox->Draw();

	for (int i = 0; i < gameobjects.size(); i++) {
		gameobjects[i]->draw();		
	}


	//glm::mat4 model(1.0);
	//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	////model = glm::rotate(model, (GLfloat)glm::radians(0.2) * glutGet(GLUT_ELAPSED_TIME), glm::vec3(0.0, 1.0, 0.0));
	//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	//currentShader->SetUniformMat4("model", model);



	glutSwapBuffers();
}

void GameEngine::reshapeFunc(int w, int h) 
{
	SCREEN_WIDTH = w;
	SCREEN_HEIGHT = h;
	glViewport(0, 0, w, h);
}

void GameEngine::addGameObject(GameObject * gameobject)
{
	
	gameobjects.push_back(gameobject);
	gameobject->start();
}

#pragma region UPDATE
void GameEngine::updateGame() {

	//Calculate deltaTime
	oldTimeSinceStart = newTimeSinceStart;
	newTimeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = newTimeSinceStart - oldTimeSinceStart;

	if (deltaTime == 0) {
		Sleep(1);
		newTimeSinceStart = glutGet(GLUT_ELAPSED_TIME);
		deltaTime = newTimeSinceStart - oldTimeSinceStart;
	}

	fadingTimer +=  (float)deltaTime / 1000;


	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		gameobjects[i]->update(deltaTime);
	}

	GameEngine::Update((float)deltaTime / 1000); //Run other game Updates 

	glutPostRedisplay();
}
#pragma endregion update the game status every frame

#pragma region START_ENGINE
void GameEngine::startEngine() {
	
	//srand(time(NULL));
//	GameEngine::waterBuffers = new FrameBuffer();
	GameEngine::Start();
	skybox = new SkyBox();
	camera.Perspective(45.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.2f, 2000.0f);

	//Send this to all shaders
	skybox->skyboxShader->Bind();
	skybox->skyboxShader->SetUniformMat4("projection", camera.GetProjection());

	for (int i = 0; i < allShaders.size(); i++) {
		allShaders[i]->Bind();
		allShaders[i]->SetUniformMat4("projection", camera.GetProjection());
		allShaders[i]->SetUniform1i("skybox", 0);
		allShaders[i]->SetUniform1i("NUM_LIGHTS", pointLights);
		allShaders[i]->Unbind();
	}
	
	if (pointLights > MAX_LIGHTS) {
		std::cout << "TOO MANY LIGHTS!" << std::endl;
	}
	
	glutMainLoop();
}
void GameEngine::cleanup()
{
}
#pragma endregion aka setup()