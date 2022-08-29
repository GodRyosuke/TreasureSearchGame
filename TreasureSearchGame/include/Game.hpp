#pragma once

#include "SDL.h"
#include "glew.h"
#include "TextureShadowMap.hpp"
#include "UnityChan.hpp"
#include "Sprite.hpp"
#include "Text.hpp"
#include "MazeBox.hpp"

class Game {
public:
	Game();
	~Game() {}

	bool Initialize();
	void RunLoop();
	void Shutdown();
	void SetViewMatrix(glm::mat4 view, glm::vec3 cameraPos);	// View MatrixÇÃê›íËÇÇ∑ÇÈÅB


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

	const int mWindowWidth;
	const int mWindowHeight;
	bool mIsRunning;

	std::vector<SpotLight> mSpotLights;

	struct MeshData {
		MeshData(Mesh* mesh, std::string name)
			:mesh(mesh),
			meshName(name)
		{
		}
		std::string meshName;
		Mesh* mesh;
	};
	struct SpriteData {
		SpriteData(Sprite* sprite, std::string name)
			:sprite(sprite),spriteName(name){}
		Sprite* sprite;
		std::string spriteName;
	};
	std::vector<MeshData> mMeshes;
	std::vector<SpriteData> mSprites;
	Sprite* mTextBox;
	Mesh* mConcretePlane;
	
	Text* mText;

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

	SDL_Window* mWindow;
	// OpenGL context
	SDL_GLContext mContext;
	Uint32 mTicksCount;
};