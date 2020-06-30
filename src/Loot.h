#pragma once

namespace Scaleform
{
	class LootMenu;
}

// Interface for interacting with the loot menu outside of UI threads
class Loot
{
public:
	static inline Loot* GetSingleton()
	{
		static Loot singleton;
		return std::addressof(singleton);
	}

	inline void Disable()
	{
		_enabled = false;
		Close();
	}

	inline void Enable() { _enabled = true; }

	void Close();
	void Open();

	void Process(Scaleform::LootMenu& a_menu);

	void ModSelectedIndex(double a_mod);
	void SetContainer(RE::TESObjectREFRPtr a_container);
	void TakeStack();

private:
	using LootMenu = Scaleform::LootMenu;
	using Tasklet = std::function<void(LootMenu&)>;

	inline Loot() :
		_taskQueue(),
		_enabled(true)
	{}

	Loot(const Loot&) = delete;
	Loot(Loot&&) = delete;

	~Loot() = default;

	Loot& operator=(const Loot&) = delete;
	Loot& operator=(Loot&&) = delete;

	inline void AddTask(Tasklet a_task);

	RE::GPtr<LootMenu> GetMenu();

	std::vector<Tasklet> _taskQueue;
	std::atomic_bool _enabled;
};
