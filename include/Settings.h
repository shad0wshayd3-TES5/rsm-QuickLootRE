#pragma once

#include "Json2Settings.h"


class Settings
{
public:
	using bSetting = Json2Settings::bSetting;
	using iSetting = Json2Settings::iSetting;
	using fSetting = Json2Settings::fSetting;
	using sSetting = Json2Settings::sSetting;
	template <class T> using aSetting = Json2Settings::aSetting<T>;


	Settings() = delete;

	static bool LoadSettings(bool a_dumpParse = false);


	static bSetting disableInCombat;
	static bSetting disableTheft;
	static bSetting disablePickPocketing;
	static bSetting disableIfEmpty;
	static bSetting disableSingleLoot;
	static bSetting disableAnimations;
	static bSetting disableInvisDispell;
	static bSetting disableForAnimals;
	static bSetting disableForActiOverride;
	static bSetting disableActiTextHook;
	static bSetting disableOnActiDispatch;
	static iSetting itemLimit;
	static fSetting scale;
	static fSetting positionX;
	static fSetting positionY;
	static fSetting opacity;
	static sSetting singleLootModifier;
	static sSetting takeMethod;
	static sSetting takeAllMethod;
	static sSetting searchMethod;
	static sSetting interfaceStyle;
	static aSetting<std::string> sortOrder;

private:
	static constexpr char FILE_NAME[] = "Data\\SKSE\\Plugins\\QuickLootRE.json";
};
