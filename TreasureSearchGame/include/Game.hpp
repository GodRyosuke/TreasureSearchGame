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
		PHASE_IDLE,
		PHASE_MOVE,
		PHASE_TALK,
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
	void SetViewMatrix(glm::mat4 view, glm::vec3 cameraPos);	// View MatrixÇÃê›íËÇÇ∑ÇÈÅB

	void AddActor(class Actor* actor) { mActors.push_back(actor); }
	void RemoveActor(class Actor* actor);

	class Mesh* GetMesh(std::string filePath, std::string ext);
	class SkinMesh* GetSkinMesh(std::string filePath, std::string ext);
	void AddSpriteComp(class SpriteComponent* sprite);
	void RemoveSpriteComp(class SpriteComponent* sprite);
	void RemoveTextComp(class TextComponent* text);

	uint32_t GetRandom();

	class Player* GetPlayer() { return mPlayer; }
	class Clerk* GetClerk() { return mClerk; }
	class TalkText* GetTalkText() { return mTalkText; }
	class TreasureBox* GetTreasureBox() { return mTreasureBox; }
	class Sound* GetSound() { return mSound; }

	void SetPhase(PHASE phase) { mPhase = phase; }
	PHASE GetPhase() { return mPhase; }

	// posÇ™ï«Ç©ÅH
	bool IsWall(glm::vec3 pos);
	void SetLevel();
	glm::vec3 GetTreasurePos();

	void AddMeshComp(class MeshComponent* meshcomp);
	void RemoveMeshComp(class MeshComponent* mesh);
	//void RemoveMeshComp(class SkinMeshComponent* mesh);
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


	class SpriteComponent* mTextBox;
	class Mesh* mConcretePlane;
	class Mesh* mRoof;
	
	class Text* mText;
	//nl::json mTextData;

	class Player* mPlayer;
	class Clerk* mClerk;
	class TalkText* mTalkText;
	class TreasureBox* mTreasureBox;

	class Plane* mPlane;
	float** mMazeData;
	//char** mLevelData;
	std::vector<char**> mLevelArray;
	int mLevelIdx;

	

	class Shader* mSkinningShader;
	class Shader* mMeshShader;
	class Shader* mSpriteShader;
	class Shader* mTextShader;

	class Random* mRandom;


	PHASE mPhase;

	class Sound* mSound;


	// Camera
	glm::vec3 mCameraUP;
	glm::vec3 mCameraOrientation;
	float mMoveSensitivity;

	glm::vec3 mMousePos;


	std::vector<class Actor*> mActors;
	std::vector<class SpriteComponent*> mSpriteComps;
	std::vector<class TextComponent*> mTextComps;
	std::vector<class TalkTextComponent*> mTalkTextComps;
	std::unordered_map<std::string, class Mesh*> mMeshes;
	std::vector<class MeshComponent*> mMeshComps;
	//std::vector<class Mesh*> mMeshes;
	std::unordered_map<std::string, class SkinMesh*> mSkinMeshes;
	std::vector<class SkinMeshComponent*> mSkinMeshComps;

	SDL_Window* mWindow;
	// OpenGL context
	SDL_GLContext mContext;
	Uint32 mTicksCount;
};