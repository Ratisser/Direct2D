#pragma once

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineObjectBase
{
private:	// member Var
	// true��� �ϴ¼��� �޸������� �ı��ɰ̴ϴ�.
	bool bRelease_;
	bool bUpdate_;
	bool bDebuging_;
	GameEngineObjectBase* parent_;
	int order_;

public:
	bool IsNull() 
	{
		return nullptr == this;
	}

	void SetParent(GameEngineObjectBase* _parent)
	{
		parent_ = _parent;
	}

public:
	bool IsDeath()
	{
		if (nullptr == parent_)
		{
			return bRelease_;
		}

		return true == bRelease_ || parent_->bRelease_;
	}

	bool IsUpdate()
	{
		if (nullptr == parent_)
		{
			return false == IsDeath() && bUpdate_;
		}

		return false == IsDeath() && true == bUpdate_ && parent_->bUpdate_;
	}


	void Death()
	{
		bRelease_ = true;
	}

	void On()
	{
		bUpdate_ = true;
	}

	void Off()
	{
		bUpdate_ = false;
	}

	void DebugOn()
	{
		bDebuging_ = true;
	}

	void DebugOff()
	{
		bDebuging_ = false;
	}

	void IsDebugCheck()
	{
		if (true == bDebuging_)
		{
			int a = 0;
		}
	}

	void SetOrder(int _Order)
	{
		order_ = _Order;
	}

	int GetOrder()
	{
		return order_;
	}

public:
	GameEngineObjectBase(); // default constructer ����Ʈ ������
	virtual ~GameEngineObjectBase(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineObjectBase(const GameEngineObjectBase& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineObjectBase(GameEngineObjectBase&& _other) = delete; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineObjectBase& operator=(const GameEngineObjectBase& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineObjectBase& operator=(const GameEngineObjectBase&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
};

