#pragma once
#include "../Module.h"

class CrystalCham : public Module {
private:
	std::vector<Actor*> CrystalList;

	bool mobs = false;

	UIColor color = UIColor(0, 255, 255, 50);
	UIColor lineColor = UIColor(0, 255, 255, 255);
public:
	float x1 = 0;
	float y1 = 0;
	float z1 = 1;
	bool CaCham = false;
	int renderMode = 0;
	CrystalCham();
	virtual void drawChams(Actor* p);
	Vec3<float> sb = (0,0,0);
	virtual void onRender(MinecraftUIRenderContext* ctx) override;
	virtual void onLevelRender()override;
};