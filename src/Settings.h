#pragma once

namespace Settings
{
	static REX::TOML::Bool closeInCombat{"General"sv, "closeInCombat"sv, true};
	static REX::TOML::Bool closeOnEmpty{"General"sv, "closeOnEmpty"sv, true};
	static REX::TOML::Bool dispelInvis{"General"sv, "dispelInvis"sv, true};
	static REX::TOML::Bool dispelEthereal{ "General"sv, "dispelEthereal"sv, true };

	static void Load()
	{
		auto toml = REX::TOML::SettingStore::GetSingleton();
		toml->Init(
			"Data/SKSE/Plugins/QuickLootRE/config.toml",
			"Data/SKSE/Plugins/QuickLootRE/configCustom.toml");
		toml->Load();
	}
}
