#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class PopCounter : public Module {
public:
	bool sendchat = false;
	bool notifi = false;
	bool cys;
	float Dura = 2.f;
	int popcount = 0;
	int Health = 20;
	bool totem = true;
	std::string names;
	virtual void onNormalTick(Actor* actor) override;
	PopCounter();
	virtual std::string getModName() override;
};