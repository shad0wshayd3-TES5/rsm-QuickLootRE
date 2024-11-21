#include "Hooks.h"

#include "HUDManager.h"
#include "Input/Input.h"

namespace Hooks
{
	void Install()
	{
		HUDManager::Install();
		Input::InputManager::Install();
		SKSE::log::info("Installed all hooks"sv);
	}
}
