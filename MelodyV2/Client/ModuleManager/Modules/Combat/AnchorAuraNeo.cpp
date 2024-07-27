
#include "AnchorAuraNeo.h"
AnchorAuraNeo::AnchorAuraNeo() : Module("AnchorAuraNeo", "Automatically places Anchor above players to destroy helmets or get them stuck", Category::COMBAT) {
	addBoolCheck("airplace", "NULL", &airplace);
	addBoolCheck("geyser", "NULL", &geyser);
	addSlider<int>("Range", "NULL", ValueType::INT_T, &range, 3, 10);
}

std::string AnchorAuraNeo::getModName() {
	return names;
}

void AnchorAuraNeo::onRender(MinecraftUIRenderContext* renderCtx)
{
}



static std::vector<Actor*> AnchorList;

static int Swap(int n) {
	PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	int slot = plrInv->selectedSlot;
	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) {
			if (itemStack->getItemPtr()->itemId == n) {
				return i;
			}
		}
	}
	return slot;
}

static int getBlockID(Vec3<float> pos) {
	int id = 0;
	Block* block = mc.getLocalPlayer()->dimension->blockSource->getBlock(pos.toInt());
	id = block->blockLegacy->blockId;
	return id;
}

static void predictBlock(Vec3<float> pos) {
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
static void tryPlace(Vec3<int> tryBuildPos, int itemslot, int topid) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	GameMode* gm = localPlayer->getGameMode();
	PlayerInventory* plrInv = localPlayer->getPlayerInventory();
	Inventory* inv = plrInv->inventory;

	Vec3<float> playerPos = *localPlayer->getPosition();
	playerPos.y -= 1.f;
	playerPos = playerPos.floor();

	Block* block = localPlayer->dimension->blockSource->getBlock(tryBuildPos);
	if (block->blockLegacy->blockId == topid) {

		int bestSlot = Swap(itemslot);
		int oldSlot = plrInv->selectedSlot;
		bool shouldSwitch = (bestSlot != plrInv->selectedSlot);
		if (shouldSwitch) {
			plrInv->selectedSlot = bestSlot;
			MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
			mc.getClientInstance()->loopbackPacketSender->send(&pk);
		}

		predictBlock(tryBuildPos.toFloat());

		if (shouldSwitch) {
			plrInv->selectedSlot = oldSlot;
		}
	}
}
void AnchorAuraNeo::onNormalTick(Actor* actor) {
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
	AnchorList.clear();

	for (Actor* actor : level->getRuntimeActorList()) {
		if (TargetUtils::isTargetValid(actor, airplace)) {
			float seenPercent = region->getSeenPercent(localPlayer->getEyePos(), *actor->getAABB());
			float dist = actor->getPosition()->dist(*localPlayer->getPosition());
			float rangeCheck = (seenPercent > 0.f) ? 7 : 7;
			if (dist < rangeCheck) AnchorList.push_back(actor);
		}
	}//я╟урд©╠Й

	if (!AnchorList.empty()) {

		Vec3<float> enemyLoc = *AnchorList[0]->getPosition();
		auto rotationToPlacement = mc.getLocalPlayer()->stateVectorComponent->pos.CalcAngle(enemyLoc.toFloat());
		rotAnglePlace = rotationToPlacement;

		if (geyser) {
			placepos = enemyLoc.add(-1, 1, 0);
		}
		else {
			placepos = enemyLoc.add(0, 1, 0);
		}
		if (getBlockID(placepos) == 0) {
			tryPlace(placepos.toInt(), 65264, 0);
			start = true;
		}
		if (start) {
			tryPlace(placepos.toInt(), 89, 66063);
			start = false;
		}
		if (!start) {
			if (airplace) {
				tryPlace(placepos.toInt(), 260, 66063);
			}
			else {
				gm->buildBlock(placepos.toInt(), 233, true);
			}
		}
	}








}
void AnchorAuraNeo::onDisable()
{
	AnchorList.clear();
}
void AnchorAuraNeo::onSendPacket(Packet* packet, bool& shouldCancel)
{
	if (packet->getId() == PacketID::PlayerAuthInput) {
		auto* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		authPacket->rotation = rotAnglePlace;
		authPacket->headYaw = rotAnglePlace.y;
	}
}

void AnchorAuraNeo::onEnable()
{
}
