#include "PreCompile.h"
#include "GameEngineObjectBase.h"

GameEngineObjectBase::GameEngineObjectBase() // default constructer ����Ʈ ������
	: bRelease_(false),
	bUpdate_(true),
	bDebuging_(false), // ����� ��� �̰� true�� �س��� f9 �ɾ� ������ 
	parent_(nullptr),
	order_(0)
{

}

GameEngineObjectBase::~GameEngineObjectBase() // default destructer ����Ʈ �Ҹ���
{

}
