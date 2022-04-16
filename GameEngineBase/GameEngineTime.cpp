#include "PreCompile.h"
#include "GameEngineTime.h"

// Static Var
GameEngineTime* GameEngineTime::Inst = new GameEngineTime();
// Static Func
// constructer destructer
GameEngineTime::GameEngineTime()
	: timeScale_(1.0f)
{
	TimeCheckReset();
}

GameEngineTime::~GameEngineTime()
{
}

// 시간잴 준비를 한다.
void GameEngineTime::TimeCheckReset()
{
	// window의 힘을 빌었습니다.
	QueryPerformanceFrequency(&timeCount_);
	QueryPerformanceCounter(&startCheck_);
	QueryPerformanceCounter(&endCheck_);
	timeScale_ = 1.0f;
}

void GameEngineTime::TimeCheck()
{
	QueryPerformanceCounter(&endCheck_);
	deltaTime_ = static_cast<double>((endCheck_.QuadPart - startCheck_.QuadPart)) / static_cast<double>(timeCount_.QuadPart);
	startCheck_.QuadPart = endCheck_.QuadPart;
}

void GameEngineTime::SetTimeScale(float _scale)
{
	timeScale_ = _scale;
}
