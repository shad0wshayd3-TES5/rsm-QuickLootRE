#include "Scaleform/Scaleform.h"

#include "Scaleform/LootMenu.h"

namespace Scaleform
{
	void Register()
	{
		LootMenu::Register();
		SKSE::log::info("Registered all movies");
	}
}
