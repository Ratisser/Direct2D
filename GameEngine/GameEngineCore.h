#pragma once
#include <GameEngineBase/GameEngineObjectBase.h>

#include "GameEngineLevel.h"

class GameEngineCore : public GameEngineObjectBase
{
private:
	void EngineInitialize();
	void EngineDestroy();

	void loadEngineTexture();
	void loadEngineShader();
	void loadEngineResource();

protected:
	GameEngineCore();
	~GameEngineCore();

protected:		// delete constructer
	GameEngineCore(const GameEngineCore& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineCore(GameEngineCore&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineCore& operator=(const GameEngineCore& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineCore& operator=(const GameEngineCore&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

private:
	static GameEngineCore* MainCore_;

private:
	static void WindowCreate(GameEngineCore& _RuntimeCore);
	static void Loop();
	static void MainLoop();

public:
	template<typename UserGameType>
	static void Start() 
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

protected:
	virtual void Initialize() = 0;
	virtual void ResourcesLoad() = 0;
	virtual void Release() = 0;

public:
	virtual float4 StartWindowSize() = 0;
	virtual float4 StartWindowPos() = 0;

private:
	static std::map<std::string, GameEngineLevel*> allLevels_;
	static GameEngineLevel* nextLevel_;
	static GameEngineLevel* currentLevel_;

public:
	static void ChangeLevel(const std::string& _levelName);

	template <typename T>
	static void CreateLevel(const std::string& _levelName)
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

};

