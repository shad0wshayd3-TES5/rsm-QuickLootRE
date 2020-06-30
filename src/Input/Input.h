#pragma once

namespace Input
{
	constexpr inline auto QUICKLOOT_FLAG = static_cast<RE::UserEvents::USER_EVENT_FLAG>(1 << 12);

	inline void HookControlMap()
	{
		using Devices = RE::INPUT_DEVICES;
		using Gamepad = RE::BSWin32GamepadDevice::Key;
		using UEFlag = RE::UserEvents::USER_EVENT_FLAG;
		constexpr auto HOOK_FLAGS = UEFlag::kActivate | UEFlag::kPOVSwitch;

		auto controls = RE::ControlMap::GetSingleton();
		if (!controls) {
			assert(false);
		} else {
			for (auto& map : controls->controlMap) {
				if (!map) {
					continue;
				}

				for (auto& deviceMapping : map->deviceMappings) {
					for (auto& userMapping : deviceMapping) {
						auto& userFlags = userMapping.userEventGroupFlag;
						if ((userFlags & HOOK_FLAGS) != UEFlag::kNone) {
							userFlags |= QUICKLOOT_FLAG;
						}
					}
				}

				for (auto& userMapping : map->deviceMappings[Devices::kGamepad]) {
					switch (userMapping.inputKey) {
					case Gamepad::kUp:
					case Gamepad::kDown:
						userMapping.userEventGroupFlag |= QUICKLOOT_FLAG;
						break;
					default:
						break;
					}
				}
			}

			controls->ToggleControls(QUICKLOOT_FLAG, true);
		}
	}
}
