#pragma once

#include <memory>
#include <DirectXMath.h>

class Particle
{
public:
	Particle();

	// Setters
	void SetMass(float mass);
	void AddForce(const DirectX::XMFLOAT3& force);
	void ResetForce();
	void ResetAccumulatedVelocity();
	void SetPosition(const DirectX::XMFLOAT3& position);
	void SetVelocity(const DirectX::XMFLOAT3& velocity);
	void SetAcceleration(const DirectX::XMFLOAT3& acceleration);

	// Getters
	float GetMass() const;
	float GetInverseMass() const;
	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMFLOAT3 GetVelocity() const;
	DirectX::XMFLOAT3 GetAcceleration() const;
	DirectX::XMFLOAT3 GetAccumulatedForce() const;

public:
	DirectX::XMFLOAT3 Position{ 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 Velocity{ 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 Acceleration{ 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 AccumulatedForce{ 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 AccumulatedVelocity{ 0.0f, 0.0f, 0.0f };

	float mMass{ 1.0f };
	float mInverseMass{ 1.0f };
};
