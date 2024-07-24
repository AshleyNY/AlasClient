#include "PopCounter.h"

using namespace std;
PopCounter::PopCounter() : Module("PopCounter", "Counter Pop", Category::COMBAT) {
	addBoolCheck("SendChat", "NULL", &sendchat);
	addSlider<float>("Duration", "NULL", ValueType::FLOAT_T, &Dura, 0.8f, 2.f);
	//registerFloatSetting("NotiDurationTime", &Dura, Dura, 2.f, 0.8f);
}


std::string PopCounter::getModName() {
	return names;
}



static std::vector<Actor*> playerlist;


void PopCounter::onNormalTick(Actor* actor) {
	playerlist.clear();

	LocalPlayer* player = mc.getLocalPlayer();
	PlayerInventory* plrInv = player->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	GameMode* gm = mc.getGameMode();
	BlockSource* region = player->dimension->blockSource;
	Level* level = player->getLevel();

	if (player == nullptr) 
		return;

	
	if (!player->isAlive()) {
		popcount = 0;
	}

	for (Actor* actor : level->getRuntimeActorList()) {
		if (TargetUtils::isTargetValid(actor, false)) {
			float dist = actor->getPosition()->dist(*player->getPosition());
			if (dist < 20) playerlist.push_back(actor);
		}
	}

	if (!playerlist.empty()) {
			if (popcount > 37) {
				popcount = 0;
			}
			//ItemStack *current = i->getOffhandSlot()->item;
			if (playerlist[0]->getOffhandSlot()->item == nullptr) {
				if (!totem) {
					ItemStack* stack = playerlist[0]->getArmor(1);
					if (stack->item != nullptr) {
						popcount += 1;
						mc.DisplayClientMessage("[%s%s%s] %s%s %sPopped %s%i%s Totems!", LIGHT_PURPLE, u8"Éµ±Æ¿Í»§¶Ë", WHITE, DARK_AQUA, playerlist[0]->getNameTag()->c_str(), WHITE, RED, popcount, WHITE);
					
						std::string messagebox = playerlist[0]->getNameTag()->c_str() + std::to_string(popcount) + " totems!";
						Notifications::addNotifBox(messagebox, Dura);
						totem = true;
					}
				}
			}
			else {
				totem = false;
			};
		
	}
}