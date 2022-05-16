#include "PreCompile.h"
#include "DevilTear.h"

DevilTear::DevilTear()
{

}

DevilTear::~DevilTear()
{

}

void DevilTear::Start()
{
	ParryObjectBase::Start();
	
}

void DevilTear::Update(float _deltaTime)
{
	
}

void DevilTear::onParry()
{
	Release();
}
