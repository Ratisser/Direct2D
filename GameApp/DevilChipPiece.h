#pragma once

#include <GameEngine\GameEngineActor.h>

class GameEngineImageRenderer;
class DevilChipPiece : public GameEngineActor
{
public:
	DevilChipPiece();
	~DevilChipPiece();

	DevilChipPiece(const DevilChipPiece& _other) = delete; 
	DevilChipPiece(DevilChipPiece&& _other) = delete; 

	DevilChipPiece& operator=(const DevilChipPiece& _other) = delete;
	DevilChipPiece& operator=(const DevilChipPiece&& _other) = delete;

public:
	// GameEngineActor을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;
	
private:
	struct Piece
	{
		GameEngineTransformComponent* Transform = nullptr;
		GameEngineImageRenderer* Renderer = nullptr;
		float4 Speed;
	};

private:
	const int MAX_PIECES = 10;
	const float GRAVITY_POWER = 10000.f;
	const float MAX_VERTICAL_SPEED = 3000.f;
	const float MAX_HORIZONTAL_SPEED = 2000.f;

	const float LIFE_TIME = 1.5f;

private:
	std::vector<Piece> pieces_;

	float timeCounter_;
};

