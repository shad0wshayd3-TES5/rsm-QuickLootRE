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

	class ControlMap
	{
	public:
		[[nodiscard]] inline UInt32 operator()(std::string_view a_userEvent) const
		{
			auto input = RE::BSInputDeviceManager::GetSingleton();
			if (!input) {
				return INVALID;
			}

			if (input->IsGamepadEnabled()) {
				return MapGamepad(a_userEvent);
			} else {
				auto result = MapKeyboard(a_userEvent);
				return result != INVALID ? result : MapMouse(a_userEvent);
			}
		}

	private:
		static constexpr UInt32 INVALID = 282;	// ???

		template <class T, std::size_t N>
		struct simple_array
		{
			T c[N];
		};

		[[nodiscard]] inline UInt32 MapGamepad(std::string_view a_userEvent) const
		{
			using Key = RE::BSWin32GamepadDevice::Keys;

			auto controlMap = RE::ControlMap::GetSingleton();
			if (!controlMap) {
				return INVALID;
			}

			const auto make_mappings = []() noexcept {
				const auto map = [](std::size_t a_key) noexcept {
					for (std::size_t i = 0; i < 32; ++i) {
						auto to = (a_key >> i) & 1;
						if (to != 0) {
							return i;
						}
					}
					return static_cast<std::size_t>(0);
				};

				const auto assign = [](auto&& a_elem, auto&& a_first, auto&& a_second) noexcept {
					a_elem.first = a_first;
					a_elem.second = a_second;
				};

				constexpr auto N = []() noexcept {
					std::size_t size = 0;
					for (std::size_t i = Key::kRightShoulder; i != 0; i >>= 1) { ++size; }
					return size + 4 + 2;
				}();

				simple_array<std::pair<UInt32, std::size_t>, N> arr{};
				std::size_t idx = 0;
				std::size_t frame = 266;
				for (std::size_t key = Key::kUp; key <= Key::kRightShoulder; key <<= 1) {
					assign(arr.c[idx++], key, map(key) + frame);
				}
				for (std::size_t key = Key::kA; key <= Key::kY; key <<= 1) {
					assign(arr.c[idx++], key, map(key >> 2) + frame);
				}

				assign(arr.c[idx], Key::kLeftTrigger, frame + idx);
				++idx;
				assign(arr.c[idx], Key::kRightTrigger, frame + idx);
				return arr;
			};

			constexpr auto mappings = frozen::make_map(make_mappings().c);

			auto key = controlMap->GetMappedKey(a_userEvent, RE::INPUT_DEVICE::kGamepad);
			auto it = mappings.find(key);
			return it != mappings.end() ? it->second : INVALID;
		}

		[[nodiscard]] inline UInt32 MapKeyboard(std::string_view a_userEvent) const
		{
			using Key = RE::BSKeyboardDevice::Keys;

			auto controlMap = RE::ControlMap::GetSingleton();
			if (!controlMap) {
				return INVALID;
			}

			constexpr auto mappings = []() noexcept {
				const auto validate_range = [](auto&& mappings, UInt32 a_begin, UInt32 a_end) noexcept {
					for (auto i = a_begin; i <= a_end; ++i) {
						mappings[i] = i;
					}
				};

				std::array<UInt32, Key::kDelete + 1> arr{ INVALID };
				validate_range(arr, Key::kEscape, Key::kKP_Decimal);
				validate_range(arr, Key::kF11, Key::kF12);
				validate_range(arr, Key::kKP_Enter, Key::kRightControl);
				validate_range(arr, Key::kKP_Divide, Key::kKP_Divide);
				validate_range(arr, Key::kPrintScreen, Key::kRightAlt);
				validate_range(arr, Key::kPause, Key::kPause);
				validate_range(arr, Key::kHome, Key::kPageUp);
				validate_range(arr, Key::kLeft, Key::kLeft);
				validate_range(arr, Key::kRight, Key::kRight);
				validate_range(arr, Key::kEnd, Key::kDelete);

				return arr;
			}();

			auto key = controlMap->GetMappedKey(a_userEvent, RE::INPUT_DEVICE::kKeyboard);
			return key <= mappings.size() ? mappings[key] : INVALID;
		}

		[[nodiscard]] inline UInt32 MapMouse(std::string_view a_userEvent) const
		{
			using Key = RE::BSWin32MouseDevice::Keys;

			auto controlMap = RE::ControlMap::GetSingleton();
			if (!controlMap) {
				return INVALID;
			}

			constexpr auto mappings = []() noexcept {
				std::array<UInt32, Key::kWheelDown + 1> arr{ INVALID };
				for (auto i = Key::kLeftButton; i <= Key::kWheelDown; ++i) {
					arr[i] = i;
				}

				return arr;
			}();

			auto key = controlMap->GetMappedKey(a_userEvent, RE::INPUT_DEVICE::kMouse);
			return key <= mappings.size() ? mappings[key] : INVALID;
		}
	};
}
