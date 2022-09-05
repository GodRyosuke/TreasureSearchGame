#pragma once

#include "SDL.h"
#include "glew.h"
#include "TextureShadowMap.hpp"
#include "UnityChan.hpp"
#include "Sprite.hpp"
#include "Text.hpp"
#include "MazeBox.hpp"
#include "json.hpp"

namespace nl = nlohmann;

class Game {
public:
	Game();
	~Game() {}

	bool Initialize();
	void RunLoop();
	void Shutdown();
	void SetViewMatrix(glm::mat4 view, glm::vec3 cameraPos);	// View MatrixÇÃê›íËÇÇ∑ÇÈÅB

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class Sprite* sprite) { mSprites.push_back(sprite); }
	void RemoveSprite(class Sprite* sprite);

	void AddMesh(class Mesh* mesh) { mMeshes.push_back(mesh); }
	void RemoveMesh(class Mesh* mesh);
	Uint32 GetTicksCount() { return mTicksCount; }


private:
	enum PHASE {
		PHASE_IDLE,
		PHASE_MOVE,
		PHASE_TALK,
		PHASE_MAX
	};
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


	Sprite* mTextBox;
	Mesh* mConcretePlane;
	Mesh* mRoof;
	
	Text* mText;
	nl::json mTextData;

	std::vector<SkinMesh*> mSkinMeshes;
	class Player* mPlayer;

	class Plane* mPlane;
	MazeBox* mMazeBox;
	float** mMazeData;
	char** mLevelData;

	
	UnityChan* mAnimUnityChan;


	TextureShadowMap* mTextureShadowMapFBO;

	class Shader* mSkinningShader;
	class Shader* mMeshShader;
	class Shader* mSpriteShader;
	class Shader* mTextShader;


	PHASE mPhase;

	// Camera
	glm::vec3 mCameraUP;
	glm::vec3 mCameraOrientation;
	float mMoveSpeed;
	float mMoveSensitivity;

	glm::vec3 mMousePos;


	std::vector<class Sprite*> mSprites;
	std::vector<class Mesh*> mMeshes;

	SDL_Window* mWindow;
	// OpenGL context
	SDL_GLContext mContext;
	Uint32 mTicksCount;
};