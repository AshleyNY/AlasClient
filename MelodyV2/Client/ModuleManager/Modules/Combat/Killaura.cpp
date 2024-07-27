#include "Killaura.h"
#include <cmath> // for std::atan2()
Killaura::Killaura() : Module("Killaura", "Auto attack players / mobs arround u.", Category::COMBAT) {
	addSlider<float>("Target Range", "Players/Mobs have range lower than this will be targeted", ValueType::FLOAT_T, &targetRange, 3.f, 20.f);
	addSlider<float>("Attack Range", "Players/Mobs have range lower than this will be attacked", ValueType::FLOAT_T, &ARange, 0.f, 10.f);
	addSlider<float>("MaxSRange", "MAX Strafe Range", ValueType::FLOAT_T, &MaxSrange, 0.f, 20.f);
	addSlider<float>("MinSRange", "Min Strafe Range", ValueType::FLOAT_T, &MinSrange, 0.f, 10.f);
	addEnumSetting("Mode", "NULL", { "Single", "Multi" }, &Mode);
	addEnumSetting("Rotation", "NULL", { "None", "Silent" ,"Strafe"}, &rotMode);
	addEnumSetting("Switch", "NULL", { "None", "Weather", "WeatherSilent" }, &switchMode);
	addBoolCheck("Attack Mob", "If u want attack mob or not", &attackMob);
	addBoolCheck("Hurttime check", "NULL", &hurttime);
	addBoolCheck("KeyContrl", "Up or Down to change package,left or right to change weapon", &keyAddPack);
	addSlider<int>("Attack delay", "NULL", ValueType::INT_T, &attackDelay, 0, 20);
	addSlider<int>("Package", "NULL", ValueType::INT_T, &package, 1, 10);
	addBoolCheck("Visual TRange", "TargetRange", &visualRange);
	addColorPicker("VA Color", "NULL", &vRColor);
	addBoolCheck("Visual ARange", "AttackRange", &visualRange1);
	addColorPicker("VT Color", "NULL", &vRColor1);
	addBoolCheck("Visual SRange", "SrafeRange", &visualRange2);
	addColorPicker("VS Color", "NULL", &vRColor2);
	addBoolCheck("Target Visualize", "NULL", &targetVisualize);
	addColorPicker("TV Color", "NULL", &visualizeColor);
}

/*
			/*
			* here some step that you can get pick and things
			* item->getItemPtr()->hasRecipeTag("minecraft:digger") <- this is how you get like a things that can digs like shovel and picks and axe maybe
			* if (item->hasRecipeTag("minecraft:is_sword"))
			* item->hasRecipeTag("minecraft:is_pickaxe") <- get pickaxe
			* item->hasRecipeTag("minecraft:is_axe") <- get axe
			* item->hasRecipeTag("minecraft:is_shovel") <- get some shovel
			* item->hasRecipeTag("minecraft:is_hoe")<- get hoe like a riel bitch
			* item->hasRecipeTag("minecraft:is_food") <- get food
			*/
int Killaura::getBestWeaponSlot() {
	auto localPlayer = mc.getLocalPlayer();
	//float health = mc.getLocalPlayer()->getHealth();
	bool isInWater = mc.getLocalPlayer()->isInWater();
	if (!localPlayer)
		return -1;

	auto plrInv = localPlayer->getPlayerInventory();
	if (!plrInv)
		return -1;

	auto inv = plrInv->inventory;
	if (!inv)
		return -1;

	float damage = 0.f;
	int slot = plrInv->selectedSlot;

	for (int i = 0; i < 9; i++) {
		auto itemStack = inv->getItemStack(i);
		if (itemStack && itemStack->isValid()) { // or you can do itemStack->getItemPtr()->hasRecipeTag("minecraft:is_sword") for sword find only 
			if (isInWater) {
				if (itemStack->getItemPtr()->itemId == 554) {
					slot = i;
				}
			}
			else {
				if (itemStack->getItemPtr()->itemId == 319) {
					slot = i;
				}
			}
		}
	}
	return slot;
}
# define M_PI 3.14159265358979323846 /* pi */
void GetSword() {
	auto supplies = mc.getLocalPlayer()->getPlayerInventory();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) {
			if (itemStack->getItemPtr()->itemId == 319) {
				supplies->selectedSlot = i;
				return;
			}
		}
	}
}
void GetTrident() {
	auto supplies = mc.getLocalPlayer()->getPlayerInventory();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) {
			if (itemStack->getItemPtr()->itemId == 554) {
				supplies->selectedSlot = i;
				return;
			}
		}
	}
}
static Vec2<float> GetRotations(Vec3<float> playerEyePos, Vec3<float> targetPos) {
	Vec3<float> delta = targetPos.sub(playerEyePos);
	float yaw = atan2(delta.z, delta.x) * 180.0f / M_PI;
	float pitch = atan2(delta.y, std::sqrt(delta.x * delta.x + delta.z * delta.z)) * 180.0f / M_PI;
	return { -pitch, yaw - 90 };
}

void Killaura::onNormalTick(Actor* actor) {
	auto localPlayer = mc.getLocalPlayer();
	auto plrInv = localPlayer->getPlayerInventory();
	auto inv = plrInv->inventory;
	auto gm = mc.getGameMode();
	auto region = localPlayer->dimension->blockSource;
	auto level = localPlayer->getLevel();
	if (!level) return;
	if (package > 100) {
		package = 0;
	}
	if (keyAddPack) {
		if (mc.isKeyDown(38)) {
			package++;
			mc.DisplayClientMessage("[%sAlas%s]:%sPackage = %s%d",DARK_PURPLE,WHITE,GREEN, RED,package);
		}
		else if (mc.isKeyDown(40)) {
			package--;
			mc.DisplayClientMessage("[%sAlas%s]:%sPackage = %s%d", DARK_PURPLE, WHITE, GREEN, RED, package);
		}
		if (mc.isKeyDown(37)) {
			mc.DisplayClientMessage("[%sAlas%s]:%sWeapon = %sSword", DARK_PURPLE, WHITE, GREEN, RED);
			GetSword();
		}
		if (mc.isKeyDown(39)) {
			mc.DisplayClientMessage("[%sAlas%s]:%sWeapon = %sTrident", DARK_PURPLE, WHITE, GREEN, RED);
			GetTrident();
		}
	}
	//bool sb = mc.getLocalPlayer().is
	targetList.clear();


	for (auto& target : level->getRuntimeActorList()) {
		if (TargetUtils::isTargetValid(target, attackMob)) {
			float dist = target->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos);
			float rangeCheck = targetRange;
			if (dist < rangeCheck)
				targetList.push_back(target);
		}
	}

	if (!targetList.empty()) {
		std::sort(targetList.begin(), targetList.end(), TargetUtils::sortByDist);
	    TargetDis = targetList[0]->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos);
		isInRange = true;
		//int health = targetList[0]->gethealth();
		if (TargetDis < MaxSrange) {
		
			rotAngle = GetRotations(localPlayer->stateVectorComponent->pos, targetList[0]->stateVectorComponent->pos.sub(Vec3<float>(0.f, 0.9f, 0.f)));
			//mc.DisplayClientMessage("KA = %f", rotAngle.y);
			if (rotMode == 2) {
				localPlayer->rotationComponent->rotation = rotAngle;
				localPlayer->rotationComponent->Set(rotAngle);
				localPlayer->rotationComponent->oldRotation = rotAngle;
			}
		}
		if (attackDelayTick >= attackDelay) {
			int bestSlot = getBestWeaponSlot();
			int oldSlot = plrInv->selectedSlot;
			bool shouldSwitch = (bestSlot != plrInv->selectedSlot);

			if (shouldSwitch && (switchMode == 1 || switchMode == 2)) {
				plrInv->selectedSlot = bestSlot;

				if (switchMode == 2) {
					/*auto packet = MinecraftPackets::createPacket(PacketID::MobEquipmentPacket);
					auto packetcasted = std::reinterpret_pointer_cast<MobEquipmentPacket>(packet);
					packetcasted->ActorRuntimeID = localPlayer->getRuntimeID();
					packetcasted->ContainerID = 0;
					packetcasted->ContainerID2 = 0;
					packetcasted->InventorySlot = bestSlot;
					packetcasted->InventorySlot2 = bestSlot;
					packetcasted->HotbarSlot = bestSlot;
					packetcasted->HotbarSlot2 = bestSlot;
					NetworkItemStackDescriptor itemstackdescriptor(*inv->getItemStack(bestSlot));
					packetcasted->ItemStackDescriptor = itemstackdescriptor;
					mc.getClientInstance()->loopbackPacketSender->send(packetcasted.get());*/ // <- this hehh tao lay tu nuvola tai vi no ngon hon
					MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
					mc.getLocalPlayer()->sendNetworkPacket(pk);
					mc.getClientInstance()->loopbackPacketSender->send(&pk);
				}
			}
			if (Mode = 1)
			{
				for (auto& target : targetList) {
					if (target->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos) < ARange) {
						if (!(hurttime && target->hurtTime > 0)) {
							InteractPacket inter(InteractAction::LEFT_CLICK, mc.getLocalPlayer()->getRuntimeID(), target->stateVectorComponent->pos.sub(Vec3<int>(0.f, 0.2f, 0.f)));

							mc.getClientInstance()->loopbackPacketSender->send(&inter);
							for (int i = 0; i < package; i++) {
								gm->attack(target);
								localPlayer->swing();
							}
						}
					}
				}
			}
			if (Mode = 0)
			{
				if (TargetDis < ARange) {
					if (!(hurttime && targetList[0]->hurtTime > 0)) {
						InteractPacket inter(InteractAction::LEFT_CLICK, mc.getLocalPlayer()->getRuntimeID(), targetList[0]->stateVectorComponent->pos.sub(Vec3<int>(0.f, 0.2f, 0.f)));

						mc.getClientInstance()->loopbackPacketSender->send(&inter);
						for (int i = 0; i < package; i++) {
							gm->attack(targetList[0]);
							localPlayer->swing();
						}
					}
				}
			}
			

			

			if (shouldSwitch && switchMode == 2) {
				plrInv->selectedSlot = oldSlot;
			}

			attackDelayTick = 0;

		}
		else {
			attackDelayTick++;
		}
	}
	else {
		isInRange = false;
		targetList.clear();
		TargetDis = 0;
	}
}

void Killaura::onEnable()
{
}

void Killaura::onDisable()
{
	targetList.clear();
}

void Killaura::onSendPacket(Packet* packet, bool& shouldCancel) {
	if (!targetList.empty() && rotMode == 1 && packet->getId() == PacketID::PlayerAuthInput) {
		PlayerAuthInputPacket* authPacket = (PlayerAuthInputPacket*)packet;
		authPacket->rotation = rotAngle;
		authPacket->headYaw = rotAngle.y;
	}

	if (!targetList.empty() && rotMode == 1 && packet->getId() == PacketID::MovePlayerPacket) {
		auto* movepacket = (MovePlayerPacket*)packet;
		movepacket->rotation = rotAngle;
		movepacket->headYaw = rotAngle.y;
	}
	if (!targetList.empty() && rotMode == 2 && packet->getId() == PacketID::MovePlayerPacket) {
		auto* movepacket = (MovePlayerPacket*)packet;
		movepacket->rotation = rotAngle;
		movepacket->headYaw = rotAngle.y;
	}
	if (!targetList.empty() && rotMode == 2 && packet->getId() == PacketID::PlayerAuthInput) {
		PlayerAuthInputPacket* authPacket = (PlayerAuthInputPacket*)packet;
		authPacket->rotation = rotAngle;
		authPacket->headYaw = rotAngle.y;
	}
/*	if (!targetList.empty() && packet->getId() == PacketID::MovePlayerPacket) {
		auto* movepacket = (MovePlayerPacket*)packet;
		movepacket->onGround = false;
		//movepacket->position = mc.getLocalPlayer()->getPosition()->y + 1;
	}*/
}

void Killaura::onImGuiRender(ImDrawList* d) {
	if (mc.getClientInstance()->getLevelRenderer() == nullptr) return;
	if (mc.getClientInstance()->getLevelRenderer()->levelRendererPlayer == nullptr) return;
	if (!mc.canUseMoveKeys()) return;

	LocalPlayer* localPlayer = mc.getLocalPlayer();
	if (visualRange) {
		Vec3<float> lpPos = localPlayer->stateVectorComponent->pos;
		if (mc.cameraPerspectiveMode == 0) lpPos = mc.getClientInstance()->getLevelRenderer()->levelRendererPlayer->cameraPos1;
		std::vector<Vec2<float>> pointsList;
		for (int i = 0; i < 360; i += 4) {
			float calcYaw = (i + 90) * (PI / 180);
			float x = cos(calcYaw) * targetRange;
			float z = sin(calcYaw) * targetRange;
			static Vec2<float> pointsVec2;
			if (ImGuiUtils::worldToScreen(lpPos.add(x, -1.6f, z), pointsVec2)) {
				pointsList.push_back(pointsVec2);
			}
		}
		for (int i = 0; i < pointsList.size(); i++) {
			int next = i + 1;
			if (next >= pointsList.size()) next = 0;
			d->AddLine(pointsList[i].toImVec2(), pointsList[next].toImVec2(), isInRange ? ImColor(255,0,0):vRColor.toImColor(), 2.f);
		}
	}
	if (visualRange1) {
		Vec3<float> lpPos = localPlayer->stateVectorComponent->pos;
		if (mc.cameraPerspectiveMode == 0) lpPos = mc.getClientInstance()->getLevelRenderer()->levelRendererPlayer->cameraPos1;
		std::vector<Vec2<float>> pointsList;
		for (int i = 0; i < 360; i += 4) {
			float calcYaw = (i + 90) * (PI / 180);
			float x = cos(calcYaw) * ARange;
			float z = sin(calcYaw) * ARange;
			static Vec2<float> pointsVec2;
			if (ImGuiUtils::worldToScreen(lpPos.add(x, -1.6f, z), pointsVec2)) {
				pointsList.push_back(pointsVec2);
			}
		}
		for (int i = 0; i < pointsList.size(); i++) {
			int next = i + 1;
			if (next >= pointsList.size()) next = 0;
			d->AddLine(pointsList[i].toImVec2(), pointsList[next].toImVec2(),   vRColor1.toImColor(), 2.f);
		}
	}
	if (visualRange2) {
		Vec3<float> lpPos = localPlayer->stateVectorComponent->pos;
		if (mc.cameraPerspectiveMode == 0) lpPos = mc.getClientInstance()->getLevelRenderer()->levelRendererPlayer->cameraPos1;
		std::vector<Vec2<float>> pointsList;
		for (int i = 0; i < 360; i += 4) {
			float calcYaw = (i + 90) * (PI / 180);
			float x = cos(calcYaw) * MaxSrange;
			float z = sin(calcYaw) * MaxSrange;
			static Vec2<float> pointsVec2;
			if (ImGuiUtils::worldToScreen(lpPos.add(x, -1.6f, z), pointsVec2)) {
				pointsList.push_back(pointsVec2);
			}
		}
		for (int i = 0; i < pointsList.size(); i++) {
			int next = i + 1;
			if (next >= pointsList.size()) next = 0;
			d->AddLine(pointsList[i].toImVec2(), pointsList[next].toImVec2(), vRColor2.toImColor(), 2.f);
		}
	}
	ImGuiIO& io = ImGui::GetIO();
	if (targetVisualize) {
		if (!targetList.empty() && targetList[0] != nullptr) {

			static unsigned int anim = 0;
			anim++;
			float height = targetList[0]->aabbComponent->aabb.upper.y - targetList[0]->aabbComponent->aabb.lower.y;
			const float coolAnim = (height / 2.f) + (height / 2.f) * sin(((float)anim / 60.f) * PI * 0.8f);

			Vec3<float> tgPos = targetList[0]->stateVectorComponent->pos;
			if (targetList[0]->isPlayer()) tgPos.y -= 1.6f;

			std::vector<Vec2<float>> pointsList;
			for (int i = 0; i < 360; i += 10) {
				float calcYaw = (i + 90) * (PI / 180);
				float x = cos(calcYaw) * 0.7f;
				float z = sin(calcYaw) * 0.7f;
				static Vec2<float> pointsVec2;
				if (ImGuiUtils::worldToScreen(tgPos.add(x, coolAnim, z), pointsVec2)) {
					pointsList.push_back(pointsVec2);
				}
			}
			for (int i = 0; i < pointsList.size(); i++) {
				int next = i + 1;
				if (next == pointsList.size()) next = 0;
				d->AddLine(pointsList[i].toImVec2(), pointsList[next].toImVec2(), visualizeColor.toImColor(), 2.f);
			}
		}
	}
}