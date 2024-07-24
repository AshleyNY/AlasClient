#pragma once
#include "../Module.h"

class ConicalHat : public Module {
private:
	UIColor chColor = UIColor(255, 255, 255, 255);
	bool rainbow = true;
public:
	ConicalHat();
	virtual void onLevelRender() override;
};