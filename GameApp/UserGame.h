#pragma once
#include <GameEngine/GameEngineCore.h>

class UserGame : public GameEngineCore
{
public:
	UserGame();
	~UserGame();
	UserGame(const UserGame& _other) = delete;
	UserGame(UserGame&& _other) = delete;
	UserGame& operator=(const UserGame& _other) = delete;
	UserGame& operator=(const UserGame&& _other) = delete;

public:
	virtual float4 StartWindowPos() {
		return { 150, 100 };
	}
	virtual float4 StartWindowSize() {
		return { 1280, 720 };
	}

public:
	virtual void Initialize() override;
	virtual void ResourcesLoad() override;
	virtual void Release() override;

private:
	void loadLevel();
	void loadSound();
	void loadShader();
	void loadTexture();
	void loadMesh();

	void loadPipeLine();
};

