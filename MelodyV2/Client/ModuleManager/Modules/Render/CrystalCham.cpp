#include "CrystalCham.h"

CrystalCham::CrystalCham() : Module("CrystalCham", "CrystalCham Enities.", Category::RENDER) {
	addEnumSetting("Mode", "Changes CrystalCham modes", { "3D","CrystalCham"}, &renderMode);
	addColorPicker("Color", "NULL", &color);
	addColorPicker("LineColor", "NULL", &lineColor);
	addBoolCheck("Mobs", "CrystalCham Mobs", &mobs);
	addBoolCheck("Cham", "T", &CaCham);
}

void CrystalCham::onRender(MinecraftUIRenderContext* ctx) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();

	if (localPlayer == nullptr) return;
	if (!mc.canUseMoveKeys()) return;
	Level* level = localPlayer->getLevel();
	if (level == nullptr) return;
	CrystalList.clear();

	for (Actor* ent : level->getRuntimeActorList()) {
		if (ent->getEntityTypeId() != 71)
			continue;
		bool isValid = TargetUtils::isTargetValid(ent, true);
		if (isValid) CrystalList.push_back(ent);
	}

	for (Actor* ent : CrystalList) {
		switch (renderMode) {
		case 0: {
			AABB mobAABB = *ent->getAABB();
			RenderUtils::drawBox(mobAABB, color, lineColor, .3f, true, false);
			;			
			break;
		}
		case 1:{
			Vec3<float> head = *localPlayer->getPosition();
			RenderUtils::drawBox(head, color, lineColor, .3f, true, false);
			  }
		}
	}
}
