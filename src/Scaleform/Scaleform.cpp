#include "Scaleform/Scaleform.h"

#include "Scaleform/LootMenu.h"

namespace Scaleform
{
	void Register()
	{
		LootMenu::Register();
		REX::INFO("Registered all movies");
	}
}
