#include "PreCompile.h"
#include "EndingLevel.h"

#include <GameApp\FadeIn.h>
#include "EndingActor.h"

EndingLevel::EndingLevel()
	:endingActor_(nullptr)
{

}

EndingLevel::~EndingLevel()
{

}

void EndingLevel::LevelChangeEndEvent()
{
	if (endingActor_ != nullptr)
	{
		endingActor_->Release();
		endingActor_ = nullptr;
	}

	
}

void EndingLevel::LevelChangeStartEvent()
{
	CreateActor<FadeIn>();
	endingActor_ = CreateActor<EndingActor>();

	mainCamera_->GetTransform()->SetLocationZ(-100.f);
	GetMainCameraComponent()->SetProjectionMode(ProjectionMode::Orthographic);
}

void EndingLevel::LevelStart()
{

}

void EndingLevel::LevelUpdate(float _deltaTime)
{
}
