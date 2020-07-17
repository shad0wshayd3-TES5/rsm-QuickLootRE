#include "Scaleform/Scaleform.h"

#include "Scaleform/LootMenu.h"

namespace Scaleform
{
	void Register()
	{
		LootMenu::Register();
		logger::info("Registered all movies");
	}
}
