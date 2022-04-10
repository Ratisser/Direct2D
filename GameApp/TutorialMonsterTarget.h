#pragma once

#include "MonsterBase.h"

class TutorialMonsterTarget : public MonsterBase
{
public:
	TutorialMonsterTarget(); 
	~TutorialMonsterTarget();

	TutorialMonsterTarget(const TutorialMonsterTarget& _other) = delete; 
	TutorialMonsterTarget(TutorialMonsterTarget&& _other) = delete; 

	TutorialMonsterTarget& operator=(const TutorialMonsterTarget& _other) = delete;
	TutorialMonsterTarget& operator=(const TutorialMonsterTarget&& _other) = delete;

};

