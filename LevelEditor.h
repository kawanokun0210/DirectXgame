#pragma once
#include "DirectX.h"
#include <dxcapi.h>
#include "../String.h"
#include "MatrixCalculation.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "../Vertex.h"
#include "Triangle.h"
#include "d3dx12.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MatrixCalculation.h"
#include "Quaternion.h"
#include <array>
#include "../Resource/json.hpp"
#include "Object.h"
#include "Camera.h"

struct LevelData
{
	struct ObjectData
	{
		std::string filename;
		Vector3 translate;
		Vector3 rotate;
		Vector3 scale;

		// colliderの情報
		std::string collisionType;
		Vector3 center;
		Vector3 size;
	};
	std::vector<ObjectData>objects;
};

class LevelEditor
{
public:

	void LoadJsonFile();

	//void Update();

	void Draw(Camera* camera, DirectionalLight directionalLight_);

	void RecursiveFunction(nlohmann::json& object);

private:

	std::map<std::string, std::unique_ptr<Object>> models;
	std::vector<Object*> objects;
	std::unique_ptr<LevelData> levelData;

};

