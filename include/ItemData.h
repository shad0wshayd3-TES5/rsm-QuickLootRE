#pragma once

#include <vector>  // vector
#include <string>  // string

#include "ManagedEntryData.h"  // ManagedEntryDataPtr

#include "RE/Skyrim.h"


namespace
{
	enum
	{
		kDebugType_Name,
		kDebugType_Count,
		kDebugType_Value,
		kDebugType_Weight,
		kDebugType_Type,
		kDebugType_Read,
		kDebugType_Enchanted,
		kDebugType_PickPocketChance,
		kDebugType_ValuePerWeight,
		kDebugType_Priority
	};
}


#define ITEM_DATA_DEBUG_ENABLED false
#if ITEM_DATA_DEBUG_ENABLED
#pragma message("ItemData debugging is enabled! Don't forget to disable it!")
#endif
#define ITEM_DATA_DEBUG_TYPE kDebugType_ValuePerWeight


class ItemData
{
private:
	using FnCompare = int(const ItemData& a_lhs, const ItemData& a_rhs);


	enum class Priority : UInt32
	{
		kKey,
		kGold,
		kLockPick,
		kAmmo,
		kSoulGem,
		kPotion,
		kPoison,
		kEnchantedWeapon,
		kEnchantedArmor,
		kGem,
		kAmulet,
		kRing,
		kWeapon,
		kArmor,
		kFood,
		kOther = kFood,
	};


	enum class Type : UInt32
	{
		kNone,
		kDefaultWeapon,
		kWeaponSword,
		kWeaponGreatSword,
		kWeaponDaedra,
		kWeaponDagger,
		kWeaponWarAxe,
		kWeaponBattleAxe,
		kWeaponMace,
		kWeaponHammer,
		kWeaponStaff,
		kWeaponBow,
		kWeaponArrow,
		kWeaponPickAxe,
		kWeaponWoodAxe,
		kWeaponCrossbow,
		kWeaponBolt,

		kDefaultArmor,

		kLightArmorBody,
		kLightArmorHead,
		kLightArmorHands,
		kLightArmorForearms,
		kLightArmorFeet,
		kLightArmorCalves,
		kLightArmorShield,
		kLightArmorMask,

		kArmorBody,
		kArmorHead,
		kArmorHands,
		kArmorForearms,
		kArmorFeet,
		kArmorCalves,
		kArmorShield,
		kArmorMask,
		kArmorBracer,
		kArmorDaedra,

		kClothingBody,
		kClothingRobe,
		kClothingHead,
		kClothingPants,
		kClothingHands,
		kClothingForearms,
		kClothingFeet,
		kClothingCalves,
		kClothingShoes,
		kClothingShield,
		kClothingMask,

		kArmorAmulet,
		kArmorRing,
		kCirclet,

		kDefaultScroll,

		kDefaultBook,
		kBookRead,
		kBookTome,
		kTomeRead,
		kBookJournal,
		kBookNote,
		kBookMap,

		kDefaultFood,
		kFoodWine,
		kFoodBeer,

		kDefaultIngredient,

		kDefaultKey,
		kKeyHouse,

		kDefaultPotion,
		kPotionHealth,
		kPotionStam,
		kPotionMagic,
		kPotionPoison,
		kPotionFrost,
		kPotionFire,
		kPotionShock,

		kDefaultMisc,
		kMiscArtifact,
		kMiscClutter,
		kMiscLockPick,
		kMiscSoulGem,

		kSoulGemEmpty,
		kSoulGemPartial,
		kSoulGemFull,
		kSoulGemGrandEmpty,
		kSoulGemGrandPartial,
		kSoulGemGrandFull,
		kSoulGemAzura,

		kMiscGem,
		kMiscOre,
		kMiscIngot,
		kMiscHide,
		kMiscStrips,
		kMiscLeather,
		kMiscWood,
		kMiscRemains,
		kMiscTrollSkull,
		kMiscTorch,
		kMiscGoldSack,
		kMiscGold,
		kMiscDragonClaw
	};

public:
	ItemData();
	ItemData(const ItemData&) = default;
	ItemData(ItemData&&) = default;
	explicit ItemData(ManagedEntryDataPtr a_entryData);
	explicit ItemData(ManagedEntryDataPtr a_entryData, SInt32 a_count);
	~ItemData();

	static void	setCompareOrder();
	static void	setContainer(RE::TESObjectREFR* a_container);

	ItemData& operator=(const ItemData&) = default;
	ItemData& operator=(ItemData&&) = default;
	friend bool	operator==(const ItemData& a_lhs, const ItemData& a_rhs);
	friend bool	operator!=(const ItemData& a_lhs, const ItemData& a_rhs);
	friend bool	operator< (const ItemData& a_lhs, const ItemData& a_rhs);
	friend bool	operator> (const ItemData& a_lhs, const ItemData& a_rhs);
	friend bool	operator<=(const ItemData& a_lhs, const ItemData& a_rhs);
	friend bool	operator>=(const ItemData& a_lhs, const ItemData& a_rhs);

