#pragma once

#pragma warning(disable:26812)

#include "../ThirdParty/FMOD/inc/fmod.hpp"

#pragma comment(lib, "../ThirdParty/FMOD/lib/x64/fmod_vc.lib")

#include <unordered_map>

class GameEngineSoundPlayer;
class GameEngineSound
{
	friend GameEngineSoundPlayer;
public:
	~GameEngineSound();

	GameEngineSound(const GameEngineSound& _other) = delete;
	GameEngineSound(GameEngineSound&& _other) = delete;

	GameEngineSound& operator=(const GameEngineSound& _other) = delete;
	GameEngineSound& operator=(const GameEngineSound&& _other) = delete;

	static GameEngineSound& GetInst() { return *instance_; }
	static void Destroy();

	void Initialize();
	void Update();

	void CreateSound(const std::string& _name, const std::string& _path, bool _bLoop = false);
	
	void PlaySoundByName(const std::string& _name);
	void SetVolume(unsigned int _volume);
	void StopSound();

private:
	GameEngineSound();

	FMOD::Sound* getSound(const std::string& _name);

private:
	static GameEngineSound* instance_;

private:
	FMOD::System* system_;
	FMOD::Channel* channel_;

	std::unordered_map<std::string, FMOD::Sound*> allSounds_;
};

