#define _CRT_SECURE_NO_WARNINGS


#include "Game.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/vector_angle.hpp"
#include "Player.hpp"
#include "Shader.hpp"
#include "Plane.hpp"
#include "Skinning.hpp"
#include "SpriteComponent.hpp"
#include "MeshComponent.hpp"
#include "SkinMeshComponent.hpp"
#include "Roof.hpp"
#include "TextBox.hpp"
#include "TextComponent.hpp"
#include "TalkTextComponent.hpp"
#include "Text.hpp"
#include "TalkText.hpp"
#include "Counter.hpp"
#include "Clerk.hpp"
#include <fstream>
#include <codecvt>
#define STB_IMAGE_IMPLEMENTATION

Game::Game()
	:mWindowWidth(1024),
	mWindowHeight(768),
	mIsRunning(true),
	
	mMoveSensitivity(100.0f)
{
	mCameraUP = glm::vec3(0.0f, 0.0f, 1.0f);
	mCameraOrientation = glm::vec3(0.5f, 0, 0);


}


bool Game::Initialize()
{

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		printf("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Set OpenGL attributes
	// Use the core OpenGL profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Specify version 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);


	mWindow = SDL_CreateWindow("Treasure Search Game", 100, 100,
		mWindowWidth, mWindowHeight, SDL_WINDOW_OPENGL);
	if (!mWindow)
	{
		printf("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create an OpenGL context
	mContext = SDL_GL_CreateContext(mWindow);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("Failed to initialize GLEW.");
		return false;
	}

	auto error_code = glGetError();


	if (!LoadData())
	{
		printf("error: Failed to Load Game Data\n");
		return false;
	}



	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::SetLighting()
{
	std::vector<Shader*> Shaders;
	Shaders.push_back(mMeshShader);
	Shaders.push_back(mSkinningShader);

	for (auto shader : Shaders) {
		shader->UseProgram();
		//shader->SetVectorUniform("gEyeWorldPos", mCameraPos);
		shader->SetSamplerUniform("gShadowMap", 1);
		shader->SetSamplerUniform("gSampler", 0);
		shader->SetSamplerUniform("gNumSpotLights", mSpotLights.size());

		// spot light
		for (int i = 0; i < mSpotLights.size(); i++) {
			SpotLight spotLight = mSpotLights[i];
			std::string uniformName;
			std::string pD = std::to_string(i);
			uniformName = "gSpotLights[" + pD + "].Base.Base.Color";
			shader->SetVectorUniform(uniformName, spotLight.pointLight.Base.Color);
			uniformName = "gSpotLights[" + pD + "].Base.Base.AmbientIntensity";
			shader->SetFloatUniform(uniformName, spotLight.pointLight.Base.AmbientIntensity);
			uniformName = "gSpotLights[" + pD + "].Base.Position";
			shader->SetVectorUniform(uniformName, spotLight.pointLight.Position);
			uniformName = "gSpotLights[" + pD + "].Direction";
			shader->SetVectorUniform(uniformName, spotLight.Direction);
			uniformName = "gSpotLights[" + pD + "].Cutoff";
			shader->SetFloatUniform(uniformName, spotLight.Cutoff);
			uniformName = "gSpotLights[" + pD + "].Base.Base.DiffuseIntensity";
			shader->SetFloatUniform(uniformName, spotLight.pointLight.Base.DiffuseIntensity);
			uniformName = "gSpotLights[" + pD + "].Base.Atten.Constant";
			shader->SetFloatUniform(uniformName, spotLight.pointLight.Atten.Constant);
			uniformName = "gSpotLights[" + pD + "].Base.Atten.Linear";
			shader->SetFloatUniform(uniformName, spotLight.pointLight.Atten.Linear);
			uniformName = "gSpotLights[" + pD + "].Base.Atten.Exp";
			shader->SetFloatUniform(uniformName, spotLight.pointLight.Atten.Exp);
		}

		 // directional light
		{
			std::string uniformName;
			uniformName = "gDirectionalLight.Base.Color";
			shader->SetVectorUniform(uniformName, glm::vec3(1.0f, 1.0f, 1.0f));
			uniformName = "gDirectionalLight.Base.AmbientIntensity";
			shader->SetFloatUniform(uniformName, 0.1f);
			uniformName = "gDirectionalLight.Base.DiffuseIntensity";
			shader->SetFloatUniform(uniformName, 0.9f);
			uniformName = "gDirectionalLight.Direction";
			shader->SetVectorUniform(uniformName, glm::vec3(0.0f, 0.0f, -1.0f));
		}
	}
}

void Game::SetViewMatrix(glm::mat4 view, glm::vec3 cameraPos)
{
	std::vector<Shader*> shaders;
	shaders.push_back(mMeshShader);
	shaders.push_back(mSkinningShader);
	for (auto shader : shaders) {
		shader->UseProgram();
		shader->SetVectorUniform("gEyeWorldPos", cameraPos);
		shader->SetMatrixUniform("CameraView", view);
	}

}

bool Game::LoadData()
{
	// Set Light
	{
		BaseLight base;
		base.Color = glm::vec3(1.0f);
		base.AmbientIntensity = 0.1f;
		base.DiffuseIntensity = 0.05f;
		Attenuation atten;
		atten.Constant = 0;
		atten.Linear = 0.01f;
		atten.Exp = 0.f;
		PointLight pointlight;
		pointlight.Base = base;
		pointlight.Position = glm::vec3(5.0f, 5.0f, 10.0f);
		pointlight.Atten = atten;
		SpotLight spotLight;
		spotLight.pointLight = pointlight;
		spotLight.Direction = glm::vec3(0.f, 0.0f, -1.0f);
		spotLight.Up = glm::vec3(0.0f, 0.0f, 1.0f);
		spotLight.Cutoff = cosf(60.0f * M_PI / 180.0f);
		//spotLight.Position = glm::vec3(-1.0f, 2.5f, 10.0f);
		//spotLight.Direction = glm::vec3(0.5f, 0.0f, -1.0f);
		//spotLight.Up = glm::vec3(0.0f, 0.0f, 1.0f);
		mSpotLights.push_back(spotLight);
		spotLight.pointLight.Position = glm::vec3(25.0f, 5.0f, 10.0f);
		mSpotLights.push_back(spotLight);
		spotLight.pointLight.Position = glm::vec3(5.0f, 25.0f, 10.0f);
		mSpotLights.push_back(spotLight);
		spotLight.pointLight.Position = glm::vec3(25.0f, 25.0f, 10.0f);
		mSpotLights.push_back(spotLight);

	}


	// Set Camera
	//glm::mat4 CameraView = glm::lookAt(
	//	mCameraPos,
	//	mCameraPos + mCameraOrientation,
	//	mCameraUP);
	glm::mat4 CameraProj = glm::perspective(glm::radians(45.0f), (float)mWindowWidth / mWindowHeight, 0.1f, 100.0f);
	glm::mat4 SpotLightView = glm::lookAt(
		mSpotLights[0].pointLight.Position,
		mSpotLights[0].Direction,
		mSpotLights[0].Up
	);


	// Shader読み込み処理
	// 普通のMesh
	{
		std::string vert_file = "./Shaders/Mesh.vert";
		std::string frag_file = "./Shaders/Lighting.frag";
		mMeshShader = new Shader();
		if (!mMeshShader->CreateShaderProgram(vert_file, frag_file)) {
			return false;
		}
	}
	mMeshShader->UseProgram();
	mMeshShader->SetMatrixUniform("LightView", SpotLightView);
	mMeshShader->SetMatrixUniform("CameraProj", CameraProj);

	// Skinning Shader
	{
		std::string vert_file = "./Shaders/Skinning.vert";
		std::string frag_file = "./Shaders/Lighting.frag";
		mSkinningShader = new Shader();
		if (!mSkinningShader->CreateShaderProgram(vert_file, frag_file)) {
			return false;
		}
	}
	mSkinningShader->UseProgram();
	mSkinningShader->SetMatrixUniform("LightView", SpotLightView);
	mSkinningShader->SetMatrixUniform("CameraProj", CameraProj);



	// Sprite用のShader
	{
		std::string vert_file = "./Shaders/Sprite.vert";
		std::string frag_file = "./Shaders/Sprite.frag";
		mSpriteShader = new Shader();
		if (!mSpriteShader->CreateShaderProgram(vert_file, frag_file)) {
			return false;
		}
	}
	{
		// Set Sprite View Proj
		glm::mat4 spriteViewProj = glm::mat4(1.0f);
		spriteViewProj[0][0] = 2.0f / (float)mWindowWidth;
		spriteViewProj[1][1] = 2.0f / (float)mWindowHeight;
		spriteViewProj[3][2] = 1.0f;
		mSpriteShader->UseProgram();
		mSpriteShader->SetMatrixUniform("gSpriteViewProj", spriteViewProj);
	}

	// Font Rendering用のShader
	{
		std::string vert_file = "./Shaders/Text.vert";
		std::string frag_file = "./Shaders/Text.frag";
		mTextShader = new Shader();
		if (!mTextShader->CreateShaderProgram(vert_file, frag_file)) {
			return false;
		}
	}
	{
		// Set Sprite View Proj
		glm::mat4 spriteViewProj = glm::mat4(1.0f);
		spriteViewProj[0][0] = 2.0f / (float)mWindowWidth;
		spriteViewProj[1][1] = 2.0f / (float)mWindowHeight;
		spriteViewProj[3][2] = 1.0f;
		mTextShader->UseProgram();
		mTextShader->SetMatrixUniform("gSpriteViewProj", spriteViewProj);
	}


	// light setting
	SetLighting();


	// Modelを読み込む
	Plane* plane = nullptr;
	// 床を読み込む
	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 15; x++) {
			plane = new Plane(this);
			plane->SetPlaneType(Plane::CONCRETE);
			glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)M_PI / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			plane->SetPosition(glm::vec3(1.f + 2.f * x, 1.f + 2.f * y, 0.f));
			plane->SetRotation(rotate);
		}
	}

	// 壁を読み込む
	for (int x = 0; x < 15; x++) {
		for (float z = 1.f; z <= 3.f; z += 2.f) {
			float x_pos = 1.0f + 2.0f * x;
			float y_pos = 0.f;
			plane = new Plane(this);
			plane->SetPlaneType(Plane::BRICK);
			plane->SetPosition(glm::vec3(x_pos, y_pos, z));
			plane->SetRotation(glm::mat4(1.f));
			y_pos = 30.f;
			plane = new Plane(this);
			plane->SetPlaneType(Plane::BRICK);
			plane->SetPosition(glm::vec3(x_pos, y_pos, z));
			plane->SetRotation(glm::mat4(1.f));
			y_pos = 0.f;
			plane = new Plane(this);
			plane->SetPlaneType(Plane::BRICK);
			plane->SetPosition(glm::vec3(y_pos, x_pos, z));
			glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)M_PI / 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			plane->SetRotation(rotate);
			y_pos = 30.f;
			plane = new Plane(this);
			plane->SetPlaneType(Plane::BRICK);
			plane->SetPosition(glm::vec3(y_pos, x_pos, z));
			plane->SetRotation(rotate);
		}
	}

	// 屋根を読み込む
	Actor* a = new Roof(this);


	// Player
	mPlayer = new Player(this);

	// 迷路の箱読み込み
	MazeBox* mazebox = nullptr;
	for (int y = 5; y < 15; y++) {
		for (int x = 0; x < 15; x++) {
			mazebox = new MazeBox(this);
			mazebox->SetBoxType((y * 15 + x) % 2 ? MazeBox::BLACK : MazeBox::WHITE);
			float x_pos = 1.0f + 2.0f * x;
			float y_pos = 1.0f + 2.0f * y;
			mazebox->SetPosition(glm::vec3(x_pos, y_pos, 0.f));
		}
	}

	// 受付カウンター読み込み
	a = new Counter(this);

	// TextBoxの読み込み
	a = new TextBox(this);
	a->SetPosition(glm::vec3(0.0f, -mWindowHeight / 4.0f, 0.0f));
	a->SetRotation(glm::mat4(1.0f));
	a->SetScale(1.5f);

	// 店員
	a = new Clerk(this);




	//{
	//	// Treasure Box
	//	Mesh* mesh = new Mesh();
	//	if (mesh->Load("./resources/TreasureBox/", "scene.gltf")) {
	//		mesh->SetPos(glm::vec3(4.0f, 5.0f / 2.0f, 0.0f));
	//		mesh->SetRotate(glm::mat4(1.0f));
	//		mesh->SetScale(0.01f / 2.0f);
	//		mMeshes.push_back(MeshData(mesh, "TreasureChest"));
	//	}
	//}
	//{
	//	// Maze Box
	//	MazeBox* mazeBox = new MazeBox("./resources/MazeBox/", 
	//		"MazeBox.fbx", "BlackUV.png", "WhiteUV.png");
	//	mazeBox->SetPos(glm::vec3(4.0f, 5.0f, 0.0f));
	//	mazeBox->SetRotate(glm::mat4(1.0f));
	//	mazeBox->SetScale(1.0f);
	//	mazeBox->SetBoxType(MazeBox::BLACK);
	//	mMazeBox = mazeBox;
	//	mMazeData = new float* [10];
	//	for (int y = 0; y < 10; y++) {
	//		mMazeData[y] = new float[15];
	//		for (int x = 0; x < 15; x++) {
	//			mMazeData[y][x] = 0.f;
	//		}
	//	}
	//}


	// Load Text
	a = new UserAssistText(this);

	// Talk Text の読み込み
	a = new TalkText(this);

	//mText = new TextComponent();
	//mText->SetPos(glm::vec3(0.0f));
	//mText->SetRotate(glm::mat4(1.0f));
	//mText->SetScale(1.0f);
	//mText->SetAlpha(1.0f);
	//mText->SetTextColor(glm::vec3(0.0f, 0.0f, 0.2f));

	// Load Level Data
	mLevelData = new char* [10];
	for (int y = 0; y < 10; y++) {
		mLevelData[y] = new char[15];
	}
	{
		std::string fileName = "./resources/1.level";
		FILE* fp = fopen(fileName.c_str(), "rb");
		char c;

		//	EOFまでファイルから文字を1文字ずつ読み込む
		int x = 0;
		int y = 0;
		while ((c = fgetc(fp)) != EOF)
		{
			//	読み込んだ1文字を画面に出力する
			if (y == 10) {
				break;
			}
			if (c == '\n') {
				y++;
				x = 0;
				continue;
			}
			mLevelData[y][x++] = c;
		}

		fclose(fp);
	}


	return true;
}

