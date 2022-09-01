#define _CRT_SECURE_NO_WARNINGS


#include "Game.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/vector_angle.hpp"
#include "Player.hpp"
#include "Shader.hpp"
#include "Plane.hpp"
#include <fstream>
#include <codecvt>
#define STB_IMAGE_IMPLEMENTATION

Game::Game()
	:mWindowWidth(1024),
	mWindowHeight(768),
	mIsRunning(true),
	mMoveSpeed(0.1),
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


	mWindow = SDL_CreateWindow("Wander OpenGL Tutorial", 100, 100,
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

	//{
	//	// Shadow Map
	//	std::string vert_file = "./Shaders/ShadowMap.vert";
	//	std::string frag_file = "./Shaders/ShadowMap.frag";
	//	mShadowMapShader = new Shader();
	//	if (!mShadowMapShader->CreateShaderProgram(vert_file, frag_file)) {
	//		return false;
	//	}
	//}
	//mShadowMapShader->UseProgram();
	//mShadowMapShader->SetMatrixUniform("LightView", SpotLightView);
	//mShadowMapShader->SetMatrixUniform("CameraProj", CameraProj);
	//mShadowMapShader->SetSamplerUniform("gShadowMap", 1);

	//{
	//	// Shadow Lighting
	//	std::string vert_file = "./Shaders/ShadowLighting.vert";
	//	std::string frag_file = "./Shaders/ShadowLighting.frag";
	//	mShadowLightingShader = new Shader();
	//	if (!mShadowLightingShader->CreateShaderProgram(vert_file, frag_file)) {
	//		return false;
	//	}
	//}
	//mShadowLightingShader->UseProgram();
	//mShadowLightingShader->SetMatrixUniform("CameraView", CameraView);
	//mShadowLightingShader->SetMatrixUniform("CameraProj", CameraProj);
	//mShadowLightingShader->SetMatrixUniform("LightView", SpotLightView);
	//mShadowLightingShader->SetSamplerUniform("gShadowMap", 1);

	//// SkinMesh
	//{
	//	// Shadow Map
	//	std::string vert_file = "./Shaders/SkinningShadowMap.vert";
	//	std::string frag_file = "./Shaders/ShadowMap.frag";
	//	mSkinShadowMapShader = new Shader();
	//	if (!mSkinShadowMapShader->CreateShaderProgram(vert_file, frag_file)) {
	//		return false;
	//	}
	//}
	//mSkinShadowMapShader->UseProgram();
	//mSkinShadowMapShader->SetMatrixUniform("LightView", SpotLightView);
	//mSkinShadowMapShader->SetMatrixUniform("CameraProj", CameraProj);
	//mSkinShadowMapShader->SetSamplerUniform("gShadowMap", 1);

	//{
	//	// Shadow Lighting
	//	std::string vert_file = "./Shaders/SkinningShadowLighting.vert";
	//	std::string frag_file = "./Shaders/ShadowLighting.frag";
	//	mSkinShadowLightingShader = new Shader();
	//	if (!mSkinShadowLightingShader->CreateShaderProgram(vert_file, frag_file)) {
	//		return false;
	//	}
	//}
	//mSkinShadowLightingShader->UseProgram();
	//mSkinShadowLightingShader->SetMatrixUniform("CameraView", CameraView);
	//mSkinShadowLightingShader->SetMatrixUniform("CameraProj", CameraProj);
	//mSkinShadowLightingShader->SetMatrixUniform("LightView", SpotLightView);
	//mSkinShadowLightingShader->SetSamplerUniform("gShadowMap", 1);

	//// Unity Chan Shadow Lighting 
	//{
	//	// Shadow Lighting
	//	std::string vert_file = "./Shaders/SkinningShadowLighting.vert";
	//	std::string frag_file = "./Shaders/UnityChan.frag";
	//	mUnityChanShader = new Shader();
	//	if (!mUnityChanShader->CreateShaderProgram(vert_file, frag_file)) {
	//		return false;
	//	}
	//}
	//mUnityChanShader->UseProgram();
	//mUnityChanShader->SetMatrixUniform("CameraView", CameraView);
	//mUnityChanShader->SetMatrixUniform("CameraProj", CameraProj);
	//mUnityChanShader->SetMatrixUniform("LightView", SpotLightView);
	//mUnityChanShader->SetSamplerUniform("gShadowMap", 1);

	// light setting
	SetLighting();


	// Model読み込み処理
	//{
	//	// Concrete Plane読み込み
	//	Mesh* mesh = new Mesh();
	//	if (mesh->Load("./resources/ConcretePlane/", "ConcretePlane.obj")) {
	//		mesh->SetPos(glm::vec3(0.0f));
	//		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)M_PI / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//		mesh->SetRotate(rotate);
	//		//mesh->SetMeshRotate(glm::mat4(1.0f));
	//		mesh->SetScale(1.0f);
	//		mConcretePlane = mesh;
	//	}
	//}
	{
		Plane* plane = new Plane();
		if (plane->Load("./resources/Plane/", "Plane.obj")) {
			plane->LoadConcreteTex("./resources/Plane/Textures/concrete_brick_wall_001_diffuse_4k.jpg");
			plane->LoadBrickTex("./resources/Plane/Textures/Bricks077_4K_Color.jpg");
			plane->SetPos(glm::vec3(0.f));
			glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)M_PI / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			plane->SetRotate(rotate);
			plane->SetScale(1.f);
			mPlane = plane;
		}
	}
	{
		// Treasure Box
		Mesh* mesh = new Mesh();
		if (mesh->Load("./resources/TreasureBox/", "scene.gltf")) {
			mesh->SetPos(glm::vec3(4.0f, 5.0f / 2.0f, 0.0f));
			mesh->SetRotate(glm::mat4(1.0f));
			mesh->SetScale(0.01f / 2.0f);
			mMeshes.push_back(MeshData(mesh, "TreasureChest"));
		}
	}
	{
		// Maze Box
		MazeBox* mazeBox = new MazeBox("./resources/MazeBox/", 
			"MazeBox.fbx", "BlackUV.png", "WhiteUV.png");
		mazeBox->SetPos(glm::vec3(4.0f, 5.0f, 0.0f));
		mazeBox->SetRotate(glm::mat4(1.0f));
		mazeBox->SetScale(1.0f);
		mazeBox->SetBoxType(MazeBox::BLACK);
		mMazeBox = mazeBox;
		mMazeData = new float* [10];
		for (int y = 0; y < 10; y++) {
			mMazeData[y] = new float[15];
			for (int x = 0; x < 15; x++) {
				mMazeData[y][x] = 0.f;
			}
		}
	}



	{
		// Player
		Player* mesh = new Player(this);
		if (mesh->Load("./resources/SimpleMan/", "test_output3.fbx")) {
			mesh->SetPos(glm::vec3(29.0f, 1.0f, 0.0f));
			mesh->SetRotate(glm::mat4(1.0f));
			mesh->SetScale(0.002f);
			mPlayer = mesh;
		}
	}




	// Load Sprites
	{
		Sprite* sprite = new Sprite();
		if (sprite->Load("./resources/TextBox.png")) {
			sprite->SetPos(glm::vec3(0.0f, -mWindowHeight / 4.0f, 0.0f));
			sprite->SetRotate(glm::mat4(1.0f));
			sprite->SetScale(1.5f);
			sprite->SetAlpha(0.7f);
			mTextBox = sprite;;
			mSprites.push_back(SpriteData(sprite, "TextBox"));
		}
	}

	// Load Text
	mText = new Text();
	mText->SetPos(glm::vec3(0.0f));
	mText->SetRotate(glm::mat4(1.0f));
	mText->SetScale(1.0f);
	mText->SetAlpha(1.0f);
	mText->SetTextColor(glm::vec3(0.0f, 0.0f, 0.2f));

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

	// 会話に使う文字列読み込み
	{
		std::string filePath = "./resources/TextData.json";
		std::ifstream ifs(filePath.c_str());
		if (ifs.good())
		{
			ifs >> mTextData;
		}
		ifs.close();
	}






	return true;
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
	glm::vec3 playerNewPos = mPlayer->GetPos();
	if (keyState[SDL_SCANCODE_W]) {
		playerNewPos = mPlayer->GetPos() + mPlayer->GetForward() * (float)mMoveSpeed;
	}
	if (keyState[SDL_SCANCODE_S]) {
		playerNewPos = mPlayer->GetPos() - mPlayer->GetForward() * (float)mMoveSpeed;
	}
	if (keyState[SDL_SCANCODE_A]) {
		mPlayer->SetPlayerRot(mPlayer->GetPlayerRot() + 1.f);
	}
	if (keyState[SDL_SCANCODE_D]) {
		mPlayer->SetPlayerRot(mPlayer->GetPlayerRot() - 1.f);
	}

	// 受付と話す
	if (mPhase != PHASE_TALK) {
		if (
			(2.0f < mPlayer->GetPos().x) && (mPlayer->GetPos().x < 3.0f) &&
			(1.0f < mPlayer->GetPos().y) && (mPlayer->GetPos().y < 2.0f)
			) {
			if (keyState[SDL_SCANCODE_RETURN]) {
				mPhase = PHASE_TALK;
			}
		}
	}



	// Playerの行動範囲制限
	bool IsUpdatePlayerPos = false;
	// 壁の外には行けない
	if ((playerNewPos.x < 0.25f) || (playerNewPos.x > 29.75f) ||
		(playerNewPos.y < 0.25f) || (playerNewPos.y > 29.75f)
		) {
		IsUpdatePlayerPos = false;
	}
	else if (
		// カウンターの中には入れない
		(0.f < playerNewPos.x) && (playerNewPos.x < 2.f) &&
		(0.f < playerNewPos.y) && (playerNewPos.y < 3.5f)
		) {
		IsUpdatePlayerPos = false;
	}
	else if (mPhase == PHASE_TALK) {
		// 話し中なら動かない
		IsUpdatePlayerPos = false;
	}
	else {
		IsUpdatePlayerPos = true;
	}
	// PlayerがMazeの領域にあるか？
	if (
		(0.f < playerNewPos.x) && (playerNewPos.x < 30.f) &&
		(10.f < playerNewPos.y) && (playerNewPos.y < 30.f)
		) {
		if (mLevelData[static_cast<int>(playerNewPos.y / 2) - 5][static_cast<int>(playerNewPos.x / 2)] == '#') {
			// Playerのいる場所が壁なら更新しない
			IsUpdatePlayerPos = false;
		}
		else {
			IsUpdatePlayerPos = true;

		}
	}



	if (IsUpdatePlayerPos) {
		mPlayer->SetPos(playerNewPos);
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

	mPlayer->Update(deltaTime);




	// 更新されたカメラの位置をShaderに反映
	//std::vector<Shader*> Shaders;
	//Shaders.push_back(mMeshShader);
	//Shaders.push_back(mSkinningShader);
	//for (auto shader : Shaders) {
	//	shader->UseProgram();
	//	mCameraPos = mPlayerPos - glm::vec3(2.0f, 0.0f, 3.0f);
	//	mCameraPos.z = 3.0f;
	//	shader->SetVectorUniform("gEyeWorldPos", mCameraPos);
	//	shader->SetMatrixUniform("CameraView", glm::lookAt(mCameraPos, mCameraPos + mCameraOrientation, mCameraUP));
	//}



}

