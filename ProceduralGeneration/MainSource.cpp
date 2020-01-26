
#include "GameEngine.h"

#include "Rect.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include <stdlib.h>
#include <random>
#include <thread>
#include <gtx\rotate_vector.hpp>

#include <time.h>


#define ASSERT(x) if(!(x)) __debugbreak();
#define FOREST_SIZE 100
#define TERRAIN_SIZE 1025

typedef unsigned int uint;

struct MeshData
{
	std::vector<Vertex> verts;
	std::vector<uint>	inds;
};

DirectionalLight* dirLight = nullptr;
PointLight* light = nullptr;
static std::mt19937 generator(time(0));
static MeshData terrainData;
static MeshData waterData;


std::string axiom = "VZFFF";
std::string sentence = axiom;

std::map<std::string, std::string> rules;

//Rect* arrow = nullptr;  //I tried. Didn't work due to some run time errors.

void CreateGeneration() {
	std::string nextSentence = "";
	for (int i = 0; i < sentence.size(); i++)
	{
		std::string current = std::string(1, sentence.at(i));
		bool found = false;
		for (auto& rule : rules)
		{
			if (current == rule.first)
			{
				nextSentence += rule.second;
				found = true;
				break;
			}

		}
		if (!found)
		{
			nextSentence += current;
		}
	}

	sentence = nextSentence;
}

MeshData GenerateTree(glm::vec3 position, int generations, float length)
{
	std::vector<Vertex> verts;
	std::vector<uint>	inds;

	glm::vec3 direction = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 normal = glm::vec3(0.f, 0.f, 1.f);
	glm::vec2 uv = glm::vec2(0.0f);
	float angle = glm::radians(20.0f);

	verts.push_back({
		glm::vec3(0.0f),
		normal,
		uv,
		glm::vec3(0.0f),
		glm::vec3(0.0f)
	});
	inds.push_back(0);

	int currentIndex = 0;
	int previousIndex = 0;
	std::vector<int> pushedIndex;
	std::vector<glm::vec3> pushedRotation;

	for (int i = 0; i < generations; i++)
		CreateGeneration();

	for (int i = 0; i < sentence.size(); i++)
	{
		char current = sentence.at(i);



		if (current == 'F')
		{
			//Forward
			verts.push_back({
				verts[previousIndex].position + length * direction,
				normal,
				uv,
				glm::vec3(0.0f),
				glm::vec3(0.0f)
			});
			currentIndex++;

			inds.push_back(currentIndex);
			inds.push_back(currentIndex);
			previousIndex = currentIndex;
		}
		else if (current == '+')
		{
			//Rotate right
			direction = glm::rotateZ(direction, angle);
		}
		else if (current == '-')
		{
			//Rotate right
			direction = glm::rotateZ(direction, -angle);
		}
		else if (current == '[')
		{
			//Push
			pushedIndex.push_back(previousIndex);
			pushedRotation.push_back(direction);
		}
		else if (current == ']')
		{
			//Pop
			inds.push_back(previousIndex);
			previousIndex = pushedIndex.back();
			inds.push_back(pushedIndex.back());
			pushedIndex.pop_back();

			direction = pushedRotation.back();
			pushedRotation.pop_back();
		}
	}


	return { verts, inds };
}


float GenerateRandom(float min, float  max)
{
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(generator);
	//return -min + static_cast<float>(rand() / static_cast<float>(RAND_MAX / min + min));
	// max + rand() / (float)(RAND_MAX / max + min)
}

void Square(std::vector<std::vector<float>>& surface, int row, int col, int squareSize, float height)
{
	int avgX = row + squareSize * 0.5f;		
	int avgY = col + squareSize * 0.5f;

	//Square step
	float avgHeight1 = (surface[avgX][avgY] + surface[row][col] + surface[row][col+squareSize])/3.0f;
	float avgHeight2 = (surface[avgX][avgY] + surface[row][col] + surface[row+squareSize][col])/3.0f;
	float avgHeight3 = (surface[avgX][avgY] + surface[row + squareSize][col] + surface[row + squareSize][col + squareSize]) / 3.0f ;
	float avgHeight4 = (surface[avgX][avgY] +  surface[row+squareSize][col+squareSize] + surface[row][col+squareSize])/3.0f;

	surface[avgX][col]					= avgHeight2 + GenerateRandom(-height, height);
	surface[row + squareSize][avgY]		= avgHeight3 + GenerateRandom(-height, height);
	surface[avgX][col + squareSize]		= avgHeight4 + GenerateRandom(-height, height);
	surface[row][avgY]					= avgHeight1 + GenerateRandom(-height, height);
}

