#pragma once

#include "GameEngineSoundManager.h"

class GameEngineSoundPlayer
{
public:
	GameEngineSoundPlayer(const std::string& _soundName);
	~GameEngineSoundPlayer();

	GameEngineSoundPlayer(const GameEngineSoundPlayer& _other) = delete;
	GameEngineSoundPlayer(GameEngineSoundPlayer&& _other) = delete;

	GameEngineSoundPlayer& operator=(const GameEngineSoundPlayer& _other) = delete;
	GameEngineSoundPlayer& operator=(const GameEngineSoundPlayer&& _other) = delete;

	void ChangeSound(const std::string& _soundName);
	void Play();
	void Stop();

	bool IsPlaying();
	bool IsPaused();
	void SetPaused(bool _bPause);
	void SetVolume(float _volume);
	void SetPitch(float _pitch);
	void SetPosition(unsigned int _positionMilliSec);
	unsigned int GetPositionMillisecond();
	unsigned int GetLengthMillisecond();

private:
	FMOD::Sound* sound_;
	FMOD::Channel* channel_;
};

