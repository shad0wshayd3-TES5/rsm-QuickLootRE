#pragma once

#include <functional>
#include <vector>

#include "RE/Skyrim.h"


namespace Scaleform
{
	class LootMenu;
}


class Loot
{
public:
	static Loot* GetSingleton();

	void Close();
	void Open();
	void Process(Scaleform::LootMenu& a_menu);

	void SetContainer(RE::TESObjectREFRPtr a_container);

private:
	using LootMenu = Scaleform::LootMenu;
	using Tasklet = std::function<void(Scaleform::LootMenu&)>;

	Loot() = default;
	Loot(const Loot&) = delete;
	Loot(Loot&&) = delete;
	~Loot() = default;

	Loot& operator=(const Loot&) = delete;
	Loot& operator=(Loot&&) = delete;

	void AddTask(Tasklet a_task);
	RE::GPtr<LootMenu> GetMenu();

	std::vector<Tasklet> _taskQueue;
};