void Diamond(std::vector<std::vector<float>>& surface, int row, int col, int squareSize, float height)
{
	int avgX = row + squareSize * 0.5f;		
	int avgY = col + squareSize * 0.5f;
	float avgHeight = (surface[row][col] + surface[row+squareSize][col] + surface[row+squareSize][col+squareSize] + surface[row][col+squareSize]) * 0.25f;

	//Diamond step
	surface[avgX][avgY] = avgHeight + GenerateRandom(-height, height); 

}


MeshData GenerateTerrain(int mapSize, float height) {
	//if ((mapSize - 1) % 2 != 0) {
		//	std::cout << "Map size should be: 2n-1" << std::endl;
		//	return nullptr;
		//}

		std::vector<Vertex> terrain;
		std::vector<uint>	indices;
		terrain.reserve(mapSize * mapSize);
		std::vector<std::vector<float>> surface;

		// Initialise terrain - set values in the height map to 0
		for (int x = 0; x < mapSize; x++)
		{
			std::vector<float> temp;
			for (int z = 0; z < mapSize; z++)
			{
				temp.push_back(-2);
			}
			surface.push_back(temp);
		}

		//TODO: Add your code here to calculate the height values of the terrain using the Diamond square algorithm
		surface[0][0] = GenerateRandom(-height, height);
		surface[0][mapSize - 1] = GenerateRandom(-height, height);
		surface[mapSize - 1][0] =  GenerateRandom(-height, height);
		surface[mapSize - 1][mapSize - 1] = GenerateRandom(-height, height);

		int n = log2(mapSize);
		int squareSize = mapSize - 1;
		int numberOfSquares = 1;
		//float rand_max = (float)height;
		for (int i = 0; i < n; i++) 
		{
		//while(squareSize > 1)
		//{
			
			for (int x = 0; x < numberOfSquares; x++) {
				int row = squareSize*x;
				for (int z = 0; z < numberOfSquares; z++) {
					int col = z* squareSize;
					Diamond(surface, row, col, squareSize, height);
				}
				//height *= 0.8f;
			}

			for (int x = 0; x < numberOfSquares; x++) {
				int row = squareSize*x;
				for (int z = 0; z < numberOfSquares; z++) {
					int col = z* squareSize;
					Square(surface, row, col, squareSize, height);
				}
				//height *= 0.8f;
			}

			numberOfSquares *= 2;
			squareSize *= 0.5f;
			height *= 0.5f;
			//height *= pow(2, -H);

			//height *= 0.8f;
	
		}



		//=======================TextureCoordinates============================
		//std::vector<std::vector<glm::vec2>> textureCoordinates = std::vector<std::vector<glm::vec2>>(mapSize, std::vector<glm::vec2>(mapSize));

		const float U = 5.f;
		const float V = 5.f;

	
		//====================Normals=====================================
	//	std::vector<std::vector<glm::vec3>> normals = std::vector<std::vector<glm::vec3>>(mapSize, std::vector<glm::vec3>(mapSize));
		std::vector<std::vector<glm::vec3>> tangents = std::vector<std::vector<glm::vec3>>(mapSize, std::vector<glm::vec3>(mapSize));
		std::vector<std::vector<glm::vec3>> bitangents = std::vector<std::vector<glm::vec3>>(mapSize, std::vector<glm::vec3>(mapSize));

		std::vector<std::vector<glm::vec3>> tempNormals[2];

		for (int i = 0; i < 2; i++) {
			tempNormals[i] = std::vector<std::vector<glm::vec3>>(mapSize - 1, std::vector<glm::vec3>(mapSize - 1));
		}

		for (int i = 0; i < mapSize - 1; i++) 
		{
			for (int j = 0; j < mapSize - 1; j++)
			{
				const glm::vec3& vertexA = glm::vec3((float)i, surface[i][j] ,(float)j);
				const glm::vec3& vertexB = glm::vec3((float)i, surface[i][j + 1], (float)j + 1);
				const glm::vec3& vertexC = glm::vec3((float)i + 1, surface[i + 1][j + 1], (float)j + 1);
				const glm::vec3& vertexD = glm::vec3((float)i + 1, surface[i + 1][j], (float)j);

				const glm::vec3 triangleNormalA = glm::cross(vertexB - vertexA, vertexA - vertexD);
				const glm::vec3 triangleNormalB = glm::cross(vertexD - vertexC, vertexC - vertexB);

				tangents[i][j] = vertexB - vertexA;
				bitangents[i][j] = vertexD - vertexA;

				tempNormals[0][i][j] = glm::normalize(triangleNormalA);
				tempNormals[1][i][j] = glm::normalize(triangleNormalB);
			}
		}
		
		//=============================================================

			// Intialise vertex array
		for (uint x = 0; x < mapSize; x++) {
			for (uint z = 0; z < mapSize; z++) {


				//Calculate normals
				  const bool isFirstRow = x == 0;
				  const bool isFirstColumn = z == 0;
				  const bool isLastRow = x == mapSize - 1;
				  const bool isLastColumn = z == mapSize - 1;

				  glm::vec3 finalVertexNormal = glm::vec3(0.0f, 0.0f, 0.0f);

				        // Look for triangle to the upper-lef
				  if (!isFirstRow && !isFirstColumn) {
					  finalVertexNormal += tempNormals[0][x - 1][z - 1];
				  }

				// Look for triangles to the upper-righ
				  if (!isFirstRow && !isLastColumn) {
					  for (auto k = 0; k < 2; k++) {
						  finalVertexNormal += tempNormals[k][x - 1][z];
					  }
				  }

				// Look for triangle to the bottom-righ
				  if (!isLastRow && !isLastColumn) {
					  finalVertexNormal += tempNormals[0][x][z];
				  }

				// Look for triangles to the bottom-righ
				  if (!isLastRow && !isFirstColumn) {
					  for (auto k = 0; k < 2; k++) {
						  finalVertexNormal += tempNormals[k][x][z - 1];
					  }
				  }



				//Assign Vertex
				Vertex v = {
					glm::vec3((float)x, surface[x][z] ,(float)z),
					-glm::normalize(finalVertexNormal),
					glm::vec2(U * x / (mapSize - 1), V * z / (mapSize - 1)),
					tangents[x][z],
					bitangents[x][z]
				};

				terrain.push_back(v);
			}
		}


		//Build indices

		
		for (uint x = 0; x < mapSize - 1; x++) {

			uint offset = (x + 1) * mapSize;

			for (uint z = mapSize*x; z < mapSize *(x+1); z++) {
				if (z % mapSize == 0 && x > 0) indices.push_back(z);		//If first row add index again

				indices.push_back(z);
				indices.push_back(offset + z % mapSize);

				if (z % mapSize == mapSize - 1)	indices.push_back(offset + z % mapSize);	//If last row add index again

				
			}

		}

		//while (stepSize > 1)
		//{
		//	for (uint x = 0; x < mapSize - 1; x += stepSize) {
		//		for (uint y = 0; y < mapSize - 1; y += stepSize) {
		//			diamond_step(x, y);
		//		}
		//	}
		//	for (uint x = 0; x < mapSize - 1; x += stepSize) {
		//		for (uint y = 0; y < mapSize - 1; y += stepSize) {
		//			square_step(x, y);
		//		}
		//	}
		//	rand_max = rand_max * pow(2, -H);
		//	stepSize = stepSize / 2;
		//
		//}

		return { terrain, indices };
}

