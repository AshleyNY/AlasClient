#pragma once
#include "../Module.h"
class AutoTrap : public Module {
private:
	std::string targetname = "None";
	bool tryAutoTrap(Vec3<float> AutoTrap);
	bool ATRots = true;
	bool fullbody = false;
	bool niggerTurnCAOn = false;
	bool switch2obby = true;
	bool airplace = false;
	int slotOB2 = 0;
	int Tdelay = 0;
	int pDelay = 0;
	int mode = 0;
	float attackHealth = 12.f;
	int attack = 0;
	/* GROUND   FEET     HEAD
	   0 0 N	0 0 N    N N N
	   0 0 0    0 0 0    N 0 N
	   0 0 0	0 0 0    0 N N
	*/

	std::vector<Vec3<float>> placements;
	Vec2<float> rotAnglePlace{};
public:
	const char* name = "AutoTrap";
	bool mustGoUp = false;

	AutoTrap();
	~AutoTrap();

	float range = 7;
	bool onClick = false;
	// Inherited via IModule
	virtual void onNormalTick(Actor* actor) override;
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
	virtual void onRender(MinecraftUIRenderContext* renderCtx) override;
};
