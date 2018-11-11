#include "Forms.h"

#include "skse64/GameForms.h"  // BGSKeyword, LookupFormByID

#include <vector>  // vector

#include "RE/TESFaction.h"  // TESFaction


namespace QuickLootRE
{
	bool initalizeForms()
	{
		std::vector<TESForm*> vec;

		// Factions
		vec.push_back(CreatureFaction			= static_cast<RE::TESFaction*>(LookupFormByID(kFACTFormID_CreatureFaction)));
		vec.push_back(CurrentFollowerFaction	= static_cast<RE::TESFaction*>(LookupFormByID(kFACTFormID_CurrentFollowerFaction)));

		// Keywords
		vec.push_back(VendorItemClothing		= static_cast<BGSKeyword*>(LookupFormByID(kKYWDFormID_VendorItemClothing)));
		vec.push_back(VendorItemJewelry			= static_cast<BGSKeyword*>(LookupFormByID(kKYWDFormID_VendorItemJewelry)));

		vec.push_back(VendorItemRecipe			= static_cast<BGSKeyword*>(LookupFormByID(kKYWDFormID_VendorItemRecipe)));
		vec.push_back(VendorItemSpellTome		= static_cast<BGSKeyword*>(LookupFormByID(kKYWDFormID_VendorItemSpellTome)));

		vec.push_back(VendorItemAnimalHide		= static_cast<BGSKeyword*>(LookupFormByID(kKYWDFormID_VendorItemAnimalHide)));
		vec.push_back(VendorItemDaedricArtifact	= static_cast<BGSKeyword*>(LookupFormByID(kKYWDFormID_VendorItemDaedricArtifact)));
		vec.push_back(VendorItemGem				= static_cast<BGSKeyword*>(LookupFormByID(kKYWDFormID_VendorItemGem)));
		vec.push_back(VendorItemAnimalPart		= static_cast<BGSKeyword*>(LookupFormByID(kKYWDFormID_VendorItemAnimalPart)));
		vec.push_back(VendorItemOreIngot		= static_cast<BGSKeyword*>(LookupFormByID(kKYWDFormID_VendorItemOreIngot)));
		vec.push_back(VendorItemClutter			= static_cast<BGSKeyword*>(LookupFormByID(kKYWDFormID_VendorItemClutter)));
		vec.push_back(VendorItemFireword		= static_cast<BGSKeyword*>(LookupFormByID(kKYWDFormID_VendorItemFireword)));

		vec.push_back(WeapTypeWarhammer			= static_cast<BGSKeyword*>(LookupFormByID(kKYWDFormID_WeapTypeWarhammer)));

		for (auto& form : vec) {
			if (!form) {
				return false;
			}
		}
		return true;
	}


	// Factions
	RE::TESFaction*	CreatureFaction = 0;
	RE::TESFaction*	CurrentFollowerFaction = 0;

	// Keywords
	BGSKeyword*		VendorItemClothing = 0;
	BGSKeyword*		VendorItemJewelry = 0;

	BGSKeyword*		VendorItemRecipe = 0;
	BGSKeyword*		VendorItemSpellTome = 0;

	BGSKeyword*		VendorItemAnimalHide = 0;
	BGSKeyword*		VendorItemDaedricArtifact = 0;
	BGSKeyword*		VendorItemGem = 0;
	BGSKeyword*		VendorItemAnimalPart = 0;
	BGSKeyword*		VendorItemOreIngot = 0;
	BGSKeyword*		VendorItemClutter = 0;
	BGSKeyword*		VendorItemFireword = 0;

	BGSKeyword*		WeapTypeWarhammer = 0;
}
