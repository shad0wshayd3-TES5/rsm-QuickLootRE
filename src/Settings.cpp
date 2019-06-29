#include "settings.h"


bool Settings::loadSettings(bool a_dumpParse)
{
	return Json2Settings::Settings::loadSettings(FILE_NAME, false, a_dumpParse);
}


decltype(Settings::disableInCombat)			Settings::disableInCombat("disableInCombat", true, true);
decltype(Settings::disableTheft)			Settings::disableTheft("disableTheft", true, false);
decltype(Settings::disablePickPocketing)	Settings::disablePickPocketing("disablePickpocketing", true, false);
decltype(Settings::disableIfEmpty)			Settings::disableIfEmpty("disableIfEmpty", true, true);
decltype(Settings::disableSingleLoot)		Settings::disableSingleLoot("disableSingleLoot", true, false);
decltype(Settings::disableAnimations)		Settings::disableAnimations("disableAnimations", true, false);
decltype(Settings::disableInvisDispell)		Settings::disableInvisDispell("disableInvisDispell", true, false);
decltype(Settings::disableForAnimals)		Settings::disableForAnimals("disableForAnimals", true, false);
decltype(Settings::disableForActiOverride)	Settings::disableForActiOverride("disableForActiOverride", true, false);
decltype(Settings::disableActiTextHook)		Settings::disableActiTextHook("disableActiTextHook", true, false);
decltype(Settings::disableOnActiDispatch)	Settings::disableOnActiDispatch("disableOnActiDispatch", true, false);
decltype(Settings::itemLimit)				Settings::itemLimit("itemLimit", true, 100);
decltype(Settings::scale)					Settings::scale("scale", true, -1.0);
decltype(Settings::positionX)				Settings::positionX("positionX", true, -1.0);
decltype(Settings::positionY)				Settings::positionY("positionY", true, -1.0);
decltype(Settings::opacity)					Settings::opacity("opacity", true, -1.0);
decltype(Settings::singleLootModifier)		Settings::singleLootModifier("singleLootModifier", false, "sprint");
decltype(Settings::takeMethod)				Settings::takeMethod("takeMethod", false, "activate");
decltype(Settings::takeAllMethod)			Settings::takeAllMethod("takeAllMethod", false, "togglePOV");
decltype(Settings::searchMethod)			Settings::searchMethod("searchMethod", false, "readyWeapon");
decltype(Settings::interfaceStyle)			Settings::interfaceStyle("interfaceStyle", false, "default");
decltype(Settings::sortOrder)				Settings::sortOrder("sortOrder", false, { "stolen", "type", "name", "value", "count" });
