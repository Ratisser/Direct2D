#include "PreCompile.h"
#include "DevilChipPiece.h"

#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine\GameEngineImageRenderer.h>

DevilChipPiece::DevilChipPiece()
	: timeCounter_(0.0f)
{

}

DevilChipPiece::~DevilChipPiece()
{

}

void DevilChipPiece::Start()
{
	GameEngineRandom random;
	for (int i = 0; i < MAX_PIECES; i++)
	{
		Piece newPiece;
		newPiece.Transform = CreateTransformComponent<GameEngineTransformComponent>();
		newPiece.Transform->SetLocationZ(-0.001f * i);
		newPiece.Renderer = CreateTransformComponent<GameEngineImageRenderer>(newPiece.Transform);
		std::string ftName = "ChipPiece" + std::to_string(i % 4);
		newPiece.Renderer->CreateAnimationFolder(ftName);
		newPiece.Renderer->ChangeAnimation(ftName);

		newPiece.Speed.x = random.RandomFloat(0, MAX_HORIZONTAL_SPEED) - MAX_HORIZONTAL_SPEED / 2.f;
		newPiece.Speed.y = random.RandomFloat(1000.f, MAX_VERTICAL_SPEED);

		pieces_.push_back(newPiece);
	}
}

void DevilChipPiece::Update(float _deltaTime)
{
	timeCounter_ += _deltaTime;

	for (Piece& p : pieces_)
	{
		p.Speed.y -= GRAVITY_POWER * _deltaTime;
		p.Transform->AddLocation(p.Speed * _deltaTime);
	}

	if (timeCounter_ > LIFE_TIME)
	{
		Release();
	}
}
