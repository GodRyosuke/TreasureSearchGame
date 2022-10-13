#define _CRT_SECURE_NO_WARNINGS


#include "Game.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/vector_angle.hpp"
#include "Actor/Player.hpp"
#include "Shader.hpp"
#include "Actor/Plane.hpp"
#include "Actor/MazeBox.hpp"
#include "Skinning.hpp"
#include "Component/SpriteComponent.hpp"
#include "Component/MeshComponent.hpp"
#include "Component/SkinMeshComponent.hpp"
#include "Actor/Roof.hpp"
#include "Actor/TextBox.hpp"
#include "Component/TextComponent.hpp"
#include "Component/TalkTextComponent.hpp"
#include "Actor/Text.hpp"
#include "Actor/TalkText.hpp"
#include "Actor/Counter.hpp"
#include "Actor/Clerk.hpp"
#include "Actor/TreasureBox.hpp"
#include "Sound.hpp"
#include "Random.hpp"
#include <fstream>
#include <codecvt>
#include <random>

#define STB_IMAGE_IMPLEMENTATION

Game::Game()
	:mWindowWidth(1024),
	mWindowHeight(768),
	mIsRunning(true),
	mPhase(PHASE_NORMAL)
	, mLevelIdx(0)
{
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

	// Load Level Data
	for (int i = 1; i <= 5; i++) {
		char** levelData = new char* [10];
		for (int y = 0; y < 10; y++) {
			levelData[y] = new char[15];
		}
		std::string fileRoot = "./resources/LevelData/";
		std::string fileName = std::to_string(i) + ".level";
		std::string filePath = fileRoot + fileName;
		FILE* fp = fopen(filePath.c_str(), "rb");
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
			levelData[y][x++] = c;
		}

		fclose(fp);
		mLevelArray.push_back(levelData);
	}

	// 乱数
	mRandom = new Random();


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
	mClerk = new Clerk(this);

	// 宝箱
	mTreasureBox = new TreasureBox(this);

	// サウンド関連
	mSound = new Sound();
	mSound->StartMusic("event:/NormalBGM");	// 店内のBGMを流す


	// Load Text
	a = new UserAssistText(this);
	a = new TimerText(this);

	// Talk Text の読み込み
	mTalkText = new TalkText(this);


	return true;
}

bool Game::IsWall(glm::vec3 pos)
{
	char** levelData = mLevelArray[mLevelIdx];

	// Mazeの領域にあるか？
	if (
		(0.f < pos.x) && (pos.x < 30.f) &&
		(10.f < pos.y) && (pos.y < 30.f)
		) {
		if (levelData[static_cast<int>(pos.y / 2) - 5][static_cast<int>(pos.x / 2)] == '#') {
			// Playerのいる場所が壁なら更新しない
			return true;
		}
		else {
			return false;
		}
	}
	else {
		// Maze外側は壁じゃない
		return false;
	}
}

void Game::SetLevel()
{
	mLevelIdx = mRandom->rnd() % 5;
}

glm::vec3 Game::GetTreasurePos()
{
	char** levelData = mLevelArray[mLevelIdx];

	// Mazeの床描画
	for (int y = 5; y < 15; y++) {
		for (int x = 0; x < 15; x++) {
			float x_pos = 1.0f + 2.0f * x;
			float y_pos = 1.0f + 2.0f * y;

			if (levelData[y - 5][x] == '*') {
				// 宝箱の場所
				return glm::vec3(x_pos, y_pos, 0.f);
			}
		}
	}
}

uint32_t Game::GetRandom()
{
	return mRandom->rnd();
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
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


	for (auto actor : mActors) {
		actor->Update(deltaTime);
	}

	mSound->Update();
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



void Game::AddMeshComp(MeshComponent* mesh)
{
	if (mesh->GetIsSkeletal()) {
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

void Game::RemoveMeshComp(MeshComponent* mesh)
{
	if (mesh->GetIsSkeletal())
	{
		SkinMeshComponent* sk = static_cast<SkinMeshComponent*>(mesh);
		auto iter = std::find(mSkinMeshComps.begin(), mSkinMeshComps.end(), sk);
		mSkinMeshComps.erase(iter);
	}
	else
	{
		auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
		mMeshComps.erase(iter);
	}
}

void Game::RemoveSpriteComp(SpriteComponent* sprite)
{
	switch (sprite->GetType()) {
	case SpriteComponent::SPRITE:
	{
		auto iter = std::find(mSpriteComps.begin(), mSpriteComps.end(), sprite);
		mSpriteComps.erase(iter);
		break;
	}
	case SpriteComponent::TEXT:
	{
		TextComponent* text = static_cast<TextComponent*>(sprite);
		auto iter = std::find(mTextComps.begin(), mTextComps.end(), text);
		mTextComps.erase(iter);
		break;
	}
	case SpriteComponent::TALK_TEXT:
	{
		TalkTextComponent* text = static_cast<TalkTextComponent*>(sprite);
		auto iter = std::find(mTalkTextComps.begin(), mTalkTextComps.end(), text);
		mTalkTextComps.erase(iter);
		break;
	}
	}
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
	//glDisable(GL_BLEND);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);


	mMeshShader->UseProgram();
	for (auto mc : mMeshComps) {
		mc->Draw(mMeshShader);
	}

	// Draw Skin Mesh
	mSkinningShader->UseProgram();
	for (auto skinmesh : mSkinMeshComps) {
		skinmesh->Draw(mSkinningShader);
	}


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

void Game::UnloadData()
{
	// Delete actors
	while (!mActors.empty())
	{
		delete mActors.back();	// SpriteやText, TalkTextここでUnload
	}


	// Mesh
	for (auto i : mMeshes)
	{
		delete i.second;
	}
	mMeshes.clear();
	// SkinMesh
	for (auto i : mSkinMeshes) {
		delete i.second;
	}
	mSkinMeshes.clear();

}

void Game::Shutdown()
{
	UnloadData();

	// unload renderer
	delete mSkinningShader;
	delete mTextShader;
	delete mSpriteShader;
	delete mMeshShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
	// Sound
	mSound->ShutDown();
	delete(mSound);
	SDL_Quit();
}