bool Game::IsWall(glm::vec3 pos)
{
	if (mLevelData[static_cast<int>(pos.y / 2) - 5][static_cast<int>(pos.x / 2)] == '#') {
		// Playerのいる場所が壁なら更新しない
		return true;
	}
}

void Game::ProcessInput()
{
	SDL_Point mouse_position = { mWindowWidth / 2, mWindowHeight / 2 };
	SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
	mMousePos.x = mouse_position.x;
	mMousePos.y = mouse_position.y;
	//printf("%d %d\n", mMousePos.x, mMousePos.y);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:	// マウスのボタンが押されたら
		{
			if (mPhase == PHASE_IDLE) {
				//mLastMousePos = mMousePos;
				//mSwipingDropPos = mMousePos / GRID_SIZE;
				mPhase = PHASE_MOVE;

				SDL_WarpMouseInWindow(mWindow, mWindowWidth / 2, mWindowHeight / 2);
				mMousePos.x = mWindowWidth / 2;
				mMousePos.y = mWindowHeight / 2;
				SDL_ShowCursor(SDL_DISABLE);
				//std::cout << "----------------------------------------------called\n";
			}
		}
		break;
		case SDL_MOUSEBUTTONUP:		// マウスを離したら
			if (mPhase == PHASE_MOVE) {
				mPhase = PHASE_IDLE;

				/*if (EraseDrops()) {
					phase = PHASE_ERASE;
				}
				else {
					phase = PHASE_IDLE;
				}*/
				SDL_ShowCursor(SDL_ENABLE);
			}
			break;
		}
	}

	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE] || keyState[SDL_SCANCODE_Q])	// escapeキーを押下すると終了
	{
		mIsRunning = false;
	}

	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
}


void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	mTicksCount = SDL_GetTicks();

	if (mPhase == PHASE_MOVE) {
		//printf("%d %d\n", mMousePos.x, mMousePos.y);

		float rotX = mMoveSensitivity * (float)((float)mMousePos.y - ((float)mWindowHeight / 2.0f)) / (float)mWindowHeight;
		float rotY = mMoveSensitivity * (float)((float)mMousePos.x - ((float)mWindowWidth / 2.0f)) / (float)mWindowWidth;
		//printf("rotX: %f rotY: %f\t", rotX, rotY);
		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(mCameraOrientation, glm::radians(-rotX), glm::normalize(glm::cross(mCameraOrientation, mCameraUP)));

		// Decides whether or not the next vertical Orientation is legal or not
		int rad = abs(glm::angle(newOrientation, mCameraUP) - glm::radians(90.0f));
		//std::cout << rad * 180 / M_PI << std::endl;
		if (abs(glm::angle(newOrientation, mCameraUP) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			mCameraOrientation = newOrientation;
		}

		// Rotates the Orientation left and right
		mCameraOrientation = glm::rotate(mCameraOrientation, glm::radians(-rotY), mCameraUP);

		if ((mMousePos.x != mWindowWidth / 2) || (mMousePos.y != mWindowHeight / 2)) {
			SDL_WarpMouseInWindow(mWindow, mWindowWidth / 2, mWindowHeight / 2);
		}
	}

	for (auto actor : mActors) {
		actor->Update(deltaTime);
	}

	//mPlayer->Update(deltaTime);
}