	RE::InventoryEntryData*	entryData() const;
	const char*				name() const;
	SInt32					count() const;
	SInt32					value() const;
	float					weight() const;
	const char*				icon() const;
	bool					isStolen() const;
	bool					isRead() const;
	bool					isEnchanted() const;
	bool					canPickPocket() const;
	SInt32					pickPocketChance() const;
	RE::TESForm*			form() const;

	void	modCount(SInt32 a_mod);
	void	dbgDumpType(UInt32 a_index);

private:
	void	constructCommon();

	float		getWeight();
	Type		getType();
	Type		getTypeArmor(RE::TESObjectARMO* a_armor);
	Type		getTypeBook(RE::TESObjectBOOK* a_book);
	Type		getTypeMisc(RE::TESObjectMISC* a_misc);
	Type		getTypeWeapon(RE::TESObjectWEAP* a_weap);
	Type		getTypePotion(RE::AlchemyItem* a_potion);
	Type		getTypeSoulGem(RE::TESSoulGem* a_gem);
	bool		getStolen();
	bool		getRead();
	bool		getEnchanted();
	bool		getCanPickPocket();
	SInt32		getPickPocketChance();
	Priority	getPriority();

	static friend int	compareByName(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	r_compareByName(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	compareByCount(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	r_compareByCount(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	compareByValue(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	r_compareByValue(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	compareByWeight(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	r_compareByWeight(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	compareByType(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	r_compareByType(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	compareByRead(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	r_compareByRead(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	compareByEnchanted(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	r_compareByEnchanted(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	compareByPickPocketChance(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	r_compareByPickPocketChance(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	compareByPriority(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	r_compareByPriority(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	compareByValuePerWeight(const ItemData& a_lhs, const ItemData& a_rhs);
	static friend int	r_compareByValuePerWeight(const ItemData& a_lhs, const ItemData& a_rhs);

	ManagedEntryDataPtr	_entryData;
	const char*			_name;
	SInt32				_count;
	SInt32				_value;
	float				_weight;
	Type				_type;
	bool				_isStolen;
	bool				_isRead;
	bool				_isEnchanted;
	bool				_canPickPocket;
	SInt32				_pickPocketChance;
	Priority			_priority;

	static const std::string			NAME;
	static const std::string			COUNT;
	static const std::string			VALUE;
	static const std::string			WEIGHT;
	static const std::string			TYPE;
	static const std::string			READ;
	static const std::string			ENCHANTED;
	static const std::string			PICK_POCKET_CHANCE;
	static const std::string			VALUE_PER_WEIGHT;
	static const std::string			PRIORITY;
	static std::vector<FnCompare*>		_compares;
	inline static RE::TESObjectREFR*	_container = 0;
	static constexpr char*				_strIcons[] = {
	"none",					// 00
	"default_weapon",
	"weapon_sword",
	"weapon_greatsword",
	"weapon_daedra",
	"weapon_dagger",
	"weapon_waraxe",
	"weapon_battleaxe",
	"weapon_mace",
	"weapon_hammer",
	"weapon_staff",			// 10
	"weapon_bow",
	"weapon_arrow",
	"weapon_pickaxe",
	"weapon_woodaxe",
	"weapon_crossbow",
	"weapon_bolt",
	"default_armor",
	"lightarmor_body",
	"lightarmor_head",
	"lightarmor_hands",		// 20
	"lightarmor_forearms",
	"lightarmor_feet",
	"lightarmor_calves",
	"lightarmor_shield",
	"lightarmor_mask",
	"armor_body",
	"armor_head",
	"armor_hands",
	"armor_forearms",
	"armor_feet",			// 30
	"armor_calves",
	"armor_shield",
	"armor_mask",
	"armor_bracer",
	"armor_daedra",
	"clothing_body",
	"clothing_robe",
	"clothing_head",
	"clothing_pants",
	"clothing_hands",		// 40
	"clothing_forearms",
	"clothing_feet",
	"clothing_calves",
	"clothing_shoes",
	"clothing_shield",
	"clothing_mask",
	"armor_amulet",
	"armor_ring",
	"armor_circlet",
	"default_scroll",		// 50
	"default_book",
	"default_book_read",
	"book_tome",
	"book_tome_read",
	"book_journal",
	"book_note",
	"book_map",
	"default_food",
	"food_wine",
	"food_beer",			// 60
	"default_ingredient",
	"default_key",
	"key_house",
	"default_potion",
	"potion_health",
	"potion_stam",
	"potion_magic",
	"potion_poison",
	"potion_frost",
	"potion_fire",			// 70
	"potion_shock",
	"default_misc",
	"misc_artifact",
	"misc_clutter",
	"misc_lockpick",
	"misc_soulgem",
	"soulgem_empty",
	"soulgem_partial",
	"soulgem_full",
	"soulgem_grandempty",	// 80
	"soulgem_grandpartial",
	"soulgem_grandfull",
	"soulgem_azura",
	"misc_gem",
	"misc_ore",
	"misc_ingot",
	"misc_hide",
	"misc_strips",
	"misc_leather",
	"misc_wood",			// 90
	"misc_remains",
	"misc_trollskull",
	"misc_torch",
	"misc_goldsack",
	"misc_gold",
	"misc_dragonclaw"
	};
};
