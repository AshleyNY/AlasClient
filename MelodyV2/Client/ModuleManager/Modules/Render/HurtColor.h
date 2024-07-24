#pragma once
#include "../Module.h"

class HurtColor : public Module {
public:
	bool fade = true;
	UIColor hurtColor = (255, 0, 0, 175);
	HurtColor();
};