#pragma once
#include <GameEngineBase/GameEngineObjectBase.h>

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineCore : public GameEngineObjectBase
{
public:
	void EngineDestroy();

protected:
	GameEngineCore(); // default constructer 디폴트 생성자
	~GameEngineCore(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineCore(const GameEngineCore& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineCore(GameEngineCore&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineCore& operator=(const GameEngineCore& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineCore& operator=(const GameEngineCore&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

private:
	static GameEngineCore* MainCore;

private:
	static void WindowCreate();
	static void Loop();
	static void MainLoop();

public:
	template<typename UserGameType>
	static void Start() 
	{
		WindowCreate();

		UserGameType NewUserGame;

		NewUserGame.Initialize();
		NewUserGame.ResourcesLoad();

		MainCore = &NewUserGame;

		Loop();

		NewUserGame.Release();
	}

protected:
	virtual void Initialize() = 0;
	virtual void ResourcesLoad() = 0;
	virtual void GameLoop() = 0;
	virtual void Release() = 0;

public:
};

