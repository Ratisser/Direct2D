#include "PreCompile.h"
#include "GameEngineObjectBase.h"

GameEngineObjectBase::GameEngineObjectBase() // default constructer 디폴트 생성자
	: bRelease_(false),
	bUpdate_(true),
	bDebuging_(false), // 디버깅 기능 이거 true로 해놓고 f9 걸어 놓으면 
	parent_(nullptr),
	order_(0)
{

}

GameEngineObjectBase::~GameEngineObjectBase() // default destructer 디폴트 소멸자
{

}
