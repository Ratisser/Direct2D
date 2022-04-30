#pragma once

#include <GameEngine\GameEngineActor.h>

class Demon : public GameEngineActor
{
public:
	Demon(); 
	~Demon();
	Demon(const Demon& _other) = delete; 
	Demon(Demon&& _other) = delete; 
	Demon& operator=(const Demon& _other) = delete;
	Demon& operator=(const Demon&& _other) = delete;

public:

};

