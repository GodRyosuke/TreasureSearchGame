#pragma once

#include "SDL.h"
#include "glew.h"
#include "TextureShadowMap.hpp"
#include "Skinning.hpp"
#include "Shader.hpp"
#include "UnityChan.hpp"
#include "Sprite.hpp"
#include "Text.hpp"

class Game {
public:
	Game();
	~Game() {}

	bool Initialize();
	void RunLoop();
	void Shutdown();

private:
	enum PHASE {
		PHASE_IDLE,
		PHASE_MOVE,
		PHASE_MAX
	};
	struct SpotLight {
		glm::vec3 Position;
		glm::vec3 Direction;
		glm::vec3 Up;
	};

	void ProcessInput();
	void UpdateGame();
	void Draw();

	bool LoadData();
	void SetShaderLighting();

	const int mWindowWidth;
	const int mWindowHeight;
	bool mIsRunning;

	SpotLight mSpotLight;

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
	
	Text* mText;

	std::vector<SkinMesh*> mSkinMeshes;
	Mesh* mUnityChan;
	SkinMesh* mRunAnim;
	UnityChan* mAnimUnityChan;

	TextureShadowMap* mTextureShadowMapFBO;

	Shader* mSkinningShader;
	Shader* mMeshShader;
	Shader* mSpriteShader;
	Shader* mTextShader;

	Shader* mShadowMapShader;
	Shader* mShadowLightingShader;
	Shader* mSkinShadowMapShader;
	Shader* mSkinShadowLightingShader;
	Shader* mUnityChanShader;

	PHASE mPhase;

	// Camera
	glm::vec3 mCameraPos;
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