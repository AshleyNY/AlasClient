#pragma once
#include "../Module.h"

class Fly : public Module {
public:
	float Speed = 4.f;
	Fly();
	Vec2<float> FlyRot;
	bool Critic = false;
	float CritValue = 0;
	Vec3<float> MoveVec = (0, 0, 0);
	Vec2<float> motion;
	virtual void onNormalTick(Actor* actor) override;
};
