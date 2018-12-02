#include "settings.h"

#include <fstream>  // ifstream
#include <exception>  // exception
#include <vector>  // vector
#include <string>  // string

#include "json.hpp"  // json


namespace QuickLootRE
{
	bool Settings::loadSettings()
	{
		using nlohmann::json;

		std::ifstream istream(FILE_NAME);
		if (!istream.is_open()) {
			_ERROR("[ERROR] Failed to open .json file!\n");
		}
		json j;
		try {
			istream >> j;
			json::iterator it;
			for (auto& setting : settings) {
				it = j.find(setting->key());

				if (it == j.end()) {
					_ERROR("[ERROR] Failed to find (%s) within .json!\n", setting->key().c_str());
					continue;
				}

				switch (it->type()) {
				case json::value_t::array:
				{
					json jArr = it.value();
					setting->assign(jArr);
					break;
				}
				case json::value_t::string:
				{
					std::string str = it.value();
					setting->assign(str);
					break;
				}
				case json::value_t::boolean:
				{
					bool b = it.value();
					setting->assign(b);
					break;
				}
				case json::value_t::number_integer:
				case json::value_t::number_unsigned:
				{
					int num = it.value();
					setting->assign(num);
					break;
				}
				case json::value_t::number_float:
				{
					float num = it.value();
					setting->assign(num);
					break;
				}
				default:
					_DMESSAGE("[ERROR] Parsed value is of invalid type (%s)!\n", j.type_name());
				}
			}
		} catch (std::exception& e) {
			_ERROR("[ERROR] Failed to parse .json file!\n");
			_ERROR(e.what());
			istream.close();
			return false;
		}

		istream.close();
		return true;
	}


	ISetting* Settings::set(std::string& a_key, int a_val)
	{
		for (auto& setting : consoleSettings) {
			if (setting->key() == a_key) {
				setting->assign(a_val);
				return setting;
			}
		}
		return 0;
	}


	void Settings::dump()
	{
		_DMESSAGE("=== SETTINGS DUMP BEGIN ===");
		for (auto& setting : settings) {
			setting->dump();
		}
		_DMESSAGE("=== SETTINGS DUMP END ===");
	}


	bSetting				Settings::disableInCombat("disableInCombat", true, true);
	bSetting				Settings::disableTheft("disableTheft", true, false);
	bSetting				Settings::disablePickPocketing("disablePickpocketing", true, false);
	bSetting				Settings::disableIfEmpty("disableIfEmpty", true, true);
	bSetting				Settings::disableSingleLoot("disableSingleLoot", true, false);
	bSetting				Settings::disableForAnimals("disableForAnimals", true, false);
	bSetting				Settings::disableActiTextHook("disableActiTextHook", true, false);
	bSetting				Settings::disableAnimations("disableAnimations", true, false);
	bSetting				Settings::disableForActiOverride("disableForActiOverride", true, false);
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

	const char*				Settings::FILE_NAME = "Data\\SKSE\\Plugins\\QuickLootRE.json";
}
