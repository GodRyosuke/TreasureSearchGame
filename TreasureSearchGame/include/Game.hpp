#pragma once

#include "SDL.h"
#include "glew.h"
#include "glm.hpp"
#include <string>
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION

class Game {
public:
	enum PHASE {
		PHASE_NORMAL,
		PHASE_GAME,
		PHASE_FAIL_GAME,
		PHASE_SUCCSESS_GAME,
		PHASE_MAX
	};
	Game();
	~Game() {}

	bool Initialize();
	void RunLoop();
	void Shutdown();
	void SetViewMatrix(glm::mat4 view, glm::vec3 cameraPos);	// View Matrixの設定をする。

	void AddActor(class Actor* actor) { mActors.push_back(actor); }
	void RemoveActor(class Actor* actor);

	class Mesh* GetMesh(std::string filePath, std::string ext);
	class SkinMesh* GetSkinMesh(std::string filePath, std::string ext);

	void AddSpriteComp(class SpriteComponent* sprite);
	void RemoveSpriteComp(class SpriteComponent* sprite);
	void RemoveTextComp(class TextComponent* text);

	uint32_t GetRandom();
	class Sound* GetSound() { return mSound; }

	class Player* GetPlayer() { return mPlayer; }
	class Clerk* GetClerk() { return mClerk; }
	class TalkText* GetTalkText() { return mTalkText; }
	class TreasureBox* GetTreasureBox() { return mTreasureBox; }

	void SetPhase(PHASE phase) { mPhase = phase; }
	PHASE GetPhase() { return mPhase; }

	// posが壁か？
	bool IsWall(glm::vec3 pos);
	void SetLevel();	// 迷路のに内容をランダムに設定
	glm::vec3 GetTreasurePos();

	void AddMeshComp(class MeshComponent* meshcomp);
	void RemoveMeshComp(class MeshComponent* mesh);
	Uint32 GetTicksCount() { return mTicksCount; }


private:
	struct BaseLight {
		glm::vec3 Color;
		float AmbientIntensity;
		float DiffuseIntensity;
	};
	struct Attenuation {
		float Constant;
		float Linear;
		float Exp;
	};
	struct PointLight {
		BaseLight Base;
		glm::vec3 Position;
		Attenuation Atten;
	};
	struct SpotLight {
		PointLight pointLight;
		glm::vec3 Direction;
		glm::vec3 Up;
		float Cutoff;
	};

	void ProcessInput();
	void UpdateGame();
	void Draw();

	bool LoadData();
	void UnloadData();
	void SetLighting();

	const int mWindowWidth;
	const int mWindowHeight;
	bool mIsRunning;

	std::vector<SpotLight> mSpotLights;


	class Player* mPlayer;
	class Clerk* mClerk;
	class TalkText* mTalkText;
	class TreasureBox* mTreasureBox;
	class Plane* mPlane;

	float** mMazeData;
	std::vector<char**> mLevelArray;
	int mLevelIdx;
	
	// Shaders
	class Shader* mSkinningShader;
	class Shader* mMeshShader;
	class Shader* mSpriteShader;
	class Shader* mTextShader;

	// Phase
	PHASE mPhase;

	class Random* mRandom;
	class Sound* mSound;

	std::vector<class Actor*> mActors;
	std::vector<class SpriteComponent*> mSpriteComps;
	std::vector<class TextComponent*> mTextComps;
	std::vector<class TalkTextComponent*> mTalkTextComps;
	std::unordered_map<std::string, class Mesh*> mMeshes;
	std::vector<class MeshComponent*> mMeshComps;
	std::unordered_map<std::string, class SkinMesh*> mSkinMeshes;
	std::vector<class SkinMeshComponent*> mSkinMeshComps;

	SDL_Window* mWindow;
	SDL_GLContext mContext;
	Uint32 mTicksCount;
};