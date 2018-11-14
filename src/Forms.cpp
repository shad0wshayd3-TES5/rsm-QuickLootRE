#include "Forms.h"

class BGSPerk;
class BGSKeyword;

namespace RE
{
	class TESFaction;
}


namespace QuickLootRE
{
	// Factions
	Form<RE::TESFaction>	CreatureFaction(kFACTFormID_CreatureFaction);
	Form<RE::TESFaction>	PredatorFaction(kFACTFormID_PredatorFaction);
	Form<RE::TESFaction>	PreyFaction(kFACTFormID_PreyFaction);
	Form<RE::TESFaction>	CurrentFollowerFaction(kFACTFormID_CurrentFollowerFaction);

	// Keywords
	Form<BGSKeyword>		VendorItemClothing(kKYWDFormID_VendorItemClothing);
	Form<BGSKeyword>		VendorItemJewelry(kKYWDFormID_VendorItemJewelry);

	Form<BGSKeyword>		VendorItemRecipe(kKYWDFormID_VendorItemRecipe);
	Form<BGSKeyword>		VendorItemSpellTome(kKYWDFormID_VendorItemSpellTome);

	Form<BGSKeyword>		VendorItemAnimalHide(kKYWDFormID_VendorItemAnimalHide);
	Form<BGSKeyword>		VendorItemDaedricArtifact(kKYWDFormID_VendorItemDaedricArtifact);
	Form<BGSKeyword>		VendorItemGem(kKYWDFormID_VendorItemGem);
	Form<BGSKeyword>		VendorItemAnimalPart(kKYWDFormID_VendorItemAnimalPart);
	Form<BGSKeyword>		VendorItemOreIngot(kKYWDFormID_VendorItemOreIngot);
	Form<BGSKeyword>		VendorItemClutter(kKYWDFormID_VendorItemClutter);
	Form<BGSKeyword>		VendorItemFireword(kKYWDFormID_VendorItemFireword);

	Form<BGSKeyword>		WeapTypeWarhammer(kKYWDFormID_WeapTypeWarhammer);

	// Perks
	Form<BGSPerk>			Misdirection(kPERKFormID_Misdirection);
	Form<BGSPerk>			PerfectTouch(kPERKFormID_PerfectTouch);
}
