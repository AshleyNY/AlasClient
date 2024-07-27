#include "CrystalCham.h"

CrystalCham::CrystalCham() : Module("CrystalCham", "CrystalCham Enities.", Category::RENDER) {
	addEnumSetting("Mode", "Changes CrystalCham modes", { "3D","CrystalCham"}, &renderMode);
	addColorPicker("Color", "NULL", &color);
	addColorPicker("LineColor", "NULL", &lineColor);
	addBoolCheck("Mobs", "CrystalCham Mobs", &mobs);
	addBoolCheck("Cham", "T", &CaCham);
}



void CrystalCham::drawChams(Actor*p) {
	Vec3<float> eyePos = p->getEyePos();;
	float yawPtr = p->getMobBodyRotationComponent()->yBodyRot;
	if (yawPtr == 0)
		return;
	float rot = -yawPtr + 180.f;
	///static Color cum(0.95f, 0.95f, 0.95f, 0.23f); // too lazy to add hurttime rn, if you really need it lmk tho 
	//Tess::setColor(p.first->getHurtTime() > 0 ? chamsHurtColor : chamsNormalColor);
	//RenderUtils::setColor(cum);
	//bool isSneaking = p.second.models[5]->rpos.y < 12.f;
	// Draw head
	std::vector<Vec3<float>> points = RenderUtils::getBoxCorners3D(eyePos.add(-0.28f, -0.225, -0.28f), eyePos.add(0.28f, 0.3f, 0.28f));
	RenderUtils::rotateBoxCorners3D(points, eyePos.add(0.f, -0.205f, 0.f), Vec3(-p->getActorHeadRotationComponent()->headRot, -p->getActorHeadRotationComponent()->headRot, p->getActorHeadRotationComponent()->headRot));
	//if (isSneaking) RenderUtils::translateBox3D(points, Vec3(0.f, -0.225f, -0.061f));
	RenderUtils::rotateBoxCorners3D(points, eyePos, Vec3(0.f, rot, 0.f));
	RenderUtils::drawRawBox3D(points,false,false);
	// Draw body
	points = RenderUtils::getBoxCorners3D(eyePos.add(-0.25f, -0.92f, -0.125f), eyePos.add(0.25f, -0.205f, 0.125f));
	RenderUtils::rotateBoxCorners3D(points, eyePos.add(0.f, -0.92f, 0.f), Vec3(-p->getMobBodyRotationComponent()->yBodyRot, -p->getMobBodyRotationComponent()->yBodyRot, p->getMobBodyRotationComponent()->yBodyRot));
	//if (isSneaking) RenderUtils::translateBox3D(points, Vec3(0.f, -0.15795f, -0.03705f));
	RenderUtils::rotateBoxCorners3D(points, eyePos, Vec3(0.f, rot, 0.f));
	RenderUtils::drawRawBox3D(points,false,false);
	// Draw all

	// Draw left leg 
	points = RenderUtils::getBoxCorners3D(eyePos.add(-0.24f, -1.67f, -0.136f), eyePos.add(0.f, -0.92f, 0.136f));
	RenderUtils::rotateBoxCorners3D(points, eyePos.add(0.f, -0.92f, 0.f), Vec3(-p->getMobBodyRotationComponent()->yBodyRot, -p->getMobBodyRotationComponent()->yBodyRot, p->getMobBodyRotationComponent()->yBodyRot));
	//if (isSneaking) RenderUtils::translateBox3D(points, Vec3(0.f, 0.f, 0.2f));
	RenderUtils::rotateBoxCorners3D(points, eyePos, Vec3(0.f, rot, 0.f));
	RenderUtils::drawRawBox3D(points, false, false);

	// Draw right leg 
	points = RenderUtils::getBoxCorners3D(eyePos.add(0.f, -1.67f, -0.136f), eyePos.add(0.24f, -0.92f, 0.136f));
	RenderUtils::rotateBoxCorners3D(points, eyePos.add(0.f, -0.92f, 0.f), Vec3(-p->getMobBodyRotationComponent()->yBodyRot, -p->getMobBodyRotationComponent()->yBodyRot, p->getMobBodyRotationComponent()->yBodyRot));
	//if (isSneaking) RenderUtils::translateBox3D(points, Vec3(0.f, 0.f, 0.2f));
	RenderUtils::rotateBoxCorners3D(points, eyePos, Vec3(0.f, rot, 0.f));
	RenderUtils::drawRawBox3D(points, false, false);
	// Draw body 
	points = RenderUtils::getBoxCorners3D(eyePos.add(-0.25f, -0.92f, -0.125f), eyePos.add(0.25f, -0.205f, 0.125f));
	RenderUtils::rotateBoxCorners3D(points, eyePos.add(0.f, -0.92f, 0.f), Vec3(-p->getMobBodyRotationComponent()->yBodyRot, -p->getMobBodyRotationComponent()->yBodyRot, p->getMobBodyRotationComponent()->yBodyRot));
	//if (isSneaking) RenderUtils::translateBox3D(points, Vec3(0.f, -0.10f, 0.25f));
	RenderUtils::rotateBoxCorners3D(points, eyePos, Vec3(0.f, rot, 0.f));
	RenderUtils::drawRawBox3D(points, false, false);
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
			//mc.DisplayClientMessage("xyz = %f,%f,%f", x1, y1, z1);
			x1 += 0.01f;
			if (x1 >= 1) {
				x1 = -1.f;
			}
			Vec3<float>  head = *ent->getPosition();
			AABB mobAABB = *ent->getAABB();
			//AABB aabb = mobAABB.intersects();
			RenderUtils::drawBox(mobAABB, color, lineColor, .3f, true, false);
			//RenderUtils::drawMoveBox(head);
			//drawChams(ent);
			//RenderUtils::drawMoveBox(head,x1,y1,z1,color, lineColor, .3f, true, false);
			break;
		}
		}
	}
}

void CrystalCham::onLevelRender()
{
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
		case 1: {
			drawChams(ent);
		}
		}
	}
}
