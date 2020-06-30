#pragma once

namespace Scaleform
{
	class LootMenu;
}

// Interface for interacting with the loot menu outside of UI threads
class Loot
{
private:
	using LootMenu = Scaleform::LootMenu;

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

	void Process(LootMenu& a_menu);

	void ModSelectedIndex(double a_mod);
	void SetContainer(RE::TESObjectREFRPtr a_container);
	void TakeStack();

private:
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

	[[nodiscard]] RE::GPtr<LootMenu> GetMenu() const;
	[[nodiscard]] bool IsOpen() const;

	std::vector<Tasklet> _taskQueue;
	std::atomic_bool _enabled;
};
