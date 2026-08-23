#include "Hooks.h"

#include "HUDManager.h"
#include "Input/Input.h"

namespace Hooks
{
	void Install()
	{
		HUDManager::Install();
		Input::InputManager::Install();
		REX::INFO("Installed all hooks"sv);
	}
}
