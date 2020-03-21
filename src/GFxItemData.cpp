#include "GFxItemData.h"

#include <limits>

#include "Forms.h"
#include "Settings.h"
#include "Utility.h"


namespace
{
	int CompareMap::compareByName(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		return strcmp(a_rhs.GetName(), a_lhs.GetName());	// Intentionally flipped to ensure the list is sorted alphabetically
	}


	int CompareMap::r_compareByName(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		return -1 * compareByName(a_lhs, a_rhs);
	}


	int CompareMap::compareByCount(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		return a_lhs.GetCount() - a_rhs.GetCount();
	}


	int CompareMap::r_compareByCount(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		return -1 * compareByCount(a_lhs, a_rhs);
	}


	int CompareMap::compareByValue(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		return a_lhs.GetValue() - a_rhs.GetValue();
	}


	int CompareMap::r_compareByValue(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		return -1 * compareByValue(a_lhs, a_rhs);
	}


	int CompareMap::compareByWeight(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		float result = a_lhs.GetWeight() - a_rhs.GetWeight();
		if (result < -0.0001) {
			return -1;
		} else if (result > 0.0001) {
			return 1;
		} else {
			return 0;
		}
	}


	int CompareMap::r_compareByWeight(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		return -1 * compareByWeight(a_lhs, a_rhs);
	}


	int CompareMap::compareByType(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		return to_underlying(a_lhs._type) - to_underlying(a_rhs._type);
	}


	int CompareMap::r_compareByType(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		return -1 * compareByType(a_lhs, a_rhs);
	}


	int CompareMap::compareByRead(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		SInt32 valueLHS = a_lhs.GetIsRead() ? 1 : 0;
		SInt32 valueRHS = a_rhs.GetIsRead() ? 1 : 0;
		return valueLHS - valueRHS;
	}


	int CompareMap::r_compareByRead(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		return -1 * compareByRead(a_lhs, a_rhs);
	}


	int CompareMap::compareByEnchanted(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		SInt32 valueLHS = a_lhs.GetIsEnchanted() ? 1 : 0;
		SInt32 valueRHS = a_rhs.GetIsEnchanted() ? 1 : 0;
		return valueLHS - valueRHS;
	}


	int CompareMap::r_compareByEnchanted(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		return -1 * compareByEnchanted(a_lhs, a_rhs);
	}


	int CompareMap::compareByPickPocketChance(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		float result = a_lhs.GetPickPocketChance() - a_rhs.GetPickPocketChance();
		if (result < -0.0001) {
			return -1;
		} else if (result > 0.0001) {
			return 1;
		} else {
			return 0;
		}
	}


	int CompareMap::r_compareByPickPocketChance(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		return -1 * compareByPickPocketChance(a_lhs, a_rhs);
	}


	int CompareMap::compareByValuePerWeight(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		float leftVPW = a_lhs.GetWeight() ? a_lhs.GetValue() / a_lhs.GetWeight() : std::numeric_limits<float>::infinity();
		float rightVPW = a_rhs.GetWeight() ? a_rhs.GetValue() / a_rhs.GetWeight() : std::numeric_limits<float>::infinity();
		float result = leftVPW - rightVPW;
		if (result < -0.0001) {
			return -1;
		} else if (result > 0.0001) {
			return 1;
		} else {
			return 0;
		}
	}


	int CompareMap::r_compareByValuePerWeight(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		return -1 * compareByValuePerWeight(a_lhs, a_rhs);
	}


	int CompareMap::compareByPriority(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		return to_underlying(a_rhs._priority) - to_underlying(a_lhs._priority);  // Lower numbers have higher priority
	}


