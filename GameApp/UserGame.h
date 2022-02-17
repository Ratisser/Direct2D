#pragma once
#include <GameEngine/GameEngineCore.h>

class UserGame : public GameEngineCore
{
public:
	UserGame(); // default constructer 디폴트 생성자
	~UserGame(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	UserGame(const UserGame& _other) = delete; // default Copy constructer 디폴트 복사생성자
	UserGame(UserGame&& _other) = delete; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	UserGame& operator=(const UserGame& _other) = delete; // default Copy operator 디폴트 대입 연산자
	UserGame& operator=(const UserGame&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	virtual float4 StartWindowPos() {
		return { 0, 0 };
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
};

