#pragma once

#include <GameEngine\GameEngineLevel.h>

class LoadingLevel : public GameEngineLevel
{
public:
	LoadingLevel(); 
	~LoadingLevel();

	LoadingLevel(const LoadingLevel& _other) = delete; 
	LoadingLevel(LoadingLevel&& _other) = delete; 

	LoadingLevel& operator=(const LoadingLevel& _other) = delete;
	LoadingLevel& operator=(const LoadingLevel&& _other) = delete;

public:
	// GameEngineLevel��(��) ���� ��ӵ�
	virtual void LevelChangeEndEvent() override;
	virtual void LevelChangeStartEvent() override;
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _deltaTime) override;

private:
	void loadLevel();
	void loadSound();
	void loadShader();
	void loadTexture();
	void loadMesh();

	void loadPipeLine();

private:
	bool bLoading_;
	bool bFinish_;
};