	int CompareMap::r_compareByPriority(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
	{
		return -1 * compareByPriority(a_lhs, a_rhs);
	}


	CompareMap::CompareMap()
	{
		std::string key;

		key = "name";
		insert({ key, compareByName });
		insert({ "r_" + key, r_compareByName });

		key = "count";
		insert({ key, compareByCount });
		insert({ "r_" + key, r_compareByCount });

		key = "value";
		insert({ key, compareByValue });
		insert({ "r_" + key, r_compareByValue });

		key = "weight";
		insert({ key, compareByWeight });
		insert({ "r_" + key, r_compareByWeight });

		key = "type";
		insert({ key, compareByType });
		insert({ "r_" + key, r_compareByType });

		key = "read";
		insert({ key, compareByRead });
		insert({ "r_" + key, r_compareByRead });

		key = "enchanted";
		insert({ key, compareByEnchanted });
		insert({ "r_" + key, r_compareByEnchanted });

		key = "pickPocketChance";
		insert({ key, compareByPickPocketChance });
		insert({ "r_" + key, r_compareByPickPocketChance });

		key = "valuePerWeight";
		insert({ key, compareByValuePerWeight });
		insert({ "r_" + key, r_compareByValuePerWeight });

		key = "priority";
		insert({ key, compareByPriority });
		insert({ "r_" + key, r_compareByPriority });
	}
}


GFxItemData::GFxItemData(ItemWrapper&& a_item, SInt32 a_count) :
	_itemImpl(std::move(a_item)),
	_name(""),
	_count(a_count),
	_value(0),
	_weight(0.0),
	_type(Type::kNone),
	_isStolen(false),
	_isRead(false),
	_isEnchanted(false),
	_canPickPocket(true),
	_pickPocketChance(-1),
	_priority(Priority::kKey)
{
	Init();
}


void GFxItemData::SetCompareOrder()
{
	_compares.clear();
	CompareMap compMap;
	for (auto& comp : Settings::sortOrder) {
		auto it = compMap.find(comp);
		if (it != compMap.end()) {
			_DMESSAGE("Added compare by %s", comp.c_str());
			_compares.push_back(it->second);
		} else {
			_ERROR("Encountered unknown compare (%s)!\n", comp.c_str());
		}
	}
}


void GFxItemData::SetContainer(RE::TESObjectREFR* a_container)
{
	_container = a_container;
}


bool operator==(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
{
	return (a_lhs._itemImpl.GetObject() == a_rhs._itemImpl.GetObject() &&
			a_lhs._name == a_rhs._name &&
			a_lhs._count == a_rhs._count &&
			a_lhs._value == a_rhs._value &&
			a_lhs._weight == a_rhs._weight &&
			a_lhs._type == a_rhs._type &&
			a_lhs._isStolen == a_rhs._isStolen &&
			a_lhs._isEnchanted == a_rhs._isEnchanted &&
			a_lhs._pickPocketChance == a_rhs._pickPocketChance &&
			a_lhs._priority == a_rhs._priority);
}


bool operator!=(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
{
	return !operator==(a_lhs, a_rhs);
}


bool operator<(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
{
	if (a_lhs.GetCanPickPocket() != a_rhs.GetCanPickPocket()) {
		return a_rhs.GetCanPickPocket();  // Ensures items that can't be pickpocketed sort to the end
	}

	for (auto& compare : GFxItemData::_compares) {
		auto cmp = compare(a_lhs, a_rhs);
		if (cmp) {
			return cmp < 0;
		}
	}

	return false;
}


bool operator>(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
{
	return operator<(a_rhs, a_lhs);
}


bool operator<=(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
{
	return !operator>(a_lhs, a_rhs);
}


bool operator>=(const GFxItemData& a_lhs, const GFxItemData& a_rhs)
{
	return !operator<(a_lhs, a_rhs);
}


const char* GFxItemData::GetName() const
{
	return _name;
}


SInt32 GFxItemData::GetCount() const
{
	return _count;
}


SInt32 GFxItemData::GetValue() const
{
	return _value;
}


float GFxItemData::GetWeight() const
{
	return _weight;
}


const char* GFxItemData::GetIcon() const
{
	return _strIcons[to_underlying(_type)];
}


bool GFxItemData::GetIsStolen() const
{
	return _isStolen;
}


bool GFxItemData::GetIsRead() const
{
	return _isRead;
}


bool GFxItemData::GetIsEnchanted() const
{
	return _isEnchanted;
}


SInt32 GFxItemData::GetPickPocketChance() const
{
	return _pickPocketChance;
}


bool GFxItemData::GetCanPickPocket() const
{
	return _canPickPocket;
}


void GFxItemData::ModCount(SInt32 a_mod)
{
	_count += a_mod;
}


void GFxItemData::DBGDumpType(std::size_t a_index)
{
	switch (ITEM_DATA_DEBUG_TYPE) {
	case kDebugType_Name:
		SKSE::Logger::VPrint("(%s) name == (%s: %zu)", GetName(), GetName(), a_index);
		break;
	case kDebugType_Count:
		SKSE::Logger::VPrint("(%i) count == (%s: %zu)", GetCount(), GetName(), a_index);
		break;
	case kDebugType_Value:
		SKSE::Logger::VPrint("(%i) value == (%s: %zu)", GetValue(), GetName(), a_index);
		break;
	case kDebugType_Weight:
		SKSE::Logger::VPrint("(%F) weight == (%s: %zu)", GetWeight(), GetName(), a_index);
		break;
	case kDebugType_Type:
		SKSE::Logger::VPrint("(%u) type == (%s: %zu)", _type, GetName(), a_index);
		break;
	case kDebugType_Read:
		SKSE::Logger::VPrint("(%s) read == (%s: %zu)", boolToString(GetIsRead()), GetName(), a_index);
		break;
	case kDebugType_Enchanted:
		SKSE::Logger::VPrint("(%s) enchanted == (%s: %zu)", boolToString(GetIsEnchanted()), GetName(), a_index);
		break;
	case kDebugType_PickPocketChance:
		SKSE::Logger::VPrint("(%i) pickPocketChance == (%s: %zu)", GetPickPocketChance(), GetName(), a_index);
		break;
	case kDebugType_ValuePerWeight:
		{
			float vpw = GetWeight() ? GetValue() / GetWeight() : std::numeric_limits<float>::infinity();
			SKSE::Logger::VPrint("(%F) valuePerWeight == (%s: %zu)", vpw, GetName(), a_index);
			break;
		}
	case kDebugType_Priority:
		SKSE::Logger::VPrint("(%u) priority == (%s: %zu)", _priority, GetName(), a_index);
		break;
	}
}


void GFxItemData::Init()
{
	_name = CalcName();
	_value = CalcValue();
	_weight = CalcWeight();
	_isRead = CalcRead();	// must come before type
	_type = CalcType();
	_isStolen = CalcStolen();
	_isEnchanted = CalcEnchanted();
	_canPickPocket = CalcCanPickPocket();
	_pickPocketChance = CalcPickPocketChance();	// must come after weight
	_priority = CalcPriority();
}


bool GFxItemData::CalcCanPickPocket()
{
	if (_itemImpl.IsNot(RE::FormType::NPC)) {
		return true;
	}

	auto actor = static_cast<RE::Actor*>(_container);
	if (actor->IsDead()) {
		return true;
	}

	if (_itemImpl.IsWorn()) {
		auto player = RE::PlayerCharacter::GetSingleton();
		if (_itemImpl.Is(RE::FormType::Weapon)) {
			return player->HasPerk(Misdirection);
		} else {
			return player->HasPerk(PerfectTouch);
		}
	}

	return true;
}


bool GFxItemData::CalcEnchanted()
{
	if (_itemImpl.HasExtraEnchantment()) {
		return true;
	}

	auto enchantForm = _itemImpl.GetObjectAs<RE::TESEnchantableForm>();
	return enchantForm && enchantForm->formEnchanting;
}


const char* GFxItemData::CalcName()
{
	return _itemImpl.GetName();
}


SInt32 GFxItemData::CalcPickPocketChance()
{
	auto player = RE::PlayerCharacter::GetSingleton();
	if (IsValidPickPocketTarget(_container, player->IsSneaking())) {
		auto targetActor = static_cast<RE::Actor*>(_container);

		auto totalValue = _itemImpl.GetValue() * GetCount();
		auto isDetected = targetActor->GetDetectionLevel(player) > 0;
		auto playerSkill = player->GetPlayerActorValueCurrent(RE::ActorValue::kPickpocket);
		auto targetSkill = targetActor->GetActorValueCurrent(RE::ActorValue::kPickpocket);

		auto chance = RE::PlayerCharacter::GetPickpocketChance(playerSkill, targetSkill, totalValue, _weight, player, targetActor, isDetected, _itemImpl.GetObject());
		if (chance > 100) {
			chance = 100;
		} else if (chance < 0) {
			chance = 0;
		}

		return chance;
	} else {
		return -1;
	}
}


auto GFxItemData::CalcPriority()
-> Priority
{
	switch (_itemImpl.GetFormType()) {
	case RE::FormType::Ammo:
		return Priority::kAmmo;
	case RE::FormType::SoulGem:
		return Priority::kSoulGem;
	case RE::FormType::AlchemyItem:
		switch (_type) {
		case Type::kDefaultFood:
		case Type::kFoodWine:
		case Type::kFoodBeer:
			return Priority::kFood;
		case Type::kPotionPoison:
			return Priority::kPoison;
		default:
			return Priority::kPotion;
		}
	case RE::FormType::Weapon:
		return (GetIsEnchanted()) ? Priority::kEnchantedWeapon : Priority::kWeapon;
	case RE::FormType::Armor:
		if (GetIsEnchanted()) {
			return Priority::kEnchantedArmor;
		} else if (_type == Type::kArmorAmulet) {
			return Priority::kAmulet;
		} else if (_type == Type::kArmorRing) {
			return Priority::kRing;
		} else {
			return Priority::kArmor;
		}
	case RE::FormType::KeyMaster:
		return Priority::kKey;
	case RE::FormType::Misc:
		switch (_type) {
		case Type::kMiscGold:
			return Priority::kGold;
		case Type::kMiscLockPick:
			return Priority::kLockPick;
		case Type::kMiscGem:
			return Priority::kGem;
		default:
			return Priority::kOther;
		}
	default:
		return Priority::kOther;
	}
}


bool GFxItemData::CalcRead()
{
	auto book = _itemImpl.GetObjectAs<RE::TESObjectBOOK>();
	return book ? book->IsRead() : false;
}


bool GFxItemData::CalcStolen()
{
	auto owner = _itemImpl.GetOwner();
	if (!owner) {
		owner = _container->GetOwner();
	}

	auto player = RE::PlayerCharacter::GetSingleton();
	if (owner) {
		return !_itemImpl.IsOwnedBy(player, owner);
	} else {
		return player->IsSneaking() && _container->GetBaseObject()->Is(RE::FormType::NPC) && !_container->IsDead(true);
	}
}


auto GFxItemData::CalcType()
-> Type
{
	switch (_itemImpl.GetFormType()) {
	case RE::FormType::Scroll:
		return Type::kDefaultScroll;
	case RE::FormType::Armor:
		return CalcTypeArmor(_itemImpl.GetObject<RE::TESObjectARMO>());
	case RE::FormType::Book:
		return CalcTypeBook(_itemImpl.GetObject<RE::TESObjectBOOK>());
	case RE::FormType::Ingredient:
		return Type::kDefaultIngredient;
	case RE::FormType::Light:
		return Type::kMiscTorch;
	case RE::FormType::Misc:
		return CalcTypeMisc(_itemImpl.GetObject<RE::TESObjectMISC>());
	case RE::FormType::Weapon:
		return CalcTypeWeapon(_itemImpl.GetObject<RE::TESObjectWEAP>());
	case RE::FormType::Ammo:
		return (_itemImpl.GetObject<RE::TESAmmo>()->IsBolt()) ? Type::kWeaponBolt : Type::kWeaponArrow;
	case RE::FormType::KeyMaster:
		return CalcTypeKey(_itemImpl.GetObject<RE::TESKey>());
	case RE::FormType::AlchemyItem:
		return CalcTypePotion(_itemImpl.GetObject<RE::AlchemyItem>());
	case RE::FormType::SoulGem:
		return CalcTypeSoulGem(_itemImpl.GetObject<RE::TESSoulGem>());
	default:
		return Type::kNone;
	}
}


auto GFxItemData::CalcTypeArmor(RE::TESObjectARMO* a_armor)
-> Type
{
	using ArmorType = RE::BGSBipedObjectForm::ArmorType;
	using Flag = RE::BIPED_MODEL::BipedObjectSlot;

	static Type types[] = {
		Type::kLightArmorBody,		// 0
		Type::kLightArmorHead,
		Type::kLightArmorHands,
		Type::kLightArmorForearms,
		Type::kLightArmorFeet,
		Type::kLightArmorCalves,
		Type::kLightArmorShield,
		Type::kLightArmorMask,

		Type::kArmorBody,			// 8
		Type::kArmorHead,
		Type::kArmorHands,
		Type::kArmorForearms,
		Type::kArmorFeet,
		Type::kArmorCalves,
		Type::kArmorShield,
		Type::kArmorMask,

		Type::kClothingBody,		// 16
		Type::kClothingHead,
		Type::kClothingHands,
		Type::kClothingForearms,
		Type::kClothingFeet,
		Type::kClothingCalves,
		Type::kClothingShield,
		Type::kClothingMask
	};

	auto special = CalcTypeArmorSpecial(a_armor->GetFormID());
	if (special != Type::kInvalid) {
		return special;
	}

	std::size_t index = 0;
	auto bipedObj = static_cast<RE::BGSBipedObjectForm*>(a_armor);

	switch (bipedObj->bipedModelData.armorType) {
	case ArmorType::kLightArmor:
		index = 0;
		break;
	case ArmorType::kHeavyArmor:
		index = 8;
		break;
	default:
		index = 16;
		break;
	}

	if (a_armor->HasKeyword(VendorItemJewelry)) {
		if (bipedObj->HasPartOf(Flag::kAmulet)) {
			return Type::kArmorAmulet;
		} else if (bipedObj->HasPartOf(Flag::kRing)) {
			return Type::kArmorRing;
		} else if (bipedObj->HasPartOf(Flag::kCirclet)) {
			return Type::kCirclet;
		} else {
			return Type::kDefaultArmor;
		}
	}

	if (bipedObj->HasPartOf(Flag::kBody | Flag::kTail)) {
		index += 0;
	} else if (bipedObj->HasPartOf(Flag::kHead | Flag::kHair | Flag::kLongHair)) {
		index += 1;
	} else if (bipedObj->HasPartOf(Flag::kHands)) {
		index += 2;
	} else if (bipedObj->HasPartOf(Flag::kForearms)) {
		index += 3;
	} else if (bipedObj->HasPartOf(Flag::kFeet)) {
		index += 4;
	} else if (bipedObj->HasPartOf(Flag::kCalves)) {
		index += 5;
	} else if (bipedObj->HasPartOf(Flag::kShield)) {
		if (a_armor->HasKeyword(ArmorMaterialDaedric)) {
			return Type::kArmorDaedra;
		} else {
			index += 6;
		}
	} else {
		return Type::kDefaultArmor;
	}

	return types[index];
}


auto GFxItemData::CalcTypeArmorSpecial(RE::FormID a_formID)
-> Type
{
	switch (a_formID) {
	case kArmorBanditGauntlets:
	case kArmorHideGauntlets:
	case kArmorImperialGauntlets:
	case kArmorImperialLightGauntlets:
	case kArmorLeatherGauntlets:
	case kArmorPenitusGauntlets:
	case kArmorScaledGauntlets:
	case kArmorStormcloakBearGauntlets:
	case kClothesUlfricGauntlets:
	case kEnchArmorDwarvenGauntletsAlchemy02:
	case kEnchArmorDwarvenGauntletsAlchemy03:
	case kEnchArmorDwarvenGauntletsAlchemy04:
	case kEnchArmorDwarvenGauntletsMarksman02:
	case kEnchArmorDwarvenGauntletsMarksman03:
	case kEnchArmorDwarvenGauntletsMarksman04:
	case kEnchArmorDwarvenGauntletsOneHanded02:
	case kEnchArmorDwarvenGauntletsOneHanded03:
	case kEnchArmorDwarvenGauntletsOneHanded04:
	case kEnchArmorDwarvenGauntletsSmithing02:
	case kEnchArmorDwarvenGauntletsSmithing03:
	case kEnchArmorDwarvenGauntletsSmithing04:
	case kEnchArmorDwarvenGauntletsTwoHanded02:
	case kEnchArmorDwarvenGauntletsTwoHanded03:
	case kEnchArmorDwarvenGauntletsTwoHanded04:
	case kEnchArmorElvenGauntletsAlchemy02:
	case kEnchArmorElvenGauntletsAlchemy03:
	case kEnchArmorElvenGauntletsAlchemy04:
	case kEnchArmorElvenGauntletsLockpicking02:
	case kEnchArmorElvenGauntletsLockpicking03:
	case kEnchArmorElvenGauntletsLockpicking04:
	case kEnchArmorElvenGauntletsMarksman02:
	case kEnchArmorElvenGauntletsMarksman03:
	case kEnchArmorElvenGauntletsMarksman04:
	case kEnchArmorElvenGauntletsOneHanded02:
	case kEnchArmorElvenGauntletsOneHanded03:
	case kEnchArmorElvenGauntletsOneHanded04:
	case kEnchArmorElvenGauntletsPickpocket02:
	case kEnchArmorElvenGauntletsPickpocket03:
	case kEnchArmorElvenGauntletsPickpocket04:
	case kEnchArmorElvenGauntletsSmithing02:
	case kEnchArmorElvenGauntletsSmithing03:
	case kEnchArmorElvenGauntletsSmithing04:
	case kEnchArmorElvenGauntletsTwoHanded02:
	case kEnchArmorElvenGauntletsTwoHanded03:
	case kEnchArmorElvenGauntletsTwoHanded04:
	case kEnchArmorHideGauntletsAlchemy01:
	case kEnchArmorHideGauntletsAlchemy02:
	case kEnchArmorHideGauntletsAlchemy03:
	case kEnchArmorHideGauntletsLockpicking01:
	case kEnchArmorHideGauntletsLockpicking02:
	case kEnchArmorHideGauntletsLockpicking03:
	case kEnchArmorHideGauntletsMarksman01:
	case kEnchArmorHideGauntletsMarksman02:
	case kEnchArmorHideGauntletsMarksman03:
	case kEnchArmorHideGauntletsOneHanded01:
	case kEnchArmorHideGauntletsOneHanded02:
	case kEnchArmorHideGauntletsOneHanded03:
	case kEnchArmorHideGauntletsPickpocket01:
	case kEnchArmorHideGauntletsPickpocket02:
	case kEnchArmorHideGauntletsPickpocket03:
	case kEnchArmorHideGauntletsSmithing01:
	case kEnchArmorHideGauntletsSmithing02:
	case kEnchArmorHideGauntletsSmithing03:
	case kEnchArmorHideGauntletsTwoHanded01:
	case kEnchArmorHideGauntletsTwoHanded02:
	case kEnchArmorHideGauntletsTwoHanded03:
	case kEnchArmorImperialGauntletsAlchemy01:
	case kEnchArmorImperialGauntletsAlchemy02:
	case kEnchArmorImperialGauntletsAlchemy03:
	case kEnchArmorImperialGauntletsAlteration01:
	case kEnchArmorImperialGauntletsAlteration02:
	case kEnchArmorImperialGauntletsAlteration03:
	case kEnchArmorImperialGauntletsConjuration01:
	case kEnchArmorImperialGauntletsConjuration02:
	case kEnchArmorImperialGauntletsConjuration03:
	case kEnchArmorImperialGauntletsDestruction01:
	case kEnchArmorImperialGauntletsDestruction02:
	case kEnchArmorImperialGauntletsDestruction03:
	case kEnchArmorImperialGauntletsMarksman01:
	case kEnchArmorImperialGauntletsMarksman02:
	case kEnchArmorImperialGauntletsMarksman03:
	case kEnchArmorImperialGauntletsOneHanded01:
	case kEnchArmorImperialGauntletsOneHanded02:
	case kEnchArmorImperialGauntletsOneHanded03:
	case kEnchArmorImperialGauntletsSmithing01:
	case kEnchArmorImperialGauntletsSmithing02:
	case kEnchArmorImperialGauntletsSmithing03:
	case kEnchArmorImperialGauntletsTwoHanded01:
	case kEnchArmorImperialGauntletsTwoHanded02:
	case kEnchArmorImperialGauntletsTwoHanded03:
	case kEnchArmorImperialLightGauntletsAlchemy01:
	case kEnchArmorImperialLightGauntletsAlchemy02:
	case kEnchArmorImperialLightGauntletsAlchemy03:
	case kEnchArmorImperialLightGauntletsAlteration01:
	case kEnchArmorImperialLightGauntletsAlteration02:
	case kEnchArmorImperialLightGauntletsAlteration03:
	case kEnchArmorImperialLightGauntletsConjuration01:
	case kEnchArmorImperialLightGauntletsConjuration02:
	case kEnchArmorImperialLightGauntletsConjuration03:
	case kEnchArmorImperialLightGauntletsDestruction01:
	case kEnchArmorImperialLightGauntletsDestruction02:
	case kEnchArmorImperialLightGauntletsDestruction03:
	case kEnchArmorImperialLightGauntletsLockpicking01:
	case kEnchArmorImperialLightGauntletsLockpicking02:
	case kEnchArmorImperialLightGauntletsLockpicking03:
	case kEnchArmorImperialLightGauntletsMarksman01:
	case kEnchArmorImperialLightGauntletsMarksman02:
	case kEnchArmorImperialLightGauntletsMarksman03:
	case kEnchArmorImperialLightGauntletsOneHanded01:
	case kEnchArmorImperialLightGauntletsOneHanded02:
	case kEnchArmorImperialLightGauntletsOneHanded03:
	case kEnchArmorImperialLightGauntletsPickpocket01:
	case kEnchArmorImperialLightGauntletsPickpocket02:
	case kEnchArmorImperialLightGauntletsPickpocket03:
	case kEnchArmorImperialLightGauntletsSmithing01:
	case kEnchArmorImperialLightGauntletsSmithing02:
	case kEnchArmorImperialLightGauntletsSmithing03:
	case kEnchArmorImperialLightGauntletsTwoHanded01:
	case kEnchArmorImperialLightGauntletsTwoHanded02:
	case kEnchArmorImperialLightGauntletsTwoHanded03:
	case kEnchArmorLeatherGauntletsAlchemy01:
	case kEnchArmorLeatherGauntletsAlchemy02:
	case kEnchArmorLeatherGauntletsAlchemy03:
	case kEnchArmorLeatherGauntletsLockpicking01:
	case kEnchArmorLeatherGauntletsLockpicking02:
	case kEnchArmorLeatherGauntletsLockpicking03:
	case kEnchArmorLeatherGauntletsMarksman01:
	case kEnchArmorLeatherGauntletsMarksman02:
	case kEnchArmorLeatherGauntletsMarksman03:
	case kEnchArmorLeatherGauntletsOneHanded01:
	case kEnchArmorLeatherGauntletsOneHanded02:
	case kEnchArmorLeatherGauntletsOneHanded03:
	case kEnchArmorLeatherGauntletsPickpocket01:
	case kEnchArmorLeatherGauntletsPickpocket02:
	case kEnchArmorLeatherGauntletsPickpocket03:
	case kEnchArmorLeatherGauntletsSmithing01:
	case kEnchArmorLeatherGauntletsSmithing02:
	case kEnchArmorLeatherGauntletsSmithing03:
	case kEnchArmorLeatherGauntletsTwoHanded01:
	case kEnchArmorLeatherGauntletsTwoHanded02:
	case kEnchArmorLeatherGauntletsTwoHanded03:
	case kEnchArmorScaledGauntletsAlchemy02:
	case kEnchArmorScaledGauntletsAlchemy03:
	case kEnchArmorScaledGauntletsAlchemy04:
	case kEnchArmorScaledGauntletsLockpicking02:
	case kEnchArmorScaledGauntletsLockpicking03:
	case kEnchArmorScaledGauntletsLockpicking04:
	case kEnchArmorScaledGauntletsMarksman02:
	case kEnchArmorScaledGauntletsMarksman03:
	case kEnchArmorScaledGauntletsMarksman04:
	case kEnchArmorScaledGauntletsOneHanded02:
	case kEnchArmorScaledGauntletsOneHanded03:
	case kEnchArmorScaledGauntletsOneHanded04:
	case kEnchArmorScaledGauntletsPickpocket02:
	case kEnchArmorScaledGauntletsPickpocket03:
	case kEnchArmorScaledGauntletsPickpocket04:
	case kEnchArmorScaledGauntletsSmithing02:
	case kEnchArmorScaledGauntletsSmithing03:
	case kEnchArmorScaledGauntletsSmithing04:
	case kEnchArmorScaledGauntletsTwoHanded02:
	case kEnchArmorScaledGauntletsTwoHanded03:
	case kEnchArmorScaledGauntletsTwoHanded04:
	case kEnchArmorSteelPlateGauntletsAlchemy02:
	case kEnchArmorSteelPlateGauntletsAlchemy03:
	case kEnchArmorSteelPlateGauntletsAlchemy04:
	case kEnchArmorSteelPlateGauntletsMarksman02:
	case kEnchArmorSteelPlateGauntletsMarksman03:
	case kEnchArmorSteelPlateGauntletsMarksman04:
	case kEnchArmorSteelPlateGauntletsOneHanded02:
	case kEnchArmorSteelPlateGauntletsOneHanded03:
	case kEnchArmorSteelPlateGauntletsOneHanded04:
	case kEnchArmorSteelPlateGauntletsSmithing02:
	case kEnchArmorSteelPlateGauntletsSmithing03:
	case kEnchArmorSteelPlateGauntletsSmithing04:
	case kEnchArmorSteelPlateGauntletsTwoHanded02:
	case kEnchArmorSteelPlateGauntletsTwoHanded03:
	case kEnchArmorSteelPlateGauntletsTwoHanded04:
		return Type::kArmorBracer;
	case kClothesBeggarRobes:
	case kClothesCollegeRobesApprentice:
	case kClothesCollegeRobesApprenticeVariant1:
	case kClothesCollegeRobesApprenticeVariant2:
	case kClothesCollegeRobesCommon:
	case kClothesCollegeRobesCommonVariant1:
	case kClothesEmperor:
	case kClothesExecutionerRobes:
	case kClothesJarl_var1:
	case kClothesMGRobesArchmage:
	case kClothesMGRobesArchmage1Hooded:
	case kClothesMonkRobes:
	case kClothesMonkRobesColorBrown:
	case kClothesMonkRobesColorBrownHooded:
	case kClothesMonkRobesColorGreen:
	case kClothesMonkRobesColorGreenHooded:
	case kClothesMonkRobesColorGrey:
	case kClothesMonkRobesColorGreyHooded:
	case kClothesMonkRobesColorRed:
	case kClothesMonkRobesColorRedHooded:
	case kClothesMonkRobesHooded:
	case kClothesMythicDawnRobes:
	case kClothesMythicDawnRobesNoHood:
	case kClothesNecromancerRobes:
	case kClothesNecromancerRobesHooded:
	case kClothesPsiijicRobes:
	case kClothesRobesBlack:
	case kClothesRobesBlackHooded:
	case kClothesRobesBlue:
	case kClothesRobesBlueHooded:
	case kClothesRobesGreybeardTunic:
	case kClothesRobesMageNoviceTemplate:
	case kClothesThalmorRobesHooded:
	case kClothesThalmorrobes:
	case kClothesWarlockRobes:
	case kClothesWarlockRobesHooded:
	case kDA16VaerminaRobes:
	case kDBClothesRobes:
	case kDremoraRobesBlack:
	case kEnchClothesMageRobesAppMagickaRate01:
	case kEnchClothesNecroRobesAlteration01:
	case kEnchClothesNecroRobesAlteration02:
	case kEnchClothesNecroRobesAlteration03:
	case kEnchClothesNecroRobesAlteration04:
	case kEnchClothesNecroRobesAlteration05:
	case kEnchClothesNecroRobesAlteration06:
	case kEnchClothesNecroRobesConjuration01:
	case kEnchClothesNecroRobesConjuration02:
	case kEnchClothesNecroRobesConjuration03:
	case kEnchClothesNecroRobesConjuration04:
	case kEnchClothesNecroRobesConjuration05:
	case kEnchClothesNecroRobesConjuration06:
	case kEnchClothesNecroRobesDestruction01:
	case kEnchClothesNecroRobesDestruction02:
	case kEnchClothesNecroRobesDestruction03:
	case kEnchClothesNecroRobesDestruction04:
	case kEnchClothesNecroRobesDestruction05:
	case kEnchClothesNecroRobesDestruction06:
	case kEnchClothesNecroRobesHoodedAlteration01:
	case kEnchClothesNecroRobesHoodedAlteration02:
	case kEnchClothesNecroRobesHoodedAlteration03:
	case kEnchClothesNecroRobesHoodedAlteration04:
	case kEnchClothesNecroRobesHoodedAlteration05:
	case kEnchClothesNecroRobesHoodedAlteration06:
	case kEnchClothesNecroRobesHoodedConjuration01:
	case kEnchClothesNecroRobesHoodedConjuration02:
	case kEnchClothesNecroRobesHoodedConjuration03:
	case kEnchClothesNecroRobesHoodedConjuration04:
	case kEnchClothesNecroRobesHoodedConjuration05:
	case kEnchClothesNecroRobesHoodedConjuration06:
	case kEnchClothesNecroRobesHoodedDestruction01:
	case kEnchClothesNecroRobesHoodedDestruction02:
	case kEnchClothesNecroRobesHoodedDestruction03:
	case kEnchClothesNecroRobesHoodedDestruction04:
	case kEnchClothesNecroRobesHoodedDestruction05:
	case kEnchClothesNecroRobesHoodedDestruction06:
	case kEnchClothesNecroRobesHoodedIllusion01:
	case kEnchClothesNecroRobesHoodedIllusion02:
	case kEnchClothesNecroRobesHoodedIllusion03:
	case kEnchClothesNecroRobesHoodedIllusion04:
	case kEnchClothesNecroRobesHoodedIllusion05:
	case kEnchClothesNecroRobesHoodedIllusion06:
	case kEnchClothesNecroRobesHoodedMagickaRate02:
	case kEnchClothesNecroRobesHoodedMagickaRate03:
	case kEnchClothesNecroRobesHoodedMagickaRate04:
	case kEnchClothesNecroRobesHoodedMagickaRate05:
	case kEnchClothesNecroRobesHoodedRestoration01:
	case kEnchClothesNecroRobesHoodedRestoration02:
	case kEnchClothesNecroRobesHoodedRestoration03:
	case kEnchClothesNecroRobesHoodedRestoration04:
	case kEnchClothesNecroRobesHoodedRestoration05:
	case kEnchClothesNecroRobesHoodedRestoration06:
	case kEnchClothesNecroRobesIllusion01:
	case kEnchClothesNecroRobesIllusion02:
	case kEnchClothesNecroRobesIllusion03:
	case kEnchClothesNecroRobesIllusion04:
	case kEnchClothesNecroRobesIllusion05:
	case kEnchClothesNecroRobesIllusion06:
	case kEnchClothesNecroRobesMagickaRate02:
	case kEnchClothesNecroRobesMagickaRate03:
	case kEnchClothesNecroRobesMagickaRate04:
	case kEnchClothesNecroRobesMagickaRate05:
	case kEnchClothesNecroRobesRestoration01:
	case kEnchClothesNecroRobesRestoration02:
	case kEnchClothesNecroRobesRestoration03:
	case kEnchClothesNecroRobesRestoration04:
	case kEnchClothesNecroRobesRestoration05:
	case kEnchClothesNecroRobesRestoration06:
	case kEnchClothesRobesMageAlteration01:
	case kEnchClothesRobesMageAlteration02:
	case kEnchClothesRobesMageAlteration03:
	case kEnchClothesRobesMageAlteration04:
	case kEnchClothesRobesMageAlteration05:
	case kEnchClothesRobesMageConjuration01:
	case kEnchClothesRobesMageConjuration02:
	case kEnchClothesRobesMageConjuration03:
	case kEnchClothesRobesMageConjuration04:
	case kEnchClothesRobesMageConjuration05:
	case kEnchClothesRobesMageDestruction01:
	case kEnchClothesRobesMageDestruction02:
	case kEnchClothesRobesMageDestruction03:
	case kEnchClothesRobesMageDestruction04:
	case kEnchClothesRobesMageDestruction05:
	case kEnchClothesRobesMageIllusion01:
	case kEnchClothesRobesMageIllusion02:
	case kEnchClothesRobesMageIllusion03:
	case kEnchClothesRobesMageIllusion04:
	case kEnchClothesRobesMageIllusion05:
	case kEnchClothesRobesMageRegen01:
	case kEnchClothesRobesMageRegen02:
	case kEnchClothesRobesMageRegen03:
	case kEnchClothesRobesMageRegen04:
	case kEnchClothesRobesMageRegen05:
	case kEnchClothesRobesMageRestoration01:
	case kEnchClothesRobesMageRestoration02:
	case kEnchClothesRobesMageRestoration03:
	case kEnchClothesRobesMageRestoration04:
	case kEnchClothesRobesMageRestoration05:
	case kEnchClothesWarlockRobesAlteration01:
	case kEnchClothesWarlockRobesAlteration02:
	case kEnchClothesWarlockRobesAlteration03:
	case kEnchClothesWarlockRobesAlteration04:
	case kEnchClothesWarlockRobesAlteration05:
	case kEnchClothesWarlockRobesAlteration06:
	case kEnchClothesWarlockRobesConjuration01:
	case kEnchClothesWarlockRobesConjuration02:
	case kEnchClothesWarlockRobesConjuration03:
	case kEnchClothesWarlockRobesConjuration04:
	case kEnchClothesWarlockRobesConjuration05:
	case kEnchClothesWarlockRobesConjuration06:
	case kEnchClothesWarlockRobesDestruction01:
	case kEnchClothesWarlockRobesDestruction02:
	case kEnchClothesWarlockRobesDestruction03:
	case kEnchClothesWarlockRobesDestruction04:
	case kEnchClothesWarlockRobesDestruction05:
	case kEnchClothesWarlockRobesDestruction06:
	case kEnchClothesWarlockRobesHoodedAlteration01:
	case kEnchClothesWarlockRobesHoodedAlteration02:
	case kEnchClothesWarlockRobesHoodedAlteration03:
	case kEnchClothesWarlockRobesHoodedAlteration04:
	case kEnchClothesWarlockRobesHoodedAlteration05:
	case kEnchClothesWarlockRobesHoodedAlteration06:
	case kEnchClothesWarlockRobesHoodedConjuration01:
	case kEnchClothesWarlockRobesHoodedConjuration02:
	case kEnchClothesWarlockRobesHoodedConjuration03:
	case kEnchClothesWarlockRobesHoodedConjuration04:
	case kEnchClothesWarlockRobesHoodedConjuration05:
	case kEnchClothesWarlockRobesHoodedConjuration06:
	case kEnchClothesWarlockRobesHoodedDestruction01:
	case kEnchClothesWarlockRobesHoodedDestruction02:
	case kEnchClothesWarlockRobesHoodedDestruction03:
	case kEnchClothesWarlockRobesHoodedDestruction04:
	case kEnchClothesWarlockRobesHoodedDestruction05:
	case kEnchClothesWarlockRobesHoodedDestruction06:
	case kEnchClothesWarlockRobesHoodedIllusion01:
	case kEnchClothesWarlockRobesHoodedIllusion02:
	case kEnchClothesWarlockRobesHoodedIllusion03:
	case kEnchClothesWarlockRobesHoodedIllusion04:
	case kEnchClothesWarlockRobesHoodedIllusion05:
	case kEnchClothesWarlockRobesHoodedIllusion06:
	case kEnchClothesWarlockRobesHoodedMagickaRate02:
	case kEnchClothesWarlockRobesHoodedMagickaRate03:
	case kEnchClothesWarlockRobesHoodedMagickaRate04:
	case kEnchClothesWarlockRobesHoodedMagickaRate05:
	case kEnchClothesWarlockRobesHoodedRestoration01:
	case kEnchClothesWarlockRobesHoodedRestoration02:
	case kEnchClothesWarlockRobesHoodedRestoration03:
	case kEnchClothesWarlockRobesHoodedRestoration04:
	case kEnchClothesWarlockRobesHoodedRestoration05:
	case kEnchClothesWarlockRobesHoodedRestoration06:
	case kEnchClothesWarlockRobesIllusion01:
	case kEnchClothesWarlockRobesIllusion02:
	case kEnchClothesWarlockRobesIllusion03:
	case kEnchClothesWarlockRobesIllusion04:
	case kEnchClothesWarlockRobesIllusion05:
	case kEnchClothesWarlockRobesIllusion06:
	case kEnchClothesWarlockRobesMagickaRate02:
	case kEnchClothesWarlockRobesMagickaRate03:
	case kEnchClothesWarlockRobesMagickaRate04:
	case kEnchClothesWarlockRobesMagickaRate05:
	case kEnchClothesWarlockRobesRestoration01:
	case kEnchClothesWarlockRobesRestoration02:
	case kEnchClothesWarlockRobesRestoration03:
	case kEnchClothesWarlockRobesRestoration04:
	case kEnchClothesWarlockRobesRestoration05:
	case kEnchClothesWarlockRobesRestoration06:
	case kTemplateClothesMageRobesApprentice:
	case kTemplateClothesMageRobesApprenticeHooded:
	case kTemplateClothesMageRobesCommon:
	case kTemplateClothesMageRobesCommonHooded:
	case kTemplateClothesRobesMageAdept:
	case kTemplateClothesRobesMageApprentice:
	case kTemplateClothesRobesMageExpert:
	case kTemplateClothesRobesMageMaster:
	case kTemplateClothesRobesMageNovice:
		return Type::kClothingRobe;
	case kClothesBarKeeperShoes:
	case kClothesBlackSmithShoes:
	case kClothesChefShoes:
	case kClothesChildrenShoes:
	case kClothesJarlShoes02:
	case kDBClothesShoes:
		return Type::kClothingShoes;
	case kClothesPrisonerRags:
		return Type::kClothingPants;
	case kArmorDragonPriestMaskIronHelmet:
	case kArmorDragonPriestMaskEbonyHelmet:
	case kArmorDragonPriestMaskMarbleHelmet:
	case kArmorDragonPriestMaskOrichalumHelmet:
	case kArmorDragonPriestMaskSteelHelmet:
	case kArmorDragonPriestMaskUltraHelmet:
		return Type::kArmorMask;
	case kArmorDragonPriestMaskBronzeHelmet:
	case kArmorDragonPriestMaskMoonstoneHelmet:
	case kArmorDragonPriestMaskCorondrumHelmet:
	case kArmorDragonPriestMaskWoodHelmet:
		return Type::kLightArmorMask;
	default:
		return Type::kInvalid;
	}
}


auto GFxItemData::CalcTypeBook(RE::TESObjectBOOK* a_book)
-> Type
{
	auto artID = a_book->inventoryModel ? a_book->inventoryModel->formID : static_cast<RE::FormID>(-1);
	switch (artID) {
	case kHighPolyJournal:
	case kHighPolyJournal02:
	case kHighPolyJournal03:
		return Type::kBookJournal;
	case kHighPolyNote:
	case kHighPolyNote02:
	case kHighPolyNote03:
	case kHighPolyNote04:
	case kHighPolyNoteBlood:
	case kHighPolyNoteTornPage:
		return Type::kBookNote;
	default:
		switch (a_book->formID) {
		case kdunTreasMapRiverwood:
		case kdunTreasMapValtheim:
		case kdunTreasMapSolLightHouse:
		case kdunTreasMapWhiterun:
		case kdunTreasMapLostValleyRedoubt:
		case kdunTreasMapKorvanjund:
		case kdunTreasMapGallowsRock:
		case kdunTreasMapDragonBridge:
		case kdunTreasMapBrokenHelm:
		case kdunTreasMapIlinaltasDeep:
		case kdunTreasMapFortNeugrad:
		case kdunMiddenTreasureMap:
		case kMQ106DragonParchment:
		case kTGLT04EastEmpireShippingRoutes:
			return Type::kBookMap;
		default:
			if (a_book->IsNoteScroll() || a_book->HasKeyword(VendorItemRecipe)) {
				return Type::kBookNote;
			} else if (a_book->HasKeyword(VendorItemSpellTome)) {
				return GetIsRead() ? Type::kBookTomeRead : Type::kBookTome;
			} else {
				return GetIsRead() ? Type::kDefaultBookRead : Type::kDefaultBook;
			}
		}
	}
}


auto GFxItemData::CalcTypeMisc(RE::TESObjectMISC* a_misc)
-> Type
{
	switch (a_misc->formID) {
	case kLockPick:
		return Type::kMiscLockPick;
	case kGold:
		return Type::kMiscGold;
	case kLeather01:
		return Type::kMiscLeather;
	case kLeatherStrips:
		return Type::kMiscStrips;
	case kDragonClawIron:
	case kDragonClawGoldenE3:
	case kDragonClawGoldenMS13:
	case kDragonClawCoral:
	case kDragonClawIvory:
	case kDragonClawRuby:
	case kDragonClawSapphire:
	case kDragonClawEmerald:
	case kDragonClawGlass:
	case kDragonClawEbony:
	case kDragonClawDiamond:
		return Type::kMiscDragonClaw;
	case kOreCorundum:
	case kOreEbony:
	case kOreGold:
	case kOreIron:
	case kOreMalachite:
	case kOreMoonstone:
	case kOreOrichalcum:
	case kOreQuicksilver:
	case kOreSilver:
		return Type::kMiscOre;
	case kBoneTrollSkull01:
		return Type::kMiscTrollSkull;
	case kSoulGemPiece001:
	case kSoulGemPiece002:
	case kSoulGemPiece003:
	case kSoulGemPiece004:
	case kSoulGemPiece005:
		return Type::kMiscSoulGem;
	default:
		for (UInt32 i = 0; i < a_misc->numKeywords; ++i) {
			switch (a_misc->keywords[i]->formID) {
			case kVendorItemAnimalHide:
				return Type::kMiscHide;
			case kVendorItemDaedricArtifact:
				return Type::kMiscArtifact;
			case kVendorItemGem:
				return Type::kMiscGem;
			case kVendorItemAnimalPart:
				return Type::kMiscRemains;
			case kVendorItemOreIngot:
				return Type::kMiscIngot;
			case kVendorItemClutter:
				return Type::kMiscClutter;
			case kVendorItemFireword:
				return Type::kMiscWood;
			default:
				break;
			}
		}
		return Type::kDefaultMisc;
	}
}


auto GFxItemData::CalcTypeWeapon(RE::TESObjectWEAP* a_weap)
-> Type
{
	using WeaponType = RE::WEAPON_TYPE;

	auto fpObjID = a_weap->firstPersonModelObject ? a_weap->firstPersonModelObject->formID : static_cast<RE::FormID>(-1);
	switch (fpObjID) {
	case k1stPersonPickAxe02:
		return Type::kWeaponPickAxe;
	case k1stPersonAxe:
		return Type::kWeaponWoodAxe;
	default:
		switch (a_weap->GetWeaponType()) {
		case WeaponType::kOneHandSword:
			return a_weap->HasKeyword(WeapMaterialDaedric) ? Type::kWeaponDaedra : Type::kWeaponSword;
		case WeaponType::kOneHandDagger:
			return Type::kWeaponDagger;
		case WeaponType::kOneHandAxe:
			return Type::kWeaponWarAxe;
		case WeaponType::kOneHandMace:
			return Type::kWeaponMace;
		case WeaponType::kTwoHandSword:
			return a_weap->HasKeyword(WeapMaterialDaedric) ? Type::kWeaponDaedra : Type::kWeaponGreatSword;
		case WeaponType::kTwoHandAxe:
			if (a_weap->HasKeyword(WeapTypeWarhammer)) {
				return Type::kWeaponHammer;
			} else {
				return Type::kWeaponBattleAxe;
			}
		case WeaponType::kBow:
			return Type::kWeaponBow;
		case WeaponType::kStaff:
			return Type::kWeaponStaff;
		case WeaponType::kCrossbow:
			return Type::kWeaponCrossbow;
		default:
			return Type::kDefaultWeapon;
		}
	}
}


auto GFxItemData::CalcTypeKey(RE::TESKey* a_key)
-> Type
{
	switch (a_key->formID) {
	case kAngasMillAeriHouseKey:
	case kAngasMillCommonHouseKey:
	case kBarleyDarkFarmHouseKey:
	case kDarkwaterCrossingVernersHouseKey:
	case kDawnstarBeitildsHouseKey:
	case kDawnstarBrinasKey:
	case kDawnstarFrukisHouseKey:
	case kDawnstarIrgnirsKey:
	case kDawnstarLeigelfsKey:
	case kDawnstarRustleifsHouseKey:
	case kDragonBridgeHorgeirsHouseKey:
	case kDragonBridgeLylvieveHouseKey:
	case kFalkreathHosueofArkayKey:
	case kFalkreathLodsHouseKey:
	case kHeljarchenFathendasHouseKey:
	case kHeljarchenTraillusHouseKey:
	case kIrontreeMillHouseKey:
	case kIvarsteadKlimmeksHouseKey:
	case kKarthwastenEnmonsHouseKey:
	case kKolskeggrMinePavosHouseKey:
	case kKolskeggrPavosHouseKey:
	case kLeftHandDaighreKey:
	case kLeftHandMineSkaggisHouseKey:
	case kMarkarthAbandonedHouseKey:
	case kMarkarthEndonsHouseKey:
	case kMarkarthNeposHouseKey:
	case kMarkarthOgmundsHouseKey:
	case kMarkarthOverseersHouseKey:
	case kMarkarthTreasuryHouseKey:
	case kMixwaterMillGilfreKey:
	case kMorthalAlvasHouseKey:
	case kMorthalFalionsHouseKey:
	case kMorthalFalionsHouseKeyCOPY0000:
	case kMorthalJorgenLamiKey:
	case kMorthalThonnirsHouseKey:
	case kRiftenAerinsHouseKey:
	case kRiftenBolliHouseKey:
	case kRiftenMariseAravelsHouseKey:
	case kRiftenRomlynKey:
	case kRiftenRomlynsHouseKey:
	case kRiftenSnowShodHouseKey:
	case kRiftenValindorsHouseKey:
	case kRiverwoodAlvorHouseKey:
	case kRiverwoodFaendalsHouseKey:
	case kRiverwoodGerdursHouseKey:
	case kRiverwoodSvensHouseKey:
	case kRoriksteadLemkilsHouseKey:
	case kShorsStoneFilnjarsHouseKey:
	case kShorsStoneOdfelsHouseKey:
	case kShorsStoneSylgjaKey:
	case kSolitudeAddvarsHouseKey:
	case kSolitudeBrylingsHouseKey:
	case kSolitudeErikursHouseKey:
	case kSolitudeEvetteSansHouseKey:
	case kSolitudeJalasHouseKey:
	case kSolitudeStyrrsHouseKey:
	case kSolitudeVittoriaVicisHouseKey:
	case kSoljundsSinkholeMinerHouseKey:
	case kStonehillsAleucsHouseKey:
	case kStonehillsGestursHouseKey:
	case kStonehillsSorlisHouseKey:
	case kTG07MercerHouseKey:
	case kWhiterunAmrensHouseKey:
	case kWhiterunCarlottaValentiasKey:
	case kWhiterunHeimskrsHouseKey:
	case kWhiterunHouseBattleBornKey:
	case kWhiterunHouseGrayManeKey:
	case kWhiterunOlavasKey:
	case kWhiterunSeverioPelagiasHouseKey:
	case kWhiterunUlfbethsKey:
	case kWhiterunUthgerdsHouseKey:
	case kWhiterunYsoldasHouseKey:
	case kWindhelmBelynHlaaluHouseKey:
	case kWindhelmBrunwulfsHouseKey:
	case kWindhelmClanCruelSeaHouseKey:
	case kWindhelmClanShatterShieldHouseKey:
	case kWindhelmNiranyesHouseKey:
	case kWindhelmViolaGiordanosKey:
	case kWinterholdKraldarsHouseKey:
	case kWinterholdRanmirsHouseKey:
	case kBattleBornFarmKey:
	case kBrandyMugFarmKey:
	case kChillfurrowFarmKey:
	case kHeljarchenHeigenFarmHouseKey:
	case kHeljarchenJensFarmHouseKey:
	case kHlaaluFarmKey:
	case kHollyfrostFarmKey:
	case kPelagiusFarmKey:
	case kSalviusFarmhouseKey:
		return Type::kKeyHouse;
	default:
		return Type::kDefaultKey;
	}
}


auto GFxItemData::CalcTypePotion(RE::AlchemyItem* a_potion)
-> Type
{
	using RE::ActorValue;

	if (a_potion->IsFoodItem()) {
		auto sound = a_potion->data.consumptionSound;
		if (sound && sound->formID == kITMPotionUse) {
			switch (a_potion->formID) {
			case kFoodMead:
			case kMQ101JuniperMead:
			case kFoodHonningbrewMead:
			case kFreeformDragonBridgeMead:
			case kAle:
			case kAleWhiterunQuest:
			case kFoodBlackBriarMead:
			case kFoodBlackBriarMeadPrivateReserve:
				return Type::kFoodBeer;
			default:
				return Type::kFoodWine;
			}
		} else {
			return Type::kDefaultFood;
		}
	} else if (a_potion->IsPoison()) {
		return Type::kPotionPoison;
	} else {
		auto effect = a_potion->GetCostliestEffectItem();
		if (effect && effect->baseEffect) {
			switch (effect->baseEffect->GetMagickSkill()) {
			case ActorValue::kHealth:
				return Type::kPotionHealth;
			case ActorValue::kMagicka:
				return Type::kPotionMagic;
			case ActorValue::kStamina:
				return Type::kPotionStam;
			case ActorValue::kResistFire:
				return Type::kPotionFire;
			case ActorValue::kResistShock:
				return Type::kPotionShock;
			case ActorValue::kResistFrost:
				return Type::kPotionFrost;
			}
		}
		return Type::kDefaultPotion;
	}
}


auto GFxItemData::CalcTypeSoulGem(RE::TESSoulGem* a_gem)
-> Type
{
	using SoulLevel = RE::SOUL_LEVEL;

	switch (a_gem->GetFormID()) {
	case kDA01SoulGemBlackStar:
	case kDA01SoulGemAzurasStar:
		return Type::kSoulGemAzura;
	default:
		{
			auto soulSize = _itemImpl.GetSoulLevel();
			if (a_gem->GetMaximumCapacity() < SoulLevel::kGreater) {
				if (soulSize >= a_gem->GetMaximumCapacity()) {
					return Type::kSoulGemFull;
				} else if (soulSize > SoulLevel::kNone) {
					return Type::kSoulGemPartial;
				} else {
					return Type::kSoulGemEmpty;
				}
			} else {
				if (soulSize >= a_gem->GetMaximumCapacity()) {
					return Type::kSoulGemGrandFull;
				} else if (soulSize > SoulLevel::kNone) {
					return Type::kSoulGemGrandPartial;
				} else {
					return Type::kSoulGemGrandEmpty;
				}
			}
		}
	}
}


SInt32 GFxItemData::CalcValue()
{
	return _itemImpl.GetValue();
}


float GFxItemData::CalcWeight()
{
	return _itemImpl.GetWeight();
}


decltype(GFxItemData::_compares) GFxItemData::_compares;
