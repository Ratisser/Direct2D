#pragma once

#include "MonsterBase.h"

class TutorialTarget : public MonsterBase
{
public:
	TutorialTarget(); 
	~TutorialTarget();

	TutorialTarget(const TutorialTarget& _other) = delete; 
	TutorialTarget(TutorialTarget&& _other) = delete; 

	TutorialTarget& operator=(const TutorialTarget& _other) = delete;
	TutorialTarget& operator=(const TutorialTarget&& _other) = delete;

public:
	// MonsterBase을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update(float _deltaTime);

	virtual void OnHit();

private:
	GameEngineImageRenderer* renderer_;
	GameEngineImageRenderer* bodyRenderer_;
};

