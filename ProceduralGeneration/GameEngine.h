#pragma once

#include "Renderer.h"
#include "Camera.h"
#include "GameObject.h"
#include "FrameBuffer.h"
#include "SkyBox.h"
#include <iostream>
#include <vector>
#include <string>

#define MAX_LIGHTS 10 //Should be the same number as in fragment shader


class GameEngine
{
	//Mouse handling
	static bool  firstMouse;
	//=======
	static std::vector<GameObject*> gameobjects; ///<Vector of (pointers to) game objects: all the objects in the game.
	static SkyBox* skybox;

	static glm::mat4 projectionMat;
	static glm::mat4 modelView;
	static int 	oldTimeSinceStart;
	static int	newTimeSinceStart;
	static int deltaTime;
	
	static float fadingTimer;
	static void FixedUpdate();
	static void updateGame(); ///<Function that updates the game state
	static void startEngine(); ///<Start the engine (and the main game loop).
public:
	static Camera camera;
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;
	static FrameBuffer* waterBuffers;
	static Shader* currentShader;
	static int pointLights; //Counting the number of point lights
	static std::vector<Shader*> allShaders;
	static void initEngine(int argc, char **argv, const char* windowTitle = "New Game", bool debugMode = false, int width = 500, int height = 500);
	static void displayFunc();
	static void reshapeFunc(int, int);
	static void addGameObject(GameObject* gameobject); ///<Adds a GameObject to the game.
	static void cleanup(); ///<Function to clean up and delete any pointers and anything else when the game exists to prevent memory leaks.
	static void MouseEventTrigger(int button, int state, int x, int y);
	static void KeyboardEvent(unsigned char key, int x, int y);
	static void KeyboardSpecialEvent(int key, int x, int y);
	static void Start();
	static void Update(float deltaTime);
};

