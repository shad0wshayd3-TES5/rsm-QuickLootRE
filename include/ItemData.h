#pragma once

#include "common/ITypes.h"  // UInt32
#include "skse64/GameExtraData.h"  // InventoryEntryData
#include "skse64/GameForms.h"  // TESForm
#include "skse64/GameObjects.h"  // TESObjectARMO, TESObjectBOOK, TESObjectMISC, TESObjectWEAP, AlchemyItem, TESSoulGem

#include <utility>  // swap
#include <vector>  // vector

namespace RE
{
	class InventoryEntryData;
	class TESObjectREFR;
}


namespace QuickLootRE
{
	class ItemData
	{
	private:
		typedef int(*FnCompare)(const ItemData& a_lhs, const ItemData& a_rhs);


		enum FormID : UInt32
		{
			kFormID_LockPick = 0x0000000A,
			kFormID_Gold = 0x0000000F,
			kFormID_Leather01 = 0x000DB5D2,
			kFormID_LeatherStrips = 0x000800E4,

			kFormID_DragonClawIron = 0x0008CDFA,
			kFormID_DragonClawGoldenE3 = 0x000999E7,
			kFormID_DragonClawGoldenMS13 = 0x00039647,
			kFormID_DragonClawCoral = 0x000B634C,
			kFormID_DragonClawIvory = 0x000AB7BB,
			kFormID_DragonClawRuby = 0x0004B56C,
			kFormID_DragonClawSapphire = 0x000663D7,
			kFormID_DragonClawEmerald = 0x000ED417,
			kFormID_DragonClawGlass = 0x0007C260,
			kFormID_DragonClawEbony = 0x0005AF48,
			kFormID_DragonClawDiamond = 0x000AB375,

			kFormID_ITMPotionUse = 0x000B6435,

			kFormID_DA01SoulGemAzurasStar = 0x00063B27,
			kFormID_DA01SoulGemBlackStar = 0x00063B29,
		};


		enum Priority : UInt32
		{
			kPriority_Key,
			kPriority_Gold,
			kPriority_LockPick,
			kPriority_Ammo,
			kPriority_SoulGem,
			kPriority_Potion,
			kPriority_Poison,
			kPriority_EnchantedWeapon,
			kPriority_EnchantedArmor,
			kPriority_Gem,
			kPriority_Amulet,
			kPriority_Ring,
			kPriority_Weapon,
			kPriority_Armor,
			kPriority_Food,
			kPriority_Other = kPriority_Food,
		};


		enum Type : UInt32
		{
			kType_None,
			kType_DefaultWeapon,
			kType_WeaponSword,
			kType_WeaponGreatSword,
			kType_WeaponDaedra,
			kType_WeaponDagger,
			kType_WeaponWarAxe,
			kType_WeaponBattleAxe,
			kType_WeaponMace,
			kType_WeaponHammer,
			kType_WeaponStaff,
			kType_WeaponBow,
			kType_WeaponArrow,
			kType_WeaponPickAxe,
			kType_WeaponWoodAxe,
			kType_WeaponCrossbow,
			kType_WeaponBolt,

			kType_DefaultArmor,

			kType_LightArmorBody,
			kType_LightArmorHead,
			kType_LightArmorHands,
			kType_LightArmorForearms,
			kType_LightArmorFeet,
			kType_LightArmorCalves,
			kType_LightArmorShield,
			kType_LightArmorMask,

			kType_ArmorBody,
			kType_ArmorHead,
			kType_ArmorHands,
			kType_ArmorForearms,
			kType_ArmorFeet,
			kType_ArmorCalves,
			kType_ArmorShield,
			kType_ArmorMask,
			kType_ArmorBracer,
			kType_ArmorDaedra,

			kType_ClothingBody,
			kType_ClothingRobe,
			kType_ClothingHead,
			kType_ClothingPants,
			kType_ClothingHands,
			kType_ClothingForearms,
			kType_ClothingFeet,
			kType_ClothingCalves,
			kType_ClothingShoes,
			kType_ClothingShield,
			kType_ClothingMask,

			kType_ArmorAmulet,
			kType_ArmorRing,
			kType_Circlet,

			kType_DefaultScroll,

			kType_DefaultBook,
			kType_BookRead,
			kType_BookTome,
			kType_TomeRead,
			kType_BookJournal,
			kType_BookNote,
			kType_BookMap,

			kType_DefaultFood,
			kType_FoodWine,
			kType_FoodBeer,

			kType_DefaultIngredient,

			kType_DefaultKey,
			kType_KeyHouse,

			kType_DefaultPotion,
			kType_PotionHealth,
			kType_PotionStam,
			kType_PotionMagic,
			kType_PotionPoison,
			kType_PotionFrost,
			kType_PotionFire,
			kType_PotionShock,

			kType_DefaultMisc,
			kType_MiscArtifact,
			kType_MiscClutter,
			kType_MiscLockPick,
			kType_MiscSoulGem,

			kType_SoulGemEmpty,
			kType_SoulGemPartial,
			kType_SoulGemFull,
			kType_SoulGemGrandEmpty,
			kType_SoulGemGrandPartial,
			kType_SoulGemGrandFull,
			kType_SoulGemAzura,