Mesh* Game::GetMesh(std::string fileName, std::string ext)
{
	Mesh* m = nullptr;
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end())
	{
		m = iter->second;
	}
	else
	{
		m = new Mesh();
		if (m->Load(fileName, ext))
		{
			mMeshes.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}

SkinMesh* Game::GetSkinMesh(std::string filePath, std::string ext)
{
	SkinMesh* m = nullptr;
	auto iter = mSkinMeshes.find(filePath);
	if (iter != mSkinMeshes.end())
	{
		m = iter->second;
	}
	else
	{
		m = new SkinMesh();
		if (m->Load(filePath, ext))
		{
			mSkinMeshes.emplace(filePath, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;

}


void Game::RemoveActor(Actor* actor)
{
	// Is it in actors?
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::RemoveSpriteComp(SpriteComponent* sprite)
{
	auto iter = std::find(mSpriteComps.begin(), mSpriteComps.end(), sprite);
	mSpriteComps.erase(iter);
}

void Game::AddMeshComp(MeshComponent* mesh)
{
	if (mesh->GetIsSkeltal()) {
		SkinMeshComponent* skin = static_cast<SkinMeshComponent*>(mesh);
		mSkinMeshComps.push_back(skin);
	}
	else {
		mMeshComps.push_back(mesh);
	}
}

void Game::AddSpriteComp(SpriteComponent* sprite)
{
	switch (sprite->GetType()) {
	case SpriteComponent::SPRITE:
		mSpriteComps.push_back(sprite);
		break;
	case SpriteComponent::TEXT:
	{
		TextComponent* text = static_cast<TextComponent*>(sprite);
		mTextComps.push_back(text);
		break;
	}	
	case SpriteComponent::TALK_TEXT:
	{
		TalkTextComponent* text = static_cast<TalkTextComponent*>(sprite);
		mTalkTextComps.push_back(text);
		break;
	}
	}
}

void Game::RemoveMeshComp(MeshComponent* mesh)
{
	auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
	mMeshComps.erase(iter);
}

void Game::RemoveMeshComp(SkinMeshComponent* mesh)
{
	auto iter = std::find(mSkinMeshComps.begin(), mSkinMeshComps.end(), mesh);
	mSkinMeshComps.erase(iter);
}

void Game::RemoveTextComp(class TextComponent* text)
{
	auto iter = std::find(mTextComps.begin(), mTextComps.end(), text);
	mTextComps.erase(iter);
}

void Game::Draw()
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(0, 0.5, 0.7, 1.0f);
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	mMeshShader->UseProgram();
	for (auto mc : mMeshComps) {
		mc->Draw(mMeshShader);
	}


	//for (auto mesh : mMeshes) {
	//	if (mesh.meshName == "TreasureChest") {
	//		mesh.mesh->SetPos(glm::vec3(4.0f, 5.0f / 2.0f, 0.0f));
	//		mesh.mesh->Draw(mMeshShader, mTicksCount / 1000.0f);
	//		mesh.mesh->SetPos(glm::vec3(4.0f, 4.0f, 0.0f));
	//		mesh.mesh->Draw(mMeshShader, mTicksCount / 1000.0f);
	//	}
	//}

	// 床描画
	//mPlane->SetPlaneType(Plane::CONCRETE);
	//for (int y = 0; y < 5; y++) {
	//	for (int x = 0; x < 15; x++) {
	//		float x_pos = 1.0f + 2.0f * x;
	//		float y_pos = 1.0f + 2.0f * y;
	//		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)M_PI / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//		mPlane->SetRotate(rotate);
	//		mPlane->SetPos(glm::vec3(x_pos, y_pos, 0.0f));
	//		mPlane->Draw(mMeshShader, mTicksCount / 1000.0f);
	//	}
	//}

	//// 壁描画
	//mPlane->SetPlaneType(Plane::BRICK);
	//for (int x = 0; x < 15; x++) {
	//	for (float z = 1.f; z <= 3.f; z += 2.f) {
	//		float x_pos = 1.0f + 2.0f * x;
	//		float y_pos = 0.f;
	//		mPlane->SetPos(glm::vec3(x_pos, y_pos, z));
	//		mPlane->SetRotate(glm::mat4(1.f));
	//		mPlane->Draw(mMeshShader, mTicksCount / 1000.0f);
	//		y_pos = 30.f;
	//		mPlane->SetPos(glm::vec3(x_pos, y_pos, z));
	//		mPlane->SetRotate(glm::mat4(1.f));
	//		mPlane->Draw(mMeshShader, mTicksCount / 1000.0f);
	//		y_pos = 0.f;
	//		mPlane->SetPos(glm::vec3(y_pos, x_pos, z));
	//		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)M_PI / 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	//		mPlane->SetRotate(rotate);
	//		mPlane->Draw(mMeshShader, mTicksCount / 1000.0f);
	//		y_pos = 30.f;
	//		mPlane->SetPos(glm::vec3(y_pos, x_pos, z));
	//		mPlane->SetRotate(rotate);
	//		mPlane->Draw(mMeshShader, mTicksCount / 1000.0f);
	//	}
	//}

	//// 屋根描画
	//mRoof->SetPos(glm::vec3(15.f, 15.f, 4.f));
	//mRoof->SetRotate(glm::mat4(1.f));
	//mRoof->SetScale(15.f);
	//mRoof->Draw(mMeshShader, mTicksCount / 1000.f);



	//// Mazeの床描画
	//mMazeBox->SetScale(1.0f);
	//for (int y = 5; y < 15; y++) {
	//	for (int x = 0; x < 15; x++) {
	//		mMazeBox->SetBoxType((y * 15 + x) % 2 ? MazeBox::BLACK : MazeBox::WHITE);
	//		float x_pos = 1.0f + 2.0f * x;
	//		float y_pos = 1.0f + 2.0f * y;

	//		if (mLevelData[y - 5][x] == '-') {
	//			// 通路
	//			mMazeBox->SetPos(glm::vec3(x_pos, y_pos, 0.f));
	//		}
	//		else {
	//			// 通れない
	//			glm::vec3 playerPos = mPlayer->GetPos();
	//			if (
	//				(x_pos - 1.5 <= playerPos.x) && (playerPos.x <= x_pos + 1.5) &&
	//				(y_pos - 1.5 <= playerPos.y) && (playerPos.y <= y_pos + 1.5)
	//				) {
	//				if (mMazeData[y - 5][x] <= 2.0f) {
	//					mMazeData[y - 5][x] += 0.01;
	//				}
	//			}
	//			else {
	//				if (mMazeData[y - 5][x] >= 0.f) {
	//					mMazeData[y - 5][x] -= 0.01;
	//				}
	//			}
	//			mMazeBox->SetPos(glm::vec3(x_pos, y_pos, mMazeData[y - 5][x]));
	//		}

	//		mMazeBox->Draw(mMeshShader, mTicksCount / 1000.0f);
	//	}
	//}


	// Draw Skin Mesh
	mSkinningShader->UseProgram();
	for (auto skinmesh : mSkinMeshComps) {
		skinmesh->Draw(mSkinningShader);
	}

	//mPlayer->Draw(mSkinningShader, mTicksCount / 1000.0f);

	// Draw Sprites
	glDisable(GL_DEPTH_TEST);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	mSpriteShader->UseProgram();
	for (auto sprite : mSpriteComps) {
		sprite->Draw(mSpriteShader);
	}

	// Draw Text
	mTextShader->UseProgram();
	for (auto text : mTextComps) {
		text->Draw(mTextShader);
	}
	for (auto talktext : mTalkTextComps) {
		talktext->Draw(mTextShader);
	}

	// 文字描画
	// エンターキーではなす
	//if (mPhase != PHASE_TALK) {
	//	if (
	//		(2.0f < mPlayer->GetPos().x) && (mPlayer->GetPos().x < 3.0f) &&
	//		(1.0f < mPlayer->GetPos().y) && (mPlayer->GetPos().y < 2.0f)
	//		) {
	//		mText->SetPos(glm::vec3(0.0f, 300.f, 0.f));
	//		mText->SetText(u"エンターキーで話す");
	//		mText->SetTextColor(glm::vec3(0.f, 0.f, 0.2f));
	//		mText->Draw(mTextShader);
	//	}
	//}

	//// 話し中ならトーク画面描画
	//if (mPhase == PHASE_TALK) {
	//}
	//mTextBox->Draw(mSpriteShader);







	SDL_GL_SwapWindow(mWindow);
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		Draw();
	}
}

void Game::Shutdown()
{

}
