#pragma once
#include <string>
#include "BlockSource.h"
class Weather {
public:
	bool isRaining() {
	return *((float*)this + 14) > 0.2;
}
};
class Dimension {
public:
	BUILD_ACCESS(this, BlockSource*, blockSource, 0xD0);
	BUILD_ACCESS(this, std::string, name, 0x140);
	BUILD_ACCESS(this, uint32_t, id, 0x160);
};