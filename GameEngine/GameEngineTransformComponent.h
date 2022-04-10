#pragma once

#include "GameEngineComponent.h"

class TransformData
{
public:
	float4x4 Wolrd;
	float4x4 View;
	float4x4 Projection;
};

class GameEngineTransformComponent : public GameEngineComponent
{
public:
	GameEngineTransformComponent(); 
	virtual ~GameEngineTransformComponent();

	GameEngineTransformComponent(const GameEngineTransformComponent& _other) = delete; 
	GameEngineTransformComponent(GameEngineTransformComponent&& _other) = delete; 

	GameEngineTransformComponent& operator=(const GameEngineTransformComponent& _other) = delete;
	GameEngineTransformComponent& operator=(const GameEngineTransformComponent&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	void SetScale(const float4& _scale);
	void SetScale(float _scale);
	void SetScale(float _x, float _y, float _z = 1.0f);
	void AddScale(float _scale);
	void AddScale(float _x, float _y, float _z);
	void MultiplyScale(float _scale);
	void MultiplyScale(float _x, float _y, float _z = 1.0f);

	void SetRotation(const float4& _rotation);
	void SetRotation(float _x, float _y, float _z = 0.0f);
	void AddRotation(float _x, float _y, float _z = 0.0f);

	void SetLocation(const float4& _location);
	void SetLocation(float _x, float _y, float _z = 0.0f);
	void AddLocation(float _x, float _y, float _z = 0.0f);
	void AddLocation(const float4& _location);

	inline void SetLocationX(float _x) { location_.x = _x; }
	inline void SetLocationY(float _y) { location_.y = _y; }
	inline void SetLocationZ(float _z) { location_.z = _z; }


	float4 GetScale() const;
	float4 GetRotation() const;
	float4 GetLocation() const;
	float4 GetWorldLocation() const;
	float4 GetWorldScale() const;

	float4 GetForward() const;
	float4 GetUp() const;
	float4 GetRight() const;

	void UpdateTransform();
	void UpdateTransformByParent();
	void ReleaseReady(bool _bReleaseByActor = true);
	TransformData& GetTransformData();

	void SetParent(GameEngineTransformComponent* _parent);
	void UnsetParent();
	void AddChild(GameEngineTransformComponent* _child);
	void RemoveChild(GameEngineTransformComponent* _child);

	std::list<GameEngineTransformComponent*>& GetChildren();

protected:
	TransformData transformData_;

	float4 scale_;
	float4 rotation_;
	float4 location_;

	GameEngineTransformComponent* parent_;
	std::list<GameEngineTransformComponent*> children_;
};
