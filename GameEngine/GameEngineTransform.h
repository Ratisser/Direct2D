#pragma once

#include <GameEngineBase/GameEngineMath.h>

class TransformData
{
public:
	float4x4 Wolrd;
	float4x4 View;
	float4x4 Projection;
};

class GameEngineTransform
{
public:
	GameEngineTransform(); 
	~GameEngineTransform();

	GameEngineTransform(const GameEngineTransform& _other) = delete; 
	GameEngineTransform(GameEngineTransform&& _other) = delete; 

	GameEngineTransform& operator=(const GameEngineTransform& _other) = delete;
	GameEngineTransform& operator=(const GameEngineTransform&& _other) = delete;

	void SetScale(const float4& _scale);
	void SetScale(float _scale);
	void SetScale(float _x, float _y, float _z = 1.0f);
	void MultiplyScale(float _scale);
	void MultiplyScale(float _x, float _y, float _z = 1.0f);

	void SetRotation(const float4& _rotation);
	void SetRotation(float _x, float _y, float _z = 0.0f);
	void AddRotation(float _x, float _y, float _z = 0.0f);

	void SetLocation(const float4& _location);
	void SetLocation(float _x, float _y, float _z = 0.0f);
	void AddLocation(float _x, float _y, float _z = 0.0f);

	float4 GetScale();
	float4 GetRotation();
	float4 GetLocation();
	float4 GetWorldLocation();

	void UpdateTransform();
	void UpdateTransformByParent();
	TransformData& GetTransformData();

	void SetParent(GameEngineTransform* _parent);
	void UnsetParent();
	void AddChild(GameEngineTransform* _child);
	void RemoveChild(GameEngineTransform* _child);

protected:
	TransformData transformData_;

	float4 scale_;
	float4 rotation_;
	float4 location_;

	GameEngineTransform* parent_;
	std::list<GameEngineTransform*> childs_;
};

