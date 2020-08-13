#pragma once

class HUDManager
{
public:
	inline static HUDManager& GetSingleton()
	{
		static HUDManager singleton;
		return singleton;
	}

	static inline void Install()
	{
		Handler<241857>::Install();	 // TESNPC
		Handler<234000>::Install();	 // TESObjectACTI
		Handler<234148>::Install();	 // TESObjectCONT
		logger::info("Installed {}"sv, typeid(HUDManager).name());
	}

	inline void Enable() noexcept { _enabled = true; }
	inline void Disable() noexcept { _enabled = false; }

protected:
	template <std::uint64_t ID>
	class Handler
	{
	public:
		inline static void Install()
		{
			REL::Relocation<std::uintptr_t> vtbl{ REL::ID(ID) };
			_func = vtbl.write_vfunc(0x4C, GetActivateText);
			logger::info("Installed {}"sv, typeid(Handler).name());
		}

	private:
		Handler() = delete;
		Handler(const Handler&) = delete;
		Handler(Handler&&) = delete;

		~Handler() = delete;

		Handler& operator=(const Handler&) = delete;
		Handler& operator=(Handler&&) = delete;

		inline static bool GetActivateText(RE::TESBoundObject* a_this, RE::TESObjectREFR* a_activator, RE::BSString& a_dst)
		{
			const auto& proxy = HUDManager::GetSingleton();
			if (proxy.Enabled()) {
				return false;
			} else {
				return _func(a_this, a_activator, a_dst);
			}
		}

		static inline REL::Relocation<decltype(GetActivateText)> _func;
	};

	[[nodiscard]] inline bool Enabled() const noexcept { return _enabled; }
	[[nodiscard]] inline bool Disabled() const noexcept { return !_enabled; }

private:
	constexpr HUDManager() noexcept = default;
	HUDManager(const HUDManager&) = delete;
	HUDManager(HUDManager&&) = delete;

	~HUDManager() = default;

	HUDManager& operator=(const HUDManager&) = delete;
	HUDManager& operator=(HUDManager&&) = delete;

	std::atomic_bool _enabled{ false };
};
