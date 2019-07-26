#include "settings.h"


bool Settings::loadSettings(bool a_dumpParse)
{
	return Json2Settings::Settings::loadSettings(FILE_NAME, a_dumpParse);
}


decltype(Settings::disableInCombat)			Settings::disableInCombat("disableInCombat", true);
decltype(Settings::disableTheft)			Settings::disableTheft("disableTheft", false);
decltype(Settings::disablePickPocketing)	Settings::disablePickPocketing("disablePickpocketing", false);
decltype(Settings::disableIfEmpty)			Settings::disableIfEmpty("disableIfEmpty", true);
decltype(Settings::disableSingleLoot)		Settings::disableSingleLoot("disableSingleLoot", false);
decltype(Settings::disableAnimations)		Settings::disableAnimations("disableAnimations", false);
decltype(Settings::disableInvisDispell)		Settings::disableInvisDispell("disableInvisDispell", false);
decltype(Settings::disableForAnimals)		Settings::disableForAnimals("disableForAnimals", false);
decltype(Settings::disableForActiOverride)	Settings::disableForActiOverride("disableForActiOverride", false);
decltype(Settings::disableActiTextHook)		Settings::disableActiTextHook("disableActiTextHook", false);
decltype(Settings::disableOnActiDispatch)	Settings::disableOnActiDispatch("disableOnActiDispatch", false);
decltype(Settings::itemLimit)				Settings::itemLimit("itemLimit", 100);
decltype(Settings::scale)					Settings::scale("scale", -1.0);
decltype(Settings::positionX)				Settings::positionX("positionX", -1.0);
decltype(Settings::positionY)				Settings::positionY("positionY", -1.0);
decltype(Settings::opacity)					Settings::opacity("opacity", -1.0);
decltype(Settings::singleLootModifier)		Settings::singleLootModifier("singleLootModifier", "sprint");
decltype(Settings::takeMethod)				Settings::takeMethod("takeMethod", "activate");
decltype(Settings::takeAllMethod)			Settings::takeAllMethod("takeAllMethod", "togglePOV");
decltype(Settings::searchMethod)			Settings::searchMethod("searchMethod", "readyWeapon");
decltype(Settings::interfaceStyle)			Settings::interfaceStyle("interfaceStyle", "default");
decltype(Settings::sortOrder)				Settings::sortOrder("sortOrder", { "stolen", "type", "name", "value", "count" });
