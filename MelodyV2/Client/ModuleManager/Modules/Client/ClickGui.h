#pragma once
#include "../Module.h"
#include "../../../CommandManager/CommandManager.h"
#include "../../../ConfigManager/ConfigManager.h"
void getModuleListByCategory(Category category, std::vector<Module*>& modList);

struct ClickWindow {
	std::string windowName;
	Vec2<float> pos;
	float scrolloffset = 0.f;
	float yOffset;
	bool extended = true;
	bool isDragging = false;
	Category category;
	std::vector<Module*> modList;

	ClickWindow(const Vec2<float>& Pos, const Category& c) {
		this->pos = Pos;
		this->extended = true;
		if (c == Category::COMBAT) this->windowName = "Combat";
		else if (c == Category::MISC) this->windowName = "Misc";
		else if (c == Category::RENDER) this->windowName = "Render";
		else if (c == Category::MOVEMENT) this->windowName = "Movement";
		else if (c == Category::PLAYER) this->windowName = "Player";
		else if (c == Category::CLIENT) this->windowName = "Client";
		else if (c == Category::UNKNOW) this->windowName = "Unknow";

		getModuleListByCategory(c, modList);
	}
};

class ClickGui : public Module {
private:
	UIColor ZitiColor = UIColor(255, 255, 255, 255);
	UIColor CBGColor = UIColor(24, 24, 24, 200);
	UIColor MBGColor = UIColor(24, 24, 24, 200);

	int opicity = 255;
	std::vector<ClickWindow*> windowList;
	bool isLeftClickDown = false;
	bool isRightClickDown = false;
	bool isHoldingLeftClick = false;
	bool isHoldingRightClick = false;
	Vec2<float> startDragPos = Vec2<float>(0.f, 0.f);
	float openDuration = 0.f;
	bool initClickGui = false;
	void init();

	bool tooltips = true;
	ConfigManager* configMgr = nullptr;
	bool isShiftDown = false;
	bool isBackKeyDown = false;
	std::string searchingModule;
	bool isSearching = false;
	int lastKeyPress = -1;
	bool* isChoosingKeyBindPtr = nullptr;
public:
	bool particles = true;
	int Pmode = 0;
	int Mode = 0;
	float blurStrength = 7.5f;
	ClickGui();
	~ClickGui();

	void onMouseUpdate(char mouseButton, bool isDown);
	void onKey(int key, bool isDown);
	void render(ImDrawList* drawlist);

	virtual bool isVisible() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};