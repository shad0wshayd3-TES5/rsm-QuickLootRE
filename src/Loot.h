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

	void ModSelectedIndex(double a_mod);
	inline void RefreshInventory() { _refreshInventory = true; }
	void SetContainer(RE::TESObjectREFRPtr a_container);
	void TakeStack();

protected:
	friend class LootMenu;

	void Process(LootMenu& a_menu);

private:
	using Tasklet = std::function<void(LootMenu&)>;

	inline Loot() :
		_taskQueue(),
		_refreshInventory(false),
		_enabled(true)
	{}

	Loot(const Loot&) = delete;
	Loot(Loot&&) = delete;

	~Loot() = default;

	Loot& operator=(const Loot&) = delete;
	Loot& operator=(Loot&&) = delete;

	void AddTask(Tasklet a_task);

	[[nodiscard]] RE::GPtr<LootMenu> GetMenu() const;
	[[nodiscard]] bool IsOpen() const;

	mutable std::mutex _lock;
	std::vector<Tasklet> _taskQueue;
	std::atomic_bool _refreshInventory;
	std::atomic_bool _enabled;
};
