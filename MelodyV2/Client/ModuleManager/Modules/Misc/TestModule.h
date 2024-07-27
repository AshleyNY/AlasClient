#pragma once
#include "../Module.h"

class TestModule : public Module {
private:
	float value = 0.f;
	float value1 = 0.f;

	int Ivalue = 0;
	int Ivalue1 = 0;

	bool bool1 = false;
	bool bool2 = false;

	int mode1 = 0;
	int mode2 = 0;

	Vec3<float> pos;
	Vec3<float> pos1;

	Vec2<float> rotAngle1;
	Vec2<float> rotAngle2;

	UIColor color1 = UIColor(255, 0, 0, 255);
	UIColor color2 = UIColor(0, 255, 0, 255);


public:
	UIColor color3 = UIColor(0, 0, 255, 255);
	float value2 = 0.f;
	int Ivalue2 = 0;
	bool bool3 = false;
	std::vector<Actor*> targetList;
	Vec2<float> rotAngle;
	Vec3<float> pos2;
	TestModule();
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
	virtual void onLevelRender() override;
	virtual void onImGuiRender(ImDrawList* d) override;
	virtual void onNormalTick(Actor* actor) override;
	virtual void onRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onDisable() override;
	virtual void onEnable() override;
};
