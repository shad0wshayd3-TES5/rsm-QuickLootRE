#include "Scaleform.h"

#include "LootMenu.h"


namespace Scaleform
{
	void Register()
	{
		LootMenu::Register();
		_MESSAGE("Registered all movies");
	}
}