std::vector<glm::vec4> GenerateClouds(int mapSize, float height) {
		std::vector<glm::vec4> output;
		output.reserve(mapSize * mapSize);
		std::vector<std::vector<float>> surface;

		// Initialise terrain - set values in the height map to 0
		for (int x = 0; x < mapSize; x++)
		{
			std::vector<float> temp;
			for (int z = 0; z < mapSize; z++)
			{
				temp.push_back(-2);
			}
			surface.push_back(temp);
		}

		//TODO: Add your code here to calculate the height values of the terrain using the Diamond square algorithm
		surface[0][0] = GenerateRandom(-height, height);
		surface[0][mapSize - 1] = GenerateRandom(-height, height);
		surface[mapSize - 1][0] =  GenerateRandom(-height, height);
		surface[mapSize - 1][mapSize - 1] = GenerateRandom(-height, height);

		int n = log2(mapSize);
		int squareSize = mapSize - 1;
		int numberOfSquares = 1;
		//float rand_max = (float)height;
		for (int i = 0; i < n; i++) 
		{
		//while(squareSize > 1)
		//{
			
			//for (int x = 0; x < numberOfSquares; x++) {
			//	int row = squareSize*x;
			//	for (int z = 0; z < numberOfSquares; z++) {
			//		int col = z* squareSize;
			//		
			//	}
			//	//height *= 0.8f;
			//}

			for (int x = 0; x < numberOfSquares; x++) {
				int row = squareSize*x;
				for (int z = 0; z < numberOfSquares; z++) {
					int col = z* squareSize;
					//if (z % mapSize == 0 && x > 0) 
					if (z % mapSize == mapSize - 1) {
						//surface[x][0] = surface[x][mapSize - 1];
							Diamond(surface, row+1, col+1, squareSize, height);
						Square(surface, row+1, col+1, squareSize, height);
					}
					else {
						Diamond(surface, row, col, squareSize, height);
						Square(surface, row, col, squareSize, height);
					}
				}
				//height *= 0.8f;
			}

			numberOfSquares *= 2;
			squareSize *= 0.5f;
			height *= 0.5f;
			//height *= pow(2, -H);

			//height *= 0.8f;
	
		}

		// Intialise vertex array
		glm::vec4 temp = glm::vec4(0);
		for (uint x = 0; x < mapSize; x++) {
			for (uint z = 0; z < mapSize; z++) {
			//	if (z % mapSize < 20 && x > 0)	temp = glm::vec4(0.0f, 0.0f, 0.0f, surface[x][mapSize - 1]);	//up
			//	//if (z % mapSize == mapSize - 1)			//down
			//	else
					temp = glm::vec4(0.0f, 0.0f, 0.0f, surface[x][z]);
				output.push_back(temp);
			}
		}

		return output;
}

