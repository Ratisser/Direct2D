#pragma once
#include <GameEngine/GameEngineCore.h>

class UserGame : public GameEngineCore
{
public:
	UserGame(); // default constructer ����Ʈ ������
	~UserGame(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	UserGame(const UserGame& _other) = delete; // default Copy constructer ����Ʈ ���������
	UserGame(UserGame&& _other) = delete; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	UserGame& operator=(const UserGame& _other) = delete; // default Copy operator ����Ʈ ���� ������
	UserGame& operator=(const UserGame&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

