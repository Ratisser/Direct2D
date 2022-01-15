#pragma once

class GameEngineTexture
{
public:
	GameEngineTexture(); 
	~GameEngineTexture();

	GameEngineTexture(const GameEngineTexture& _other) = delete; 
	GameEngineTexture(GameEngineTexture&& _other) = delete; 

	GameEngineTexture& operator=(const GameEngineTexture& _other) = delete;
	GameEngineTexture& operator=(const GameEngineTexture&& _other) = delete;

};