std::u16string Game::GetText(nl::json data)
{
	std::string str;
	data.get_to(str);
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	return convert.from_bytes(str);
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
	for (auto mesh : mMeshes) {
		if (mesh.meshName == "TreasureChest") {
			mesh.mesh->SetPos(glm::vec3(4.0f, 5.0f / 2.0f, 0.0f));
			mesh.mesh->Draw(mMeshShader, mTicksCount / 1000.0f);
			mesh.mesh->SetPos(glm::vec3(4.0f, 4.0f, 0.0f));
			mesh.mesh->Draw(mMeshShader, mTicksCount / 1000.0f);
		}
	}

	// 床描画
	mPlane->SetPlaneType(Plane::CONCRETE);
	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 15; x++) {
			float x_pos = 1.0f + 2.0f * x;
			float y_pos = 1.0f + 2.0f * y;
			glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)M_PI / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			mPlane->SetRotate(rotate);
			mPlane->SetPos(glm::vec3(x_pos, y_pos, 0.0f));
			mPlane->Draw(mMeshShader, mTicksCount / 1000.0f);
		}
	}

	// 壁描画
	mPlane->SetPlaneType(Plane::BRICK);
	for (int x = 0; x < 15; x++) {
		for (float z = 1.f; z <= 3.f; z += 2.f) {
			float x_pos = 1.0f + 2.0f * x;
			float y_pos = 0.f;
			mPlane->SetPos(glm::vec3(x_pos, y_pos, z));
			mPlane->SetRotate(glm::mat4(1.f));
			mPlane->Draw(mMeshShader, mTicksCount / 1000.0f);
			y_pos = 30.f;
			mPlane->SetPos(glm::vec3(x_pos, y_pos, z));
			mPlane->SetRotate(glm::mat4(1.f));
			mPlane->Draw(mMeshShader, mTicksCount / 1000.0f);
			y_pos = 0.f;
			mPlane->SetPos(glm::vec3(y_pos, x_pos, z));
			glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)M_PI / 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			mPlane->SetRotate(rotate);
			mPlane->Draw(mMeshShader, mTicksCount / 1000.0f);
			y_pos = 30.f;
			mPlane->SetPos(glm::vec3(y_pos, x_pos, z));
			mPlane->SetRotate(rotate);
			mPlane->Draw(mMeshShader, mTicksCount / 1000.0f);
		}
	}

	// 受付カウンター描画
	mMazeBox->SetScale(0.25f);
	for (int z = 0; z < 2; z++) {
		for (int y = 0; y < 7; y++) {
			mMazeBox->SetBoxType((z * 7 + y) % 2 ? MazeBox::BLACK : MazeBox::WHITE);
			mMazeBox->SetPos(glm::vec3(1.75f, 0.5f * y + 0.25, 0.5f * z + 0.5f));
			mMazeBox->Draw(mMeshShader, mTicksCount / 1000.0f);
		}
		for (int x = 0; x < 3; x++) {
			mMazeBox->SetBoxType((z * 3 + x) % 2 ? MazeBox::WHITE : MazeBox::BLACK);
			mMazeBox->SetPos(glm::vec3(0.5f * x + 0.25, 3.25f, 0.5f * z + 0.5f));
			mMazeBox->Draw(mMeshShader, mTicksCount / 1000.0f);
		}
	}

	// Mazeの床描画
	mMazeBox->SetScale(1.0f);
	for (int y = 5; y < 15; y++) {
		for (int x = 0; x < 15; x++) {
			mMazeBox->SetBoxType((y * 15 + x) % 2 ? MazeBox::BLACK : MazeBox::WHITE);
			float x_pos = 1.0f + 2.0f * x;
			float y_pos = 1.0f + 2.0f * y;

			if (mLevelData[y - 5][x] == '-') {
				// 通路
				mMazeBox->SetPos(glm::vec3(x_pos, y_pos, 0.f));
			}
			else {
				// 通れない
				glm::vec3 playerPos = mPlayer->GetPos();
				if (
					(x_pos - 1.5 <= playerPos.x) && (playerPos.x <= x_pos + 1.5) &&
					(y_pos - 1.5 <= playerPos.y) && (playerPos.y <= y_pos + 1.5)
					) {
					if (mMazeData[y - 5][x] <= 2.0f) {
						mMazeData[y - 5][x] += 0.01;
					}
				}
				else {
					if (mMazeData[y - 5][x] >= 0.f) {
						mMazeData[y - 5][x] -= 0.01;
					}
				}
				mMazeBox->SetPos(glm::vec3(x_pos, y_pos, mMazeData[y - 5][x]));
			}

			mMazeBox->Draw(mMeshShader, mTicksCount / 1000.0f);
		}
	}


	// Draw Skin Mesh
	mSkinningShader->UseProgram();
	mPlayer->Draw(mSkinningShader, mTicksCount / 1000.0f);

	// Draw Sprites
	glDisable(GL_DEPTH_TEST);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);


	// 文字描画
	// エンターキーではなす
	if (mPhase != PHASE_TALK) {
		if (
			(2.0f < mPlayer->GetPos().x) && (mPlayer->GetPos().x < 3.0f) &&
			(1.0f < mPlayer->GetPos().y) && (mPlayer->GetPos().y < 2.0f)
			) {
			mText->SetPos(glm::vec3(0.0f, 300.f, 0.f));
			mText->SetText(u"エンターキーで話す");
			mText->SetTextColor(glm::vec3(0.f, 0.f, 0.2f));
			mText->Draw(mTextShader);
		}
	}

	// 話し中ならトーク画面描画
	if (mPhase == PHASE_TALK) {
	}
	mTextBox->Draw(mSpriteShader);

	{
		mText->SetText(GetText(mTextData["TalkCostomer"]["Welcome"]["Talk2"]));
		mText->SetPos(glm::vec3(0.0f, -150.0f, 0.f));
		mText->SetTextColor(glm::vec3(0.f, 0.f, 0.2f));
		mText->DrawTalkText(mTextShader);
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

void Game::Shutdown()
{

}
