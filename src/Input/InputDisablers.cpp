#include "Input/InputDisablers.h"

namespace Input
{
	Disablers::Disablers() :
		_disablers()
	{
		_disablers.emplace_back(UEFlag::kPOVSwitch);

		Enable();
	}

	Disablers::~Disablers()
	{
		Disable();
	}

	Disablers::Disabler::Disabler(UEFlag a_flag) :
		_originalState(std::nullopt),
		_flag(a_flag)
	{}

	void Disablers::Disabler::Enable()
	{
		auto controlMap = RE::ControlMap::GetSingleton();
		if (controlMap) {
			_originalState = controlMap->AreControlsEnabled(_flag);
			controlMap->ToggleControls(_flag, false);
		}
	}

	void Disablers::Disabler::Disable()
	{
		if (_originalState) {
			auto controlMap = RE::ControlMap::GetSingleton();
			if (controlMap) {
				controlMap->ToggleControls(_flag, *_originalState);
			}
			_originalState = std::nullopt;
		}
	}

	void Disablers::Enable()
	{
		for (auto& disabler : _disablers) {
			disabler.Enable();
		}
	}

	void Disablers::Disable()
	{
		for (auto& disabler : _disablers) {
			disabler.Disable();
		}
	}
}
