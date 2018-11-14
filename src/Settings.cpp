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
		json j;
		try {
			istream >> j;
			json::iterator it;
			for (auto& setting : settings) {
				it = j.find(setting->key());

				if (it == j.end()) {
					_ERROR("[ERROR] Failed to find (%s) within .json!", setting->key().c_str());
					continue;
				}

				switch (it->type()) {
				case json::value_t::array:
				{
					json jArr = it.value();
					setting->Assign(jArr);
					break;
				}
				case json::value_t::string:
				{
					std::string str = it.value();
					setting->Assign(str);
					break;
				}
				case json::value_t::boolean:
				{
					bool b = it.value();
					setting->Assign(b);
					break;
				}
				case json::value_t::number_integer:
				case json::value_t::number_unsigned:
				{
					int num = it.value();
					setting->Assign(num);
					break;
				}
				case json::value_t::number_float:
				{
					float num = it.value();
					setting->Assign(num);
					break;
				}
				default:
					_DMESSAGE("[ERROR] Parsed value is of invalid type (%s)!", j.type_name());
				}
			}
		} catch (std::exception& e) {
			_ERROR("[ERROR] Failed to parse json file!");
			_ERROR(e.what());
			istream.close();
			return false;
		}

		istream.close();
		return true;
	}


	void Settings::dump()
	{
		_DMESSAGE("=== SETTINGS DUMP BEGIN ===");
		for (auto& setting : settings) {
			setting->Dump();
		}
		_DMESSAGE("=== SETTINGS DUMP END ===");
	}


	bSetting	Settings::disableInCombat("disableInCombat", true);
	bSetting	Settings::disableTheft("disableTheft", false);
	bSetting	Settings::disablePickPocketing("disablePickpocketing", false);
	bSetting	Settings::disableIfEmpty("disableIfEmpty", true);
	bSetting	Settings::disableSingleLoot("disableSingleLoot", false);
	bSetting	Settings::playAnimations("playAnimations", true);
	bSetting	Settings::disableForAnimals("disableForAnimals", false);
	iSetting	Settings::itemLimit("itemLimit", 100);
	fSetting	Settings::scale("scale", -1.0);
	fSetting	Settings::positionX("positionX", -1.0);
	fSetting	Settings::positionY("positionY", -1.0);
	fSetting	Settings::opacity("opacity", -1.0);
	sSetting	Settings::takeAllMethod("takeAllMethod", "togglePOV");
	aSetting	Settings::sortOrder("sortOrder", { "stolen", "type", "name", "value", "count" });

	bool		Settings::isApplied = false;

	const char*	Settings::FILE_NAME = "Data\\SKSE\\Plugins\\QuickLootRE.json";
}
