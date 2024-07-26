#pragma once
#include "../Module.h"

class CrystalCham : public Module {
private:
	std::vector<Actor*> CrystalList;

	bool mobs = false;

	UIColor color = UIColor(0, 255, 255, 50);
	UIColor lineColor = UIColor(0, 255, 255, 255);
public:
	bool CaCham = false;
	int renderMode = 0;
	CrystalCham();

	virtual void onRender(MinecraftUIRenderContext* ctx) override;
};