#include "Scaleform/Scaleform.h"

#include "Scaleform/LootMenu.h"

namespace Scaleform
{
	void Register()
	{
		LootMenu::Register();
		_MESSAGE("Registered all movies");
	}
}
