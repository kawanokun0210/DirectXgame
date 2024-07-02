#include "Block.h"
#include <cmath>

Block* Block::instance = nullptr;

Block* Block::GetInstance() {
	if (instance == nullptr) {
		instance = new Block;
	}
	return instance;
}

void Block::Finalize() {
	for (BlockData* od : mapData_) {
		delete od;
	}
	delete instance;
	instance = nullptr;
}

void Block::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	engine_ = MyEngine::GetInstance();
}

void Block::Update() {

	int id = 0;

	int i = 0;
	for(BlockData * od : mapData_) {
		char label[32];
		std::string str = "Tr" + std::to_string(i);
		std::copy(str.begin(), str.end(), label);
		label[str.size()] = '\0'; //必要に応じてnull終端する

		ImGui::DragFloat3(label, &od->transform.translate.x);

		i++;
	}

}

void Block::Draw(Camera* camera, DirectionalLight directionalLight) {
	for (BlockData* od : mapData_) {
		if (od->isActive == true) {
			od->model->SetTransform(od->transform);
			od->model->Draw(2, camera, directionalLight, true);
		}
	}
}

void Block::LoadStage()
{
	//ファイルの基本名と拡張子を指定
	std::string baseName = "./Resource/map";
	std::string extension = ".csv";

	std::stringstream ss;
	ss << baseName << extension;

	//ファイルを開く
	std::ifstream file;
	file.open(ss.str());
	assert(file.is_open());

	std::stringstream fileData;
	fileData << file.rdbuf();

	//ファイルの内容を文字列ストリームコピー
	popCommand_ = std::move(fileData);

	//ファイルを閉じる
	file.close();

	//一行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(popCommand_, line)) {
		//一行分の文字列をストリームに変換
		std::stringstream ss(line);
		std::string word;

		//一行取得
		while (getline(ss, word, ',')) {

			//数値やファイル名の前に記載されたコマンドに応じて値を代入
			std::string on;
			Transform tr = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
			float fZ = 0.0f;
			float eZ = 0.0f;

			//オブジェクト名を取得し、ファイルパスを指定する
			if (word.find("On") == 0) {
				// オブジェクト名の取得
				getline(ss, word, ',');
				on = word.c_str();
				getline(ss, word, ',');
			}

			//座標を指定する
			if (word.find("Tr") == 0) {
				//オブジェクト名の取得
				getline(ss, word, ',');
				tr.translate.x = (float)std::atof(word.c_str());
				getline(ss, word, ',');
				tr.translate.y = (float)std::atof(word.c_str());
				getline(ss, word, ',');
				tr.translate.z = (float)std::atof(word.c_str());

				getline(ss, word, ',');
			}

			//出現位置と消滅位置(Z軸)を指定する？
			if (word.find("fe") == 0) {
				//オブジェクト名の取得
				getline(ss, word, ',');
				fZ = (float)std::atof(word.c_str());
				getline(ss, word, ',');
				eZ = (float)std::atof(word.c_str());

			}

			//最後に数値を代入する
			BlockData* newBlock = new BlockData(on, tr, fZ, eZ);
			mapData_.push_back(newBlock);

		}

	}

	for (BlockData* od : mapData_) {
		od->model = std::make_unique<Object>();
	}

}

void Block::FindObject(BlockData* od)
{
	//stringstreamにまとめる
	std::stringstream ss;
	ss << od->objectName << ".obj";
	if (dxCommon_ == nullptr) {
		dxCommon_ = DirectXCommon::GetInstance();
	}
	od->model->Initialize("Resource/map/", ss.str());
	od->isActive = true;
}

void Block::FindALLObject()
{
	for (BlockData* od : mapData_)
	{
		//stringstreamにまとめる
		std::stringstream ss;
		ss << od->objectName << ".obj";

		dxCommon_;
		engine_;

		od->model->Initialize("Resource/map/", ss.str());
		od->isActive = true;
	}
}

void Block::EraseObject(BlockData* od)
{
	od->isActive = false;
	delete od;
}