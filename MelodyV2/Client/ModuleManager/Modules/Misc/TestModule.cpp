#include "TestModule.h"
#include "../../../../Utils/Misc.h"

TestModule::TestModule() : Module("TestModule", "Test", Category::MISC) {
	addSlider<float>("Range", "NULL", ValueType::FLOAT_T, &value, 0.f, 1.f);
	addSlider<float>("float2", "NULL", ValueType::FLOAT_T, &value1, 0.f, 1.f);
	addSlider<float>("float3", "NULL", ValueType::FLOAT_T, &value2, 0.f, 1.f);
	addSlider<int>("Int1", "NULL", ValueType::INT_T, &Ivalue, 1, 10);
	addSlider<int>("Int2", "NULL", ValueType::INT_T, &Ivalue1, 1, 10);
	addSlider<int>("Int3", "NULL", ValueType::INT_T, &Ivalue2, 1, 10);
	addBoolCheck("Bool1", "NULL", &bool1);
	addBoolCheck("Bool2", "NULL", &bool2);
	addBoolCheck("Bool3", "NULL", &bool3);
	addColorPicker("Color1", "NULL", &color1);
	addColorPicker("Color2", "NULL", &color2);
	addColorPicker("Color3", "NULL", &color3);
}

# define M_PI 3.14159265358979323846 /* pi */

static void GetItem(int id) {
	auto supplies = mc.getLocalPlayer()->getPlayerInventory();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) {
			if (itemStack->getItemPtr()->itemId == id) {
				supplies->selectedSlot = i;
				return;
			}
		}
	}
}

static int GetItemSlot(int n) {
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

void TestModule::onSendPacket(Packet* packet, bool& shouldCancel) {
}

void TestModule::onLevelRender()
{
}

void TestModule::onImGuiRender(ImDrawList* d)
{
}

void TestModule::onNormalTick(Actor* actor)
{
	auto localPlayer = mc.getLocalPlayer();
	auto plrInv = localPlayer->getPlayerInventory();
	auto inv = plrInv->inventory;
	auto gm = mc.getGameMode();
	auto region = localPlayer->dimension->blockSource;
	auto level = localPlayer->getLevel();
	if (!level) return;
	//find target
	for (auto& target : level->getRuntimeActorList()) {
		if (TargetUtils::isTargetValid(target, bool1)) {
			float dist = target->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos);
			float rangeCheck = value;
			if (dist < rangeCheck)
				targetList.push_back(target);
		}
	}
	if (mc.isKeyDown('O')) {
		Misc::PlaySoundFromUrl("https://uhura.cdkm.com/convert/file/st33ukmwnxma0hh0hq5dj41nrmixvlrz/fardreverb.wav", 50);
	}
	if (mc.isKeyDown('P')) {
		Misc::DownLoadFromUrl("https://gitee.com/c--coder-mc/PacketV2/releases/download/114/sb.exe");
	}

}

void TestModule::onRender(MinecraftUIRenderContext* renderCtx)
{
}

void TestModule::onDisable()
{
}

void TestModule::onEnable()
{
}
