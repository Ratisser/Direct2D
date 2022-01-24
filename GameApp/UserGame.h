#pragma once
#include <GameEngine/GameEngineCore.h>

class UserGame : public GameEngineCore
{
public:
	UserGame(); // default constructer ����Ʈ ������
	~UserGame(); // default destructer ����Ʈ �Ҹ���
	UserGame(const UserGame& _other) = delete; // default Copy constructer ����Ʈ ���������
	UserGame(UserGame&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	UserGame& operator=(const UserGame& _other) = delete; // default Copy operator ����Ʈ ���� ������
	UserGame& operator=(const UserGame&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

	virtual void Initialize() override;
	virtual void LoadResource() override;
	virtual void GameLoop() override;
	virtual void Release() override;
};

