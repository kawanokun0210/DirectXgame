#include "LevelEditor.h"
#include <numbers>

void LevelEditor::LoadJsonFile() {
	//連結してフルパスを得る
	const std::string fullPath = "Resource/untitled.json";

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullPath);
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	//Json文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	levelData.reset(new LevelData());

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		RecursiveFunction(object);
	}

}

void LevelEditor::Draw(Camera* camera) {
	//レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		//ファイル名から登録済みモデルを検索
		Object* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.filename);
		if (it != models.end()) { model = it->second.get(); }
		//モデルを指定して3Dオブジェクトを生成
		

	}
}

void LevelEditor::RecursiveFunction(nlohmann::json& object) {
	assert(object.contains("type"));

	//種別を取得
	std::string type = object["type"].get<std::string>();

	//種類ごとの処理
	//MESH
	if (type.compare("MESH") == 0) {
		//要素追加
		levelData->objects.emplace_back(LevelData::ObjectData{});
		//今追加した要素の参照を得る
		LevelData::ObjectData& objectData = levelData->objects.back();

		if (object.contains("file_name")) {
			const std::string path = "Resource/";
			//ファイル名
			objectData.filename = object["file_name"];
			objectData.filename = path + objectData.filename;
		}

		//トランスフォームのパラメータ読み込み
		nlohmann::json& transform = object["transform"];
		//平行移動
		objectData.translate.x = (float)transform["translation"][0];
		objectData.translate.y = (float)transform["translation"][2];
		objectData.translate.z = (float)transform["translation"][1];

		//回転角
		objectData.rotate.x = -(float)transform["rotation"][0] * (float)std::numbers::pi / 180.0f;
		objectData.rotate.y = -(float)transform["rotation"][2] * (float)std::numbers::pi / 180.0f;
		objectData.rotate.z = -(float)transform["rotation"][1] * (float)std::numbers::pi / 180.0f;

		//スケーリング
		objectData.scale.x = (float)transform["scaling"][0];
		objectData.scale.y = (float)transform["scaling"][2];
		objectData.scale.z = (float)transform["scaling"][1];

		//コライダーのパラメータ読み込み
		if (object.contains("collider")) {
			nlohmann::json& collider = object["collider"];

			if (collider.contains("type")) {
				//コライダー情報があったら取得
				std::string type = collider["type"].get<std::string>();
				objectData.collisionType = type;

				objectData.center.x = (float)collider["center"][0];
				objectData.center.y = (float)collider["center"][2];
				objectData.center.z = (float)collider["center"][1];

				objectData.size.x = (float)collider["size"][0];
				objectData.size.y = (float)collider["size"][2];
				objectData.size.z = (float)collider["size"][1];
			}
		}
	}

	//オブジェクト走査を再帰関数にまとめ、再起呼出で枝を走査する
	if (object.contains("children")) {
		for (nlohmann::json& child : object["children"]) {
			RecursiveFunction(child);
		}
	}
}