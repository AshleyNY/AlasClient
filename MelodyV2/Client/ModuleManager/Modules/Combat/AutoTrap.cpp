#include "AutoTrap.h"

#include "../../ModuleManager.h"
#include "../../../../Utils/TargetUtils.h"
#include "../../../Client.h"

AutoTrap::AutoTrap() : Module("AutoTrap", "Null.", Category::COMBAT) {
	addEnumSetting("Mode", "NULL", { "Default", "Big" }, &mode);
	addSlider<float>("Range", "Null", ValueType::FLOAT_T, &range, 3.f, 10.f);
	addSlider<int>("Delay", "NULL", ValueType::INT_T, &Tdelay, 0, 20);
	addBoolCheck("OnClick", "NULL", &onClick);
	addBoolCheck("FullBody", "NULL", &fullbody);
	addBoolCheck("Switch", "NULL", &switch2obby);
	addBoolCheck("AirPlace", "NULL", &airplace);
}
static int getObi() {
	PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	int slot = plrInv->selectedSlot;
	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) {
			if (itemStack->getItemPtr()->itemId == 49) {
				return i;
			}
		}
	}
	return slot;
}//

static std::vector<Actor*> TrapList;

static void PredictBlok(Vec3<float> pos) {
	GameMode* gm = mc.getGameMode();
	static std::vector<Vec3<float>> blocks;

	if (blocks.empty()) {
		for (int y = -2; y <= 2; y++) {
			for (int x = -5; x <= 5; x++) {
				for (int z = -5; z <= 5; z++) {
					blocks.emplace_back(Vec3<float>(x, y, z));
				}
			}
		}

		std::ranges::sort(blocks, {}, &Math::calculateDistance);
	}

	auto tryPlaceBlock = [&](const Vec3<float>& offset) {
		Vec3<float> currentBlock = (Vec3<float>(pos.floor())).add(offset);
		if (gm->tryPlaceBlock(currentBlock.toInt())) {
			return true;
		}
		return false;
		};

	for (const Vec3<float>& offset : blocks) {
		if (tryPlaceBlock(offset)) {
			return;
		}
	}
}

static void tryObsidian(Vec3<int> tryBuildPos) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	GameMode* gm = localPlayer->getGameMode();
	PlayerInventory* plrInv = localPlayer->getPlayerInventory();
	Inventory* inv = plrInv->inventory;

	Vec3<float> playerPos = *localPlayer->getPosition();
	playerPos.y -= 1.f;
	playerPos = playerPos.floor();

	Block* block = localPlayer->dimension->blockSource->getBlock(tryBuildPos);
	if (block->blockLegacy->blockId == 0) {

		int bestSlot = getObi();
		int oldSlot = plrInv->selectedSlot;
		bool shouldSwitch = (bestSlot != plrInv->selectedSlot);
		if (shouldSwitch) {
			plrInv->selectedSlot = bestSlot;
			MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
			mc.getClientInstance()->loopbackPacketSender->send(&pk);
		}

		PredictBlok(tryBuildPos.toFloat());
		//gm->buildBlock(tryBuildPos, 0, 0);
		//gm->buildBlock(tryBuildPos, 0, 0);

		if (shouldSwitch) {
			plrInv->selectedSlot = oldSlot;
		}
	}
}

void AutoTrap::onNormalTick(Actor* actor) {

	bool mustGoUp = false;
	placements.clear();

	if (mc.getLocalPlayer() == nullptr)
		return;
	if (!mc.canUseMoveKeys())
		return;
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	PlayerInventory* plrInv = localPlayer->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	GameMode* gm = mc.getGameMode();
	BlockSource* region = localPlayer->dimension->blockSource;
	Level* level = localPlayer->getLevel();
	TrapList.clear();

	int place = 1;
	for (Actor* actor : level->getRuntimeActorList()) {
		if (TargetUtils::isTargetValid(actor, false)) {
			float seenPercent = region->getSeenPercent(localPlayer->getEyePos(), *actor->getAABB());
			float dist = actor->getPosition()->dist(*localPlayer->getPosition());
			float rangeCheck = (seenPercent > 0.f) ? 7 : 7;
			if (dist < rangeCheck) TrapList.push_back(actor);
		}
	}
	Tdelay++;
	if (Tdelay > pDelay) {
		if (onClick) {
			if (mc.isRightClickDown()) {
				place = 0;
			}
			else {
				place = 1;
			}
		}

		if (!onClick) {
			place = 0;
		}

		if (!TrapList.empty() && mode == 0) {
			// ground level
			Vec3<float> enemLoc = TrapList[0]->getEyePos().floor();
			auto rotationToPlacement = mc.getLocalPlayer()->stateVectorComponent->pos.CalcAngle(enemLoc.toFloat());
			rotAnglePlace = rotationToPlacement;
			if (!fullbody) {
				/* GROUND   FEET     HEAD
			   0 0 N	0 0 N    N N N    +x
			   0 0 0    0 0 0    N 0 N  -z   +z
			   0 0 0	0 0 0    0 N N	  -x
				*/
				placements.clear();
				placements.push_back(enemLoc.add(1, -1, 0));
				placements.push_back(enemLoc.add(-1, -1, 0));
				placements.push_back(enemLoc.add(0, -1, 1));
				placements.push_back(enemLoc.add(0, -1, -1));

				placements.push_back(enemLoc.add(1, 1, 0));
				placements.push_back(enemLoc.add(-1, 1, 0));
				placements.push_back(enemLoc.add(0, 1, 1));
				placements.push_back(enemLoc.add(0, 1, -1));

				placements.push_back(enemLoc.add(1, 2, 0));
				placements.push_back(enemLoc.add(0, 2, 0));
			}
			else {
				/*
				GROUND   FEET     HEAD
				0 N 0	 0 N 0	  0 N 0     +x
				N 0 N	 N 0 N	  N N N	  -z   +z
				0 N 0 	 0 N 0	  0 N 0	    -x
				*/
				placements.clear();

				placements.push_back(enemLoc.add(1, -1, 0));
				placements.push_back(enemLoc.add(-1, -1, 0));
				placements.push_back(enemLoc.add(0, -1, 1));
				placements.push_back(enemLoc.add(0, -1, -1));

				placements.push_back(enemLoc.add(1, 0, 0));
				placements.push_back(enemLoc.add(-1, 0, 0));
				placements.push_back(enemLoc.add(0, 0, 1));
				placements.push_back(enemLoc.add(0, 0, -1));

				placements.push_back(enemLoc.add(1, 1, 0));
				placements.push_back(enemLoc.add(-1, 1, 0));
				placements.push_back(enemLoc.add(0, 1, 1));
				placements.push_back(enemLoc.add(0, 1, -1));

				placements.push_back(enemLoc.add(1, 2, 0));
				placements.push_back(enemLoc.add(0, 2, 0));
			}

			if (place == 0) {

				for (Vec3<float> i : placements) {
					if (airplace)
						tryObsidian(i.toInt());
					else
						gm->buildBlock(Vec3<float>(i).toInt(),233,true);
				}
			}
		}
		placements.clear();
		Tdelay = 0;
	}
}

void AutoTrap::onSendPacket(Packet* packet, bool& shouldCancel)
{
	if (packet->getId() == PacketID::PlayerAuthInput) {
		auto* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		authPacket->rotation = rotAnglePlace;
		authPacket->headYaw = rotAnglePlace.y;
	}
}

void AutoTrap::onRender(MinecraftUIRenderContext* renderCtx)
{
}