			kType_MiscGem,
			kType_MiscOre,
			kType_MiscIngot,
			kType_MiscHide,
			kType_MiscStrips,
			kType_MiscLeather,
			kType_MiscWood,
			kType_MiscRemains,
			kType_MiscTrollSkull,
			kType_MiscTorch,
			kType_MiscGoldSack,
			kType_MiscGold,
			kType_MiscDragonClaw
		};

	public:
		explicit ItemData(RE::InventoryEntryData* a_entryData);
		explicit ItemData(RE::InventoryEntryData* a_entryData, SInt32 a_count);
		~ItemData();

		ItemData&						operator= (ItemData a_rhs);
		friend bool						operator==(const ItemData& a_lhs, const ItemData& a_rhs);
		friend bool						operator!=(const ItemData& a_lhs, const ItemData& a_rhs);
		friend bool						operator< (const ItemData& a_lhs, const ItemData& a_rhs);
		friend bool						operator> (const ItemData& a_lhs, const ItemData& a_rhs);
		friend bool						operator<=(const ItemData& a_lhs, const ItemData& a_rhs);
		friend bool						operator>=(const ItemData& a_lhs, const ItemData& a_rhs);
		friend void						swap(ItemData& a_lhs, ItemData& a_rhs);

		RE::InventoryEntryData*			entryData()			const;
		const char*						name()				const;
		SInt32							count()				const;
		SInt32							value()				const;
		float							weight()			const;
		const char*						icon()				const;
		bool							isStolen()			const;
		bool							isRead()			const;
		bool							isEnchanted()		const;
		UInt32							pickPocketChance()	const;
		TESForm*						form()				const;

		void							reduceCount();

		static void						setCompareOrder();
		static void						setContainer(RE::TESObjectREFR* a_container);

	private:
		void							constructCommon();

		float							getWeight();
		Type							getType();
		Type							getTypeArmor(TESObjectARMO* a_armor);
		Type							getTypeBook(TESObjectBOOK* a_book);
		Type							getTypeMisc(TESObjectMISC* a_misc);
		Type							getTypeWeapon(TESObjectWEAP* a_weap);
		Type							getTypePotion(AlchemyItem* a_potion);
		Type							getTypeSoulGem(TESSoulGem* a_gem);
		bool							getStolen();
		bool							getRead();
		bool							getEnchanted();
		UInt32							getPickPocketChance();
		Priority						getPriority();

		friend int						compareByStolen	(const ItemData& a_lhs, const ItemData& a_rhs);
		friend int						compareByType	(const ItemData& a_lhs, const ItemData& a_rhs);
		friend int						compareByName	(const ItemData& a_lhs, const ItemData& a_rhs);
		friend int						compareByValue	(const ItemData& a_lhs, const ItemData& a_rhs);
		friend int						compareByCount	(const ItemData& a_lhs, const ItemData& a_rhs);

		RE::InventoryEntryData*			_entryData;
		const char*						_name;
		SInt32							_count;
		SInt32							_value;
		float							_weight;
		Type							_type;
		bool							_isStolen;
		bool							_isRead;
		bool							_isEnchanted;
		UInt32							_pickPocketChance;
		Priority						_priority;

		static std::vector<FnCompare>	_compares;
		static RE::TESObjectREFR*		_container;
		static const char*				_strIcons[];
	};


	inline ItemData& ItemData::operator=(ItemData a_rhs)
	{
		swap(*this, a_rhs);
		return *this;
	}


	inline bool operator==(const ItemData& a_lhs, const ItemData& a_rhs)
	{
		return (a_lhs._entryData == a_rhs._entryData &&
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


	inline bool operator!=(const ItemData& a_lhs, const ItemData& a_rhs)
	{
		return !operator==(a_lhs, a_rhs);
	}


	inline bool operator>(const ItemData& a_lhs, const ItemData& a_rhs)
	{
		return operator<(a_rhs, a_lhs);
	}


	inline bool operator<=(const ItemData& a_lhs, const ItemData& a_rhs)
	{
		return !operator>(a_lhs, a_rhs);
	}


	inline bool operator>=(const ItemData& a_lhs, const ItemData& a_rhs)
	{
		return !operator<(a_lhs, a_rhs);
	}

	inline void swap(ItemData& a_lhs, ItemData& a_rhs)
	{
		std::swap(a_lhs._entryData, a_rhs._entryData);
		std::swap(a_lhs._name, a_rhs._name);
		std::swap(a_lhs._count, a_rhs._count);
		std::swap(a_lhs._value, a_rhs._value);
		std::swap(a_lhs._weight, a_rhs._weight);
		std::swap(a_lhs._type, a_rhs._type);
		std::swap(a_lhs._isStolen, a_rhs._isStolen);
		std::swap(a_lhs._isEnchanted, a_rhs._isEnchanted);
		std::swap(a_lhs._pickPocketChance, a_rhs._pickPocketChance);
		std::swap(a_lhs._priority, a_rhs._priority);
	}
}
