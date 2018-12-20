#pragma once

#include "Json2Settings.h"  // Json2Settings


namespace QuickLootRE
{
	class Settings : public Json2Settings::Settings
	{
	public:
		Settings() = delete;
		static bool loadSettings(bool a_dumpParse = false);


		static bSetting					disableInCombat;
		static bSetting					disableTheft;
		static bSetting					disablePickPocketing;
		static bSetting					disableIfEmpty;
		static bSetting					disableSingleLoot;
		static bSetting					disableAnimations;
		static bSetting					disableForAnimals;
		static bSetting					disableForActiOverride;
		static bSetting					disableActiTextHook;
		static bSetting					disableOnActiDispatch;
		static iSetting					itemLimit;
		static fSetting					scale;
		static fSetting					positionX;
		static fSetting					positionY;
		static fSetting					opacity;
		static sSetting					singleLootModifier;
		static sSetting					takeMethod;
		static sSetting					takeAllMethod;
		static sSetting					searchMethod;
		static sSetting					interfaceStyle;
		static aSetting<std::string>	sortOrder;

		static bool						isApplied;

	private:
		static constexpr char*			FILE_NAME = "Data\\SKSE\\Plugins\\QuickLootRE.json";
	};
}
