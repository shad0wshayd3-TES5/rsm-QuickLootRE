#include "settings.h"

#include "Json2Settings.h"  // Json2Settings


namespace QuickLootRE
{
	bool Settings::loadSettings(bool a_dumpParse)
	{
		Json2Settings::Settings::setFileName(FILE_NAME);
		return Json2Settings::Settings::loadSettings(a_dumpParse);
	}


	bSetting				Settings::disableInCombat("disableInCombat", true, true);
	bSetting				Settings::disableTheft("disableTheft", true, false);
	bSetting				Settings::disablePickPocketing("disablePickpocketing", true, false);
	bSetting				Settings::disableIfEmpty("disableIfEmpty", true, true);
	bSetting				Settings::disableSingleLoot("disableSingleLoot", true, false);
	bSetting				Settings::disableAnimations("disableAnimations", true, false);
	bSetting				Settings::disableInvisDispell("disableInvisDispell", true, false);
	bSetting				Settings::disableForAnimals("disableForAnimals", true, false);
	bSetting				Settings::disableForActiOverride("disableForActiOverride", true, false);
	bSetting				Settings::disableActiTextHook("disableActiTextHook", true, false);
	bSetting				Settings::disableOnActiDispatch("disableOnActiDispatch", true, false);
	iSetting				Settings::itemLimit("itemLimit", true, 100);
	fSetting				Settings::scale("scale", true, -1.0);
	fSetting				Settings::positionX("positionX", true, -1.0);
	fSetting				Settings::positionY("positionY", true, -1.0);
	fSetting				Settings::opacity("opacity", true, -1.0);
	sSetting				Settings::singleLootModifier("singleLootModifier", false, "sprint");
	sSetting				Settings::takeMethod("takeMethod", false, "activate");
	sSetting				Settings::takeAllMethod("takeAllMethod", false, "togglePOV");
	sSetting				Settings::searchMethod("searchMethod", false, "readyWeapon");
	sSetting				Settings::interfaceStyle("interfaceStyle", false, "default");
	aSetting<std::string>	Settings::sortOrder("sortOrder", false, { "stolen", "type", "name", "value", "count" });

	bool					Settings::isApplied = false;
}
