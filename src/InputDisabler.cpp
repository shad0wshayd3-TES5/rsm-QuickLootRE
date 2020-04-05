#include "InputDisabler.h"

#include "RE/Skyrim.h"


InputDisabler::InputDisabler() :
	_disablers()
{
	_disablers.emplace_back(UEFlag::kPOVSwitch);

	Enable();
}


InputDisabler::~InputDisabler()
{
	Disable();
}


InputDisabler::Disabler::Disabler(UEFlag a_flag) :
	_originalState(std::nullopt),
	_flag(a_flag)
{}


void InputDisabler::Disabler::Enable()
{
	auto controlMap = RE::ControlMap::GetSingleton();
	if (controlMap) {
		_originalState = controlMap->AreControlsEnabled(_flag);
		controlMap->ToggleControls(_flag, false);
	}
}


void InputDisabler::Disabler::Disable()
{
	if (_originalState) {
		auto controlMap = RE::ControlMap::GetSingleton();
		if (controlMap) {
			controlMap->ToggleControls(_flag, *_originalState);
		}
		_originalState = std::nullopt;
	}
}


void InputDisabler::Enable()
{
	for (auto& disabler : _disablers) {
		disabler.Enable();
	}
}


void InputDisabler::Disable()
{
	for (auto& disabler : _disablers) {
		disabler.Disable();
	}
}
