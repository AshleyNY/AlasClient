#pragma once
#include "../Module.h"

class Killaura : public Module {
private:


	int Mode = 0;

	int switchMode = 0;
	bool isInRange = false;
	bool hurttime = false;
	int attackDelay = 2;
	int attackDelayTick = 0;
	bool visualRange = true;
	bool visualRange1 = true;
	bool visualRange2 = true;
	UIColor vRColor = UIColor(255, 255, 255, 255);
	UIColor vRColor1 = UIColor(255, 255, 255, 255);
	UIColor vRColor2 = UIColor(255, 255, 255, 255);
	bool targetVisualize = true;
	UIColor visualizeColor = UIColor(255, 255, 255, 255);

	int getBestWeaponSlot();
public:
	float TargetDis = 0;
	float ARange = 7.f;
	float MaxSrange = 20.f;
	float MinSrange = 3.f;
	float targetRange = 20.f;
	int package = 1;
	bool attackMob = false;
	bool keyAddPack = false;
	std::vector<Actor*> targetList;
	Vec2<float> rotAngle;
	Killaura();
	int rotMode = 1;
	virtual void onNormalTick(Actor* actor) override;
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
	virtual void onImGuiRender(ImDrawList* drawlist) override;
};