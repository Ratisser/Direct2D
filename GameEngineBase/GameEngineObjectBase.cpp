#include "PreCompile.h"
#include "GameEngineObjectBase.h"

GameEngineObjectBase::GameEngineObjectBase() // default constructer ����Ʈ ������
	: isDeath_(false),
	isUpdate_(true),
	isDebug_(false), // ����� ��� �̰� true�� �س��� f9 �ɾ� ������ 
	parent_(nullptr),
	order_(0)
{

}

GameEngineObjectBase::~GameEngineObjectBase() // default destructer ����Ʈ �Ҹ���
{

}
