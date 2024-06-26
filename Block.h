#pragma once
#include "DirectX.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Vertex.h"
#include "MatrixCalculation.h"
#include "Object.h"
#include "Engine.h"

class  BlockData {
public:
	std::unique_ptr<Object> model = nullptr;
	Vector4 material = { 1.0f,1.0f,1.0f,1.0f };
	std::string objectName;
	Transform transform;
	float findZ;
	float eraseZ;
	bool isActive = false;

	BlockData(std::string on, Transform tr, float fZ, float eZ) :
		objectName(on), transform(tr), findZ(fZ), eraseZ(eZ)
	{}
};

class Block
{
public:
	//シングルトン用
	static Block* GetInstance();
	//初期化処理
	void Initialize();
	//更新処理
	void Update();
	//描画処理
	void Draw(Camera* camera, DirectionalLight directionalLight);
	//解放処理
	void Finalize();

public:
	//ステージ情報の読み込み
	void LoadStage();

	void FindObject(BlockData* od);
	void FindALLObject();
	void EraseObject(BlockData* od);

private:
	// 外部のポインタ
	DirectXCommon* dxCommon_ = nullptr;
	MyEngine* engine_ = nullptr;

	static Block* instance;
	Block() = default;
	~Block() = default;
	
	std::stringstream popCommand_;
	std::list<BlockData*> mapData_;

};

