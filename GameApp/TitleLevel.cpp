#include "PreCompile.h"

#include "TitleLevel.h"
#include "TitleActor.h"

TitleLevel::TitleLevel()
{
}

TitleLevel::~TitleLevel()
{

}

void TitleLevel::LevelChangeEndEvent()
{
	
}

void TitleLevel::LevelChangeStartEvent()
{


}

void TitleLevel::LevelStart()
{
	mainCamera_->GetCameraComponent()->SetProjectionMode(ProjectionMode::Orthographic);
	mainCamera_->GetTransform()->SetLocationZ(-300.f);


	CreateActor<TitleActor>();
}

void TitleLevel::LevelUpdate(float _deltaTime)
{
}