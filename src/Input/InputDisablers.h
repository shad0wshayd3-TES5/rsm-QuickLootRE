#pragma once

#include "Input/Input.h"

namespace Input
{
	class Disablers
	{
	public:
		Disablers() = default;
		Disablers(const Disablers&) = default;
		Disablers(Disablers&&) = default;

		inline ~Disablers() { Disable(); }

		Disablers& operator=(const Disablers&) = default;
		Disablers& operator=(Disablers&&) = default;

		inline void Enable()
		{
			auto controlMap = RE::ControlMap::GetSingleton();
			if (controlMap) {
				controlMap->ToggleControls(QUICKLOOT_FLAG, false);
			}
		}

		inline void Disable()
		{
			auto controlMap = RE::ControlMap::GetSingleton();
			if (controlMap) {
				controlMap->ToggleControls(QUICKLOOT_FLAG, true);
			}
		}
	};
}