void GameEngine::MouseEventTrigger(int button, int state, int x, int y) {
	
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	
		//arrow = new Rect(1, 1, currentBall->transform->position, "arrow");
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		

	}
}
void GameEngine::KeyboardEvent(unsigned char key, int x, int y) {
	GameObject::keys[key] = true;

	if (key == 27) {

		exit(0);
	}
	if (key == 'a') {
		camera.ProcessKeyboardInput(LEFT, deltaTime);

	}
	if (key == 'd') {
		camera.ProcessKeyboardInput(RIGHT, deltaTime);

	}
	if (key == 'w') {
		camera.ProcessKeyboardInput(FORWARD, deltaTime);

	}
	if (key == 's') {
		camera.ProcessKeyboardInput(BACKWARD, deltaTime);
		//std::cout << "CamPos: " << camera.Position.x << " | " << camera.Position.y << " | " << camera.Position.z << std::endl;
	}
	if (key == 'g') {
	


	}
	if (key == '[') {
		dirLight->transform->rotation.x += 1;

	}
	if (key == ']') {
		dirLight->transform->rotation.z += 1;

	}
	if (key == '{') {
		dirLight->transform->rotation.x -= 1;

	}
	if (key == '}') {
		dirLight->transform->rotation.z -= 1;

	}
	if (key == ' ') {
		light->transform->position.z += 0.1f;
	
		//	light1->transform->position.x -= 0.1f;->transform->position.z -= 0.1f;
		//currentBall->transform->position.z += 0.1f;
	}
	
}

void GameEngine::KeyboardSpecialEvent(int key, int x, int y) {
	GameObject::specialKeys[key] = true;

	if (key == GLUT_KEY_UP) {
		camera.ProcessMouseInput(0, 5);
	}
	if (key == GLUT_KEY_DOWN) {
		camera.ProcessMouseInput(0, -5);
	}
	if (key == GLUT_KEY_LEFT) {
		camera.ProcessMouseInput(-5, 0);
	}
	if (key == GLUT_KEY_RIGHT) {
		camera.ProcessMouseInput(5, 0);
	}
	
}

static void MakeTerrain(int size, float height) {
	terrainData	= GenerateTerrain(size, height);
}

static void MakeWater(int size, float height) {
	waterData		= GenerateTerrain(size, height);
}


static void PrintControls()
{
	std::cout << "================== CAMERA CONTROLS ======================\n";
	std::cout << "| W | A | S | D | --> Move Camera\n";
	std::cout << "| UP | DOWN | LEFT | RIGHT | --> Rotate Camera\n";

	std::cout << "=========================================================\n";
}

