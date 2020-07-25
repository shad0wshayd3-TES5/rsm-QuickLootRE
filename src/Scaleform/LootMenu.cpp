#include "Scaleform/LootMenu.h"

#include "Loot.h"

namespace Scaleform
{
	void LootMenu::Close()
	{
		auto loot = Loot::GetSingleton();
		loot->Close();
	}

	void LootMenu::ProcessDelegate()
	{
		auto loot = Loot::GetSingleton();
		loot->Process(*this);
	}

	void LootMenu::QueueInventoryRefresh()
	{
		auto loot = Loot::GetSingleton();
		loot->RefreshInventory();
	}

	void LootMenu::QueueUIRefresh()
	{
		auto loot = Loot::GetSingleton();
		loot->RefreshUI();
	}
}
