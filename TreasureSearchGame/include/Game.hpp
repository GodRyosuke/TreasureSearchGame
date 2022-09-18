#pragma once

#include "SDL.h"
#include "glew.h"
#include "MazeBox.hpp"
#include "json.hpp"

namespace nl = nlohmann;

class Game {
public:
	enum PHASE {
		PHASE_IDLE,
		PHASE_MOVE,
		PHASE_TALK,
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
	void AddSprite(class Sprite* sprite) { mSprites.push_back(sprite); }
	void RemoveSprite(class Sprite* sprite);

	Player* GetPlayer() { return mPlayer; }

	void SetPhase(PHASE phase) { mPhase = phase; }
	PHASE GetPhase() { return mPhase; }

	// posÇ™ï«Ç©ÅH
	bool IsWall(glm::vec3 pos);

	void AddMeshComp(class MeshComponent* meshcomp) { mMeshComponent.push_back(meshcomp); }
	void RemoveMesh(class Mesh* mesh);
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
	void SetLighting();
	std::u16string GetText(nl::json data);

	const int mWindowWidth;
	const int mWindowHeight;
	bool mIsRunning;

	std::vector<SpotLight> mSpotLights;


	class Sprite* mTextBox;
	class Mesh* mConcretePlane;
	class Mesh* mRoof;
	
	class Text* mText;
	nl::json mTextData;

	class Player* mPlayer;

	class Plane* mPlane;
	MazeBox* mMazeBox;
	float** mMazeData;
	char** mLevelData;

	

	class Shader* mSkinningShader;
	class Shader* mMeshShader;
	class Shader* mSpriteShader;
	class Shader* mTextShader;


	PHASE mPhase;

	// Camera
	glm::vec3 mCameraUP;
	glm::vec3 mCameraOrientation;
	float mMoveSensitivity;

	glm::vec3 mMousePos;


	std::vector<class Actor*> mActors;
	std::vector<class Sprite*> mSprites;
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