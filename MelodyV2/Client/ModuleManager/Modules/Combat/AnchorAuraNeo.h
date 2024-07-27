#pragma once
#include "../../../../Utils/TargetUtils.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class AnchorAuraNeo : public Module {
private:
	bool airplace = false;
	bool geyser = false;
	std::string names;
	int range = 7;

	//ground level  ~~~ funni initializations
	bool start = false;
	Vec3<float> top;
	Vec3<float> placepos;
	Vec2<float> rotAnglePlace{};
public:
	AnchorAuraNeo();
	~AnchorAuraNeo();
	// Inherited via IModule
	//void charge2(C_GameMode* gm, Vec3<float>* pos);
	virtual void onNormalTick(Actor* actor) override;
	virtual std::string getModName() override;
	virtual void onRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};