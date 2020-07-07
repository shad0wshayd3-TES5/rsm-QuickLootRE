#pragma once

namespace Input
{
	constexpr inline auto QUICKLOOT_FLAG = static_cast<RE::UserEvents::USER_EVENT_FLAG>(1 << 12);

	class UserEventMap
	{
	public:
		using value_type = std::string_view;

		inline UserEventMap() :
			_mappings{}
		{
			insert("Activate");
			insert("Ready Weapon");
		}

		[[nodiscard]] inline bool operator()(std::size_t a_device, value_type a_value) const
		{
			const auto& mapping = _mappings[a_device];
			auto it = mapping.find(a_value);
			return it != mapping.end();
		}

	private:
		inline void insert(value_type a_value)
		{
			for (std::size_t i = 0; i < RE::INPUT_DEVICES::kTotal; ++i) {
				_mappings[i].insert(a_value);
			}
		}

		inline void insert(value_type a_value, RE::INPUT_DEVICE a_device)
		{
			_mappings[a_device].insert(a_value);
		}

		struct cicompare
		{
			[[nodiscard]] inline bool operator()(const value_type& a_lhs, const value_type& a_rhs) const
			{
				return _stricmp(a_lhs.data(), a_rhs.data()) < 0;
			}
		};

		std::array<std::set<value_type, cicompare>, RE::INPUT_DEVICES::kTotal> _mappings;
	};

	class IDCodeMap
	{
	public:
		using value_type = UInt32;

		inline IDCodeMap() :
			_mappings{}
		{
			using Device = RE::INPUT_DEVICE;
			using Gamepad = RE::BSWin32GamepadDevice::Key;
			using Mouse = RE::BSWin32MouseDevice::Key;

			insert(Gamepad::kUp, Device::kGamepad);
			insert(Gamepad::kDown, Device::kGamepad);

			insert(Mouse::kWheelUp, Device::kMouse);
			insert(Mouse::kWheelDown, Device::kMouse);
		}

		[[nodiscard]] inline bool operator()(std::size_t a_device, value_type a_value) const
		{
			const auto& mapping = _mappings[a_device];
			auto it = mapping.find(a_value);
			return it != mapping.end();
		}

	private:
		inline void insert(value_type a_value)
		{
			for (std::size_t i = 0; i < RE::INPUT_DEVICES::kTotal; ++i) {
				_mappings[i].insert(a_value);
			}
		}

		inline void insert(value_type a_value, RE::INPUT_DEVICE a_device)
		{
			_mappings[a_device].insert(a_value);
		}

		std::array<std::set<value_type>, RE::INPUT_DEVICES::kTotal> _mappings;
	};

	inline void HookControlMap()
	{
		auto controls = RE::ControlMap::GetSingleton();
		if (!controls) {
			assert(false);
			return;
		}

		UserEventMap eventMap;
		IDCodeMap idMap;

		for (auto& map : controls->controlMap) {
			if (map) {
				for (std::size_t i = 0; i < RE::INPUT_DEVICES::kTotal; ++i) {
					for (auto& userMapping : map->deviceMappings[i]) {
						if (eventMap(i, userMapping.eventID) || idMap(i, userMapping.inputKey)) {
							userMapping.userEventGroupFlag |= QUICKLOOT_FLAG;
						}
					}
				}
			}
		}

		controls->ToggleControls(QUICKLOOT_FLAG, true);
	}
}
