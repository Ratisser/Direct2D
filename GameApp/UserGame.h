#pragma once
#include <GameEngine/GameEngineCore.h>

// 분류 : 
// 용도 : 
// 설명 : 
class UserGame : public GameEngineCore
{
public:
	UserGame(); // default constructer 디폴트 생성자
	~UserGame(); // default destructer 디폴트 소멸자
	UserGame(const UserGame& _other) = delete; // default Copy constructer 디폴트 복사생성자
	UserGame(UserGame&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	UserGame& operator=(const UserGame& _other) = delete; // default Copy operator 디폴트 대입 연산자
	UserGame& operator=(const UserGame&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

	virtual void Initialize() override;
	virtual void LoadResource() override;
	virtual void GameLoop() override;
	virtual void Release() override;
};

