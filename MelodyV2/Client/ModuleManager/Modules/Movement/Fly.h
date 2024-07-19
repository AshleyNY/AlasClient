#pragma once
#include "../Module.h"

class Fly : public Module {
public:
	float Speed = 4.f;
	Fly();

	virtual void onNormalTick(Actor* actor) override;
};
