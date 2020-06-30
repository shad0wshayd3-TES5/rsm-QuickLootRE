#include "Scaleform/LootMenu.h"

#include "Loot.h"

namespace Scaleform
{
	void LootMenu::ProcessDelegate()
	{
		auto loot = Loot::GetSingleton();
		loot->Process(*this);
	}
}
