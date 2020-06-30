#pragma once

class HUDHandler
{
public:
	inline static HUDHandler* GetSingleton()
	{
		static HUDHandler singleton;
		return std::addressof(singleton);
	}

	inline static void Install()
	{
		Handler<241857>::Install();	 // TESNPC
		Handler<234000>::Install();	 // TESObjectACTI
		Handler<234148>::Install();	 // TESObjectCONT
		_MESSAGE("Installed %s", typeid(HUDHandler).name());
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
			REL::Offset<std::uintptr_t> vtbl = REL::ID(ID);
			_func = vtbl.write_vfunc(0x4C, GetActivateText);
			_MESSAGE("Installed %s", typeid(Handler).name());
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
			const auto* proxy = HUDHandler::GetSingleton();
			if (proxy->Enabled()) {
				return false;
			} else {
				return _func(a_this, a_activator, a_dst);
			}
		}

		static inline REL::Offset<decltype(GetActivateText)> _func;
	};

	[[nodiscard]] inline bool Enabled() const noexcept { return _enabled; }
	[[nodiscard]] inline bool Disabled() const noexcept { return !_enabled; }

private:
	constexpr HUDHandler() noexcept :
		_enabled(false)
	{}

	HUDHandler(const HUDHandler&) = delete;
	HUDHandler(HUDHandler&&) = delete;

	~HUDHandler() = default;

	HUDHandler& operator=(const HUDHandler&) = delete;
	HUDHandler& operator=(HUDHandler&&) = delete;

	std::atomic_bool _enabled;
};
