#include "GamePlayScene.h"

void GamePlayScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon)
{
	engine_ = engine;
	dxCommon_ = dxCommon;

	sound_ = new Sound();
	sound_->Initialize();

	input_ = Input::GetInstance();
	input_->Initialize();

	soundDataHandle_ = sound_->LoadWave("Resource/Audio/Alarm01.wav");

	triangleData_[0].position[0] = { -0.5f,-0.5f,0.0f,1.0f };
	triangleData_[0].position[1] = { 0.0f,0.5f,0.0f,1.0f };
	triangleData_[0].position[2] = { 0.5f,-0.5f,0.0f,1.0f };
	triangleData_[0].material = { 1.0f,1.0f,1.0f,1.0f };

	triangleData_[1].position[0] = { -0.5f,-0.5f,0.5f,1.0f };
	triangleData_[1].position[1] = { 0.0f,0.0f,0.0f,1.0f };
	triangleData_[1].position[2] = { 0.5f,-0.5f,-0.5f,1.0f };
	triangleData_[1].material = { 1.0f,1.0f,1.0f,1.0f };

	for (int i = 0; i < 2; i++)
	{
		transform_[i] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	}

	triangleDrawA_ = false;
	triangleDrawB_ = false;

	spriteData_.LeftTop[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.RightDown[0] = { 640.0f,360.0f,0.0f,1.0f };
	spriteData_.LeftTop[1] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.RightDown[1] = { 640.0f,360.0f,0.0f,1.0f };
	spriteData_.material = { 1.0f,1.0f,1.0f,1.0f };
	spriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	spriteDraw_ = false;

	sphereTransform_ = { {0.4f,0.4f,0.4f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	sphereMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	sphereDraw_ = false;

	objectDraw_ = false;

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	engine_->SettingTexture("Resource/uvChecker.png", 2);

	monsterBallResourceNum_ = 1;
	engine_->SettingTexture("Resource/fence/fence.png", 3);

	engine_->SettingTexture("Resource/circle.png", 4);

	engine_->SettingTexture("Resource/particle.png", 5);

	for (int i = 0; i < 2; i++)
	{
		triangle_[i] = new Triangle();
		triangle_[i]->Initialize(dxCommon_, engine_);
	}

	for (int i = 0; i < 2; i++)
	{
		sprite_[i] = new Sprite();
		sprite_[i]->Initialize(dxCommon_, engine_);
	}

	sphere_ = new Sphere();
	sphere_->Initialize(dxCommon_, engine_);

	object_[0] = new Object();

	object_[0]->Initialize(dxCommon_, engine_, "Resource/fence", "fence.obj");

	object_[1] = new Object();

	object_[1]->Initialize(dxCommon_, engine_, "Resource/", "plane.obj");

	for (int i = 0; i < 2; i++) {
		objectTransform_[i] = { {0.4f,0.4f,0.4f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
		objectMaterial_[i] = { 1.0f,1.0f,1.0f,1.0f };
	}

	objectTransform_[1] = { {0.4f,0.4f,0.4f},{0.0f,0.0f,0.0f},{1.0f,-1.0f,0.0f} };

	particle = new Particle();

	particle->Initialize(dxCommon_, engine_, "Resource/", "plane.obj");

	std::mt19937 randomEngine(seedGenerator());
	
	for (uint32_t index = 0; index < 10; ++index) {
		particles[index] = particle->MakeNewParticle(randomEngine);
	}

	camera_ = new Camera();
	camera_->Initialize(dxCommon_);
	//cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };

}

void GamePlayScene::Update()
{
	//XINPUT_STATE joyState;
	input_->Update();

	for (int i = 0; i < 2; i++)
	{
		/*if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
			return;
		}*/
		//if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		if (input_->PushKey(DIK_R)) {
			transform_[i].rotate.y += 0.01f;
		}
		//}
		worldMatrix_ = MakeAffineMatrix(transform_[i].scale, transform_[i].rotate, transform_[i].translate);
	}

	if (input_->PushKey(DIK_SPACE)) {
		sound_->PlayWave(soundDataHandle_, true, 1.0f);
	}

	if (input_->PushKey(DIK_RETURN)) {
		sphereTransform_.rotate.y += 0.01f;
	}
	sphereMatrix_ = MakeAffineMatrix(sphereTransform_.scale, sphereTransform_.rotate, sphereTransform_.translate);

	Matrix4x4 sphereAffine = MakeAffineMatrix(sphereTransform_.scale, sphereTransform_.rotate, sphereTransform_.translate);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(dxCommon_->GetWin()->kClientWidth) / float(dxCommon_->GetWin()->kClientHeight), 0.1f, 100.0f);
	
	directionalLight_.direction = Normalise(directionalLight_.direction);

	ImGui::Begin("OPTION");
	if (ImGui::TreeNode("Triangle"))
	{
		ImGui::Text("Rotate Push [R]");
		if (ImGui::Button("TriangleA"))
		{
			if (triangleDrawA_ == false)
			{
				triangleDrawA_ = true;
			}
			else {
				triangleDrawA_ = false;
			}
		}

		if (ImGui::Button("TriangleB"))
		{
			if (triangleDrawB_ == false)
			{
				triangleDrawB_ = true;
			}
			else {
				triangleDrawB_ = false;
			}
		}

		if (triangleDrawA_ == true)
		{
			if (ImGui::TreeNode("Triangle1"))
			{
				ImGui::DragFloat3("Translate", &transform_[0].translate.x, 0.05f);
				ImGui::DragFloat3("Rotate", &transform_[0].rotate.x, 0.05f);
				ImGui::DragFloat3("Scale", &transform_[0].scale.x, 0.05f);
				ImGui::ColorEdit4("Color", &triangleData_[0].material.x, 0);
				ImGui::TreePop();
			}
		}
		if (triangleDrawB_ == true)
		{
			if (ImGui::TreeNode("Triangle2"))
			{
				ImGui::DragFloat3("Translate2", &transform_[1].translate.x, 0.05f);
				ImGui::DragFloat3("Rotate2", &transform_[1].rotate.x, 0.05f);
				ImGui::DragFloat3("Scale2", &transform_[1].scale.x, 0.05f);
				ImGui::ColorEdit4("Color", &triangleData_[1].material.x, 0);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Sphere"))
	{
		ImGui::Text("Rotate Push [ENTER]");
		if (ImGui::Button("Sphere"))
		{
			if (sphereDraw_ == false)
			{
				sphereDraw_ = true;
			}
			else {
				sphereDraw_ = false;
			}
		}

		ImGui::DragFloat3("Translate", &sphereTransform_.translate.x, 0.05f);
		ImGui::DragFloat3("Rotate", &sphereTransform_.rotate.x, 0.05f);
		ImGui::DragFloat3("Scale", &sphereTransform_.scale.x, 0.05f);
		ImGui::ColorEdit4("Color", &sphereMaterial_.x, 0);
		ImGui::Checkbox("ChangeTexture", &texture_);
		ImGui::DragFloat4("LightColor", &directionalLight_.color.x, 1.0f);
		ImGui::DragFloat3("DirectionLight", &directionalLight_.direction.x, 0.1f);
		ImGui::DragFloat2("UVTranslate", &sphere_->uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat2("UVScale", &sphere_->uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
		ImGui::SliderAngle("UVRotate", &sphere_->uvTransformSprite.rotate.z);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Particle"))
	{
		for (int i = 0; i < 10; i++) {
			ImGui::DragFloat3("Translate", &particles[i].transform.translate.x, 0.05f);
			ImGui::DragFloat3("Rotate", &particles[i].transform.rotate.x, 0.05f);
			ImGui::DragFloat3("Scale", &particles[i].transform.scale.x, 0.05f);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Object"))
	{
		if (ImGui::Button("Object"))
		{
			if (objectDraw_ == false)
			{
				objectDraw_ = true;
			}
			else {
				objectDraw_ = false;
			}
		}

		ImGui::DragFloat3("Translate", &objectTransform_[0].translate.x, 0.05f);
		ImGui::DragFloat3("Rotate", &objectTransform_[0].rotate.x, 0.05f);
		ImGui::DragFloat3("Scale", &objectTransform_[0].scale.x, 0.05f);
		ImGui::ColorEdit4("Color", &objectMaterial_[1].x, 0);
		//ImGui::Checkbox("ChangeTexture", &texture_);
		ImGui::DragFloat4("LightColor", &directionalLight_.color.x, 1.0f);
		ImGui::DragFloat3("DirectionLight", &directionalLight_.direction.x, 0.1f);
		/*ImGui::DragFloat2("UVTranslate", &sphere_->uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat2("UVScale", &sphere_->uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
		ImGui::SliderAngle("UVRotate", &sphere_->uvTransformSprite.rotate.z);*/

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sprite"))
	{
		if (ImGui::Button("Sprite"))
		{
			if (spriteDraw_ == false)
			{
				spriteDraw_ = true;
			}
			else {
				spriteDraw_ = false;
			}
		}

		ImGui::DragFloat3("Translate", &spriteTransform_.translate.x, 0.05f);
		ImGui::DragFloat3("Rotate", &spriteTransform_.rotate.x, 0.05f);
		ImGui::DragFloat3("Scale", &spriteTransform_.scale.x, 0.05f);
		ImGui::ColorEdit4("Color", &spriteData_.material.x, 0);
		for (int i = 0; i < 2; i++) {
			ImGui::DragFloat2("UVTranslate", &sprite_[i]->uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat2("UVScale", &sprite_[i]->uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
			ImGui::SliderAngle("UVRotate", &sprite_[i]->uvTransformSprite.rotate.z);
		}
		ImGui::TreePop();
	}

	/*if (ImGui::TreeNode("Camera"))
	{
		ImGui::DragFloat3("Translate", &camera_->GetTransform().translate.x, 0.05f);
		ImGui::DragFloat3("Rotate", &camera_->GetTransform().rotate.x, 0.05f);
		ImGui::DragFloat3("Scale", &camera_->GetTransform().scale.x, 0.05f);
		ImGui::TreePop();
	}*/
	ImGui::End();
}

void GamePlayScene::Draw()
{

	if (triangleDrawA_)
	{
		triangle_[0]->Draw(triangleData_[0].position[0], triangleData_[0].position[1], triangleData_[0].position[2], triangleData_[0].material, transform_[0], camera_, 2, directionalLight_);
	}

	if (triangleDrawB_)
	{
		triangle_[1]->Draw(triangleData_[1].position[0], triangleData_[1].position[1], triangleData_[1].position[2], triangleData_[1].material, transform_[1], camera_, 2, directionalLight_);
	}

	if (sphereDraw_)
	{
		sphere_->Draw(sphereMaterial_, sphereTransform_, 2, camera_, directionalLight_);
	}

	if (spriteDraw_)
	{
		for (int i = 0; i < 1; i++)
		{
			sprite_[i]->Draw(spriteData_.LeftTop[i], spriteData_.RightDown[i], spriteTransform_, spriteData_.material, 2, directionalLight_);
		}
	}
	if (objectDraw_) {
		for (int i = 0; i < 2; i++) {
			object_[i]->Draw(objectMaterial_[i], objectTransform_[i], 3, camera_, directionalLight_, true);
		}
	}

	particle->Draw(&particles[0], 4, camera_);

}

void GamePlayScene::Finalize()
{
	for (int i = 0; i < 2; i++)
	{
		//triangle_[i]->Finalize();
		delete triangle_[i];
	}

	for (int i = 0; i < 2; i++)
	{
		//sprite_[i]->Finalize();
		delete sprite_[i];
	}

	sphere_->Finalize();
	for (int i = 0; i < 2; i++) {
		object_[i]->Finalize();
	}
	delete object_[0];
	delete object_[1];

	sound_->Finalize();
	sound_->UnLoad(&soundDataHandle_);

	delete sphere_;
	delete sound_;

	delete camera_;

	delete particle;
	//delete input_;
}