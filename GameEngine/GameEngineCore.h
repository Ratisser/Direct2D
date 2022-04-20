#pragma once
#include <GameEngineBase\GameEngineObjectBase.h>
#include <GameEngineBase\GameEngineThreadQueue.h>

#include "GameEngineLevel.h"

class GameEngineCore : public GameEngineObjectBase
{
	friend class GameEngineLevelControlWindow;
public:
	static GameEngineThreadQueue ThreadQueue_;

public:
	template<typename UserGameType>
	static void Start();

	template <typename T>
	static void CreateLevel(const std::string& _levelName);

	static void ChangeLevel(const std::string& _levelName);

public:
	virtual float4 StartWindowSize() = 0;
	virtual float4 StartWindowPos() = 0;

protected:
	virtual void Initialize() = 0;
	virtual void ResourcesLoad() = 0;
	virtual void Release() = 0;

	GameEngineCore();
	~GameEngineCore();
	GameEngineCore(const GameEngineCore& _other) = delete;
	GameEngineCore(GameEngineCore&& _other) noexcept = delete;
	GameEngineCore& operator=(const GameEngineCore& _other) = delete;
	GameEngineCore& operator=(const GameEngineCore&& _other) = delete;

private:
	void EngineInitialize();
	void EngineDestroy();

	void loadEngineTexture();
	void loadEngineShader();
	void loadEngineResource();


private:
	static void WindowCreate(GameEngineCore& _RuntimeCore);
	static void Loop();
	static void MainLoop();

private:
	static GameEngineCore* MainCore_;
	static std::map<std::string, GameEngineLevel*> allLevels_;
	static GameEngineLevel* nextLevel_;
	static GameEngineLevel* currentLevel_;
};
















template<typename UserGameType>
inline void GameEngineCore::Start()
{
	GameEngineDebug::LeakCheckOn();

#ifdef _DEBUG
	new int();
#endif
	UserGameType NewUserGame;
	WindowCreate(NewUserGame);

	NewUserGame.EngineInitialize();
	NewUserGame.ResourcesLoad();
	NewUserGame.Initialize();

	MainCore_ = &NewUserGame;

	Loop();

	NewUserGame.Release();
	NewUserGame.EngineDestroy();
}

template<typename T>
inline void GameEngineCore::CreateLevel(const std::string& _levelName)
{
	if (allLevels_.end() != allLevels_.find(_levelName))
	{
		GameEngineDebug::MsgBoxError("중복된 이름의 Level을 만들려고 했습니다. : " + _levelName);
	}

	GameEngineLevel* newLevel = new T;
	newLevel->SetName(_levelName);
	newLevel->init();
	newLevel->LevelStart();

	allLevels_.insert(std::pair<std::string, GameEngineLevel*>(_levelName, newLevel));
}
