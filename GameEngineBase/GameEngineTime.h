#pragma once
#include <Windows.h>
// #include <chrono>

// 분류 :
// 용도 :
// 설명 :
class GameEngineTime
{
public:
	GameEngineTime();
	~GameEngineTime();
	GameEngineTime(const GameEngineTime& _Other) = delete;
	GameEngineTime(const GameEngineTime&& _Other) = delete;
	GameEngineTime& operator=(const GameEngineTime& _Other) = delete;
	GameEngineTime& operator=(const GameEngineTime&& _Other) = delete;

public:
	static GameEngineTime& GetInst() { return *Inst; }
	static void Destroy()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = nullptr;
		}
	}

public:
	double GetDeltaTimeD() { return deltaTime_; }
	float GetDeltaTime() { return static_cast<float>(deltaTime_) * timeScale_; }

public:
	void TimeCheckReset();
	void TimeCheck();
	void SetTimeScale(float _scale);

private:
	static GameEngineTime* Inst;

private:
	LARGE_INTEGER timeCount_;
	LARGE_INTEGER startCheck_;
	LARGE_INTEGER endCheck_;
	double deltaTime_;
	float timeScale_;
};