void GameEngine::Start() {
	
	//rules.insert(std::pair<char, std::string>('F', "FF"));
	//rules.insert(std::pair<std::string, std::string>("F", "F[+FF][-FF]F[-F][+F]F"));
	PrintControls();

	rules.insert(std::pair<std::string, std::string>("V", "[+++W][---W]YV"));
	rules.insert(std::pair<std::string, std::string>("W", "+X[-W]Z"));
	rules.insert(std::pair<std::string, std::string>("X", "-W[+X]Z"));
	rules.insert(std::pair<std::string, std::string>("Y", "YZ"));
	rules.insert(std::pair<std::string, std::string>("Z", "[-FFF][+FFF]F"));
	dirLight = new DirectionalLight(glm::vec3(1.0, 1.0, 1.0));
	light = new PointLight(glm::vec3(0.0, 1.0, -6.0), glm::vec3(1.0, 1.0, 1.0), 10.0f, 1.0f);

	std::thread terrainThread(MakeTerrain, TERRAIN_SIZE, 100.0f);
	std::thread waterThread(MakeWater, TERRAIN_SIZE, 30.0f);
	
	terrainThread.join();
	waterThread.join();

	MeshData treeData = GenerateTree(glm::vec3(0.0f, 0.0f, 0.0f), 10, 1);

	Mesh tree(treeData.verts, treeData.inds);
	tree.isTree = true;

	Mesh terrain(terrainData.verts, terrainData.inds);
	Mesh water(waterData.verts, waterData.inds, "Resources/Shaders/WaterVertexShader.vs", "Resources/Shaders/WaterFragmentShader.fs");
	std::vector<glm::vec4> cloudTexture = GenerateClouds(129, 1.0f);

	water.meshMaterial->diffuseColour = glm::vec3(0, 0, 1);//7/255, 141/255, 237/255
	water.meshMaterial->specularColour = glm::vec3(1, 1, 1);
	water.meshMaterial->shininess = 256;

	//Rect* treeObj = new Rect(glm::vec3(0,0,0), "Tree");
	//treeObj->model->AddNewMesh(tree);
	for (int i = 0; i < terrainData.verts.size(); i += 900)
	{
 	
		if (terrainData.verts[i].position.y > 30)
		{
			glm::vec3 pos(0.0f);
			pos.x = terrainData.verts[i].position.x + GenerateRandom(-30, 30);
			pos.y = terrainData.verts[i].position.y;
			pos.z = terrainData.verts[i].position.z + GenerateRandom(-30, 30);
			Rect* treeObj = new Rect(pos, "Tree");
			treeObj->model->AddNewMesh(tree);
			
			
			if (terrainData.verts[i].position.y > 40)
			{
				treeObj->model->meshes[0].meshMaterial->diffuseColour = glm::vec3(1.0f, 1.0f, 1.0f);
			}
			else
			{
				treeObj->model->meshes[0].meshMaterial->diffuseColour = glm::vec3(0.0f, 1.0f, 0.0f);
			}
		}
	}
	
	


	Rect* someRect = new Rect(glm::vec3(0.0, 0, 0), "Terrain");
	Rect* waterObject = new Rect(glm::vec3(0, -20, 0), "Water");
	Rect* cloud = new Rect(glm::vec3(132.5, 130, 132.5), "Cloud");

	light->ambientColour = glm::vec3(0.5, 0.05, 0.05);



	someRect->model->AddNewMesh(terrain);
	someRect->model->SetTexture("Resources/Textures/Grass.png", Texture::TheOtherType::Grass, true);//WhiteLines.png
	someRect->model->SetTexture("Resources/Textures/Sand.jpg", Texture::TheOtherType::Sand, true);
	someRect->model->SetTexture("Resources/Textures/Snow.jpg", Texture::TheOtherType::Snow, true);
	someRect->model->SetTexture("Resources/Textures/Rock.jpg", Texture::TheOtherType::Rock, true);
	
	waterObject->model->AddNewMesh(water);
	
	cloud->AddRect(1000, 1000);
	cloud->model->SetTexture(cloudTexture, Texture::TheOtherType::Cloud, true);
}

void GameEngine::Update(float deltaTime) { //DeltaTime in seconds


}

int main(int argc, char* argv[]) {

	GameEngine::initEngine(argc, argv, "Procedural Generation", false, 800, 600);
	system("PAUSE");
	return 0;
}