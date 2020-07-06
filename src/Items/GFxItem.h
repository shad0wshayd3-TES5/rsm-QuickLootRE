#pragma once

namespace Items
{
	class GFxItem
	{
	public:
		inline GFxItem(std::ptrdiff_t a_count, observer<RE::InventoryEntryData*> a_item) :
			_displayName(),
			_flags(),
			_count(a_count),
			_value(0),
			_enchantmentCharge(100.0),
			_formID(std::numeric_limits<RE::FormID>::max())
		{
			if (a_item) {
				SetAmmo(*a_item);
				SetBook(*a_item);
				SetDisplayName(*a_item);
				SetEnchanted(*a_item);
				SetEnchantmentCharge(*a_item);
				SetFormID(*a_item);
				SetGold(*a_item);
				SetKey(*a_item);
				SetLockpick(*a_item);
				SetNote(*a_item);
				SetQuestItem(*a_item);
				SetStolen(*a_item);
				SetValue(*a_item);
			} else {
				assert(false);
			}
		}

		inline GFxItem(std::ptrdiff_t a_count, const std::vector<RE::ObjectRefHandle>& a_items) :
			_displayName(),
			_flags(),
			_count(a_count),
			_value(0),
			_enchantmentCharge(100.0),
			_formID(std::numeric_limits<RE::FormID>::max())
		{
			std::vector<RE::TESObjectREFRPtr> items;
			items.reserve(a_items.size());
			for (auto& item : a_items) {
				items.push_back(item.get());
			}

			const auto enumerate = [&](std::function<bool(GFxItem&, RE::TESObjectREFR&)> a_functor) {
				for (const auto& item : items) {
					if (item) {
						if (!a_functor(*this, *item)) {
							break;
						}
					}
				}
			};

			enumerate(&GFxItem::AmmoFunctor);
			enumerate(&GFxItem::BookFunctor);
			enumerate(&GFxItem::DisplayNameFunctor);
			enumerate(&GFxItem::EnchantedFunctor);
			enumerate(&GFxItem::EnchantmentChargeFunctor);
			enumerate(&GFxItem::FormIDFunctor);
			enumerate(&GFxItem::GoldFunctor);
			enumerate(&GFxItem::KeyFunctor);
			enumerate(&GFxItem::LockpickFunctor);
			enumerate(&GFxItem::NoteFunctor);
			enumerate(&GFxItem::QuestItemFunctor);
			enumerate(&GFxItem::StolenFunctor);
			enumerate(&GFxItem::ValueFunctor);
		}

		[[nodiscard]] constexpr std::ptrdiff_t Count() const noexcept { return _count; }

		// Quest Item
		// Key
		// Note
		// Book
		// Gold
		// Ammo
		// Healing?
		// Lockpick
		// Value
		// Alphabetical
		// FormID
		[[nodiscard]] inline int Compare(const GFxItem& a_rhs) const
		{
			if (_flags[kQuestItem] != a_rhs._flags[kQuestItem]) {
				return _flags[kQuestItem] ? -1 : 1;
			} else if (_flags[kKey] != a_rhs._flags[kKey]) {
				return _flags[kKey] ? -1 : 1;
			} else if (_flags[kNote] != a_rhs._flags[kNote]) {
				return _flags[kNote] ? -1 : 1;
			} else if (_flags[kBook] != a_rhs._flags[kBook]) {
				return _flags[kBook] ? -1 : 1;
			} else if (_flags[kGold] != a_rhs._flags[kGold]) {
				return _flags[kGold] ? -1 : 1;
			} else if (_flags[kAmmo] != a_rhs._flags[kAmmo]) {
				return _flags[kAmmo] ? -1 : 1;
			} else if (_flags[kLockpick] != a_rhs._flags[kLockpick]) {
				return _flags[kLockpick] ? -1 : 1;
			} else if (_value != a_rhs._value) {
				return _value > a_rhs._value ? -1 : 1;
			} else if (const auto alphabetical = _stricmp(_displayName.c_str(), a_rhs._displayName.c_str());
					   alphabetical != 0) {
				return alphabetical < 0 ? -1 : 1;
			} else if (_formID != a_rhs._formID) {
				return _formID < a_rhs._formID ? -1 : 1;
			} else {
				return 0;
			}
		}

		[[nodiscard]] inline RE::GFxValue Value() const { return { _displayName }; }

	private:
		enum : std::size_t
		{
			kQuestItem,
			kKey,
			kNote,
			kBook,
			kGold,
			kAmmo,
			kLockpick,
			kEnchanted,
			kStolen,
			kTotal
		};

		[[nodiscard]] inline bool AmmoFunctor(RE::TESObjectREFR& a_ref)
		{
			auto obj = a_ref.GetObjectReference();
			if (obj) {
				_flags.set(kAmmo, obj->IsAmmo());
				return false;
			}
			return true;
		}

		[[nodiscard]] inline bool BookFunctor(RE::TESObjectREFR& a_ref)
		{
			auto obj = a_ref.GetObjectReference();
			if (obj) {
				_flags.set(kBook, obj->IsBook());
				return false;
			}
			return true;
		}

		[[nodiscard]] inline bool DisplayNameFunctor(RE::TESObjectREFR& a_ref)
		{
			_displayName = safe_string(a_ref.GetDisplayFullName());
			return _displayName.empty();
		}

		[[nodiscard]] inline bool EnchantedFunctor(RE::TESObjectREFR& a_ref)
		{
			auto charge = a_ref.GetEnchantmentCharge();
			if (charge) {
				_enchantmentCharge = *charge;
				return false;
			}
			return true;
		}

		[[nodiscard]] inline bool EnchantmentChargeFunctor(RE::TESObjectREFR& a_ref)
		{
			if (a_ref.IsEnchanted()) {
				_flags.set(kEnchanted);
				return false;
			}
			return true;
		}

		[[nodiscard]] inline bool FormIDFunctor(RE::TESObjectREFR& a_ref)
		{
			auto obj = a_ref.GetObjectReference();
			if (obj) {
				_formID = obj->GetFormID();
				return false;
			}
			return true;
		}

		[[nodiscard]] inline bool GoldFunctor(RE::TESObjectREFR& a_ref)
		{
			auto obj = a_ref.GetObjectReference();
			if (obj && obj->IsGold()) {
				_flags.set(kGold);
				return false;
			}
			return true;
		}

		[[nodiscard]] inline bool KeyFunctor(RE::TESObjectREFR& a_ref)
		{
			auto obj = a_ref.GetObjectReference();
			if (obj) {
				_flags.set(kKey, obj->IsKey());
				return false;
			}
			return true;
		}

		[[nodiscard]] inline bool LockpickFunctor(RE::TESObjectREFR& a_ref)
		{
			auto obj = a_ref.GetObjectReference();
			if (obj) {
				_flags.set(kLockpick, obj->IsLockpick());
				return false;
			}
			return true;
		}

		[[nodiscard]] inline bool NoteFunctor(RE::TESObjectREFR& a_ref)
		{
			auto obj = a_ref.GetObjectReference();
			if (obj) {
				_flags.set(kNote, obj->IsNote());
				return false;
			}
			return true;
		}

		[[nodiscard]] inline bool QuestItemFunctor(RE::TESObjectREFR& a_ref)
		{
			if (a_ref.HasQuestObject()) {
				_flags.set(kQuestItem);
				return false;
			}
			return true;
		}

		[[nodiscard]] inline bool StolenFunctor(RE::TESObjectREFR& a_ref)
		{
			auto player = RE::PlayerCharacter::GetSingleton();
			if (player && player->WouldBeStealing(std::addressof(a_ref))) {
				_flags.set(kStolen);
				return false;
			}
			return true;
		}

		[[nodiscard]] inline bool ValueFunctor(RE::TESObjectREFR& a_ref)
		{
			_value = a_ref.GetGoldValue() * _count;
			return false;
		}

		inline void SetAmmo(RE::InventoryEntryData& a_item)
		{
			auto obj = a_item.GetObject();
			if (obj && obj->IsAmmo()) {
				_flags.set(kAmmo);
			}
		}

		inline void SetBook(RE::InventoryEntryData& a_item)
		{
			auto obj = a_item.GetObject();
			if (obj && obj->IsBook()) {
				_flags.set(kBook);
			}
		}

		inline void SetDisplayName(RE::InventoryEntryData& a_item)
		{
			_displayName = safe_string(a_item.GetDisplayName());
		}

		inline void SetEnchanted(RE::InventoryEntryData& a_item)
		{
			_flags.set(kEnchanted, a_item.IsEnchanted());
		}

		inline void SetEnchantmentCharge(RE::InventoryEntryData& a_item)
		{
			auto charge = a_item.GetEnchantmentCharge();
			if (charge) {
				_enchantmentCharge = *charge;
			}
		}

		inline void SetFormID(RE::InventoryEntryData& a_item)
		{
			auto obj = a_item.GetObject();
			if (obj) {
				_formID = obj->GetFormID();
			}
		}

		inline void SetGold(RE::InventoryEntryData& a_item)
		{
			auto obj = a_item.GetObject();
			if (obj) {
				_flags.set(kGold, obj->IsGold());
			}
		}

		inline void SetKey(RE::InventoryEntryData& a_item)
		{
			auto obj = a_item.GetObject();
			if (obj) {
				_flags.set(kKey, obj->IsKey());
			}
		}

		inline void SetLockpick(RE::InventoryEntryData& a_item)
		{
			auto obj = a_item.GetObject();
			if (obj) {
				_flags.set(kLockpick, obj->IsLockpick());
			}
		}

		inline void SetNote(RE::InventoryEntryData& a_item)
		{
			auto obj = a_item.GetObject();
			if (obj && obj->IsNote()) {
				_flags.set(kNote);
			}
		}

		inline void SetQuestItem(RE::InventoryEntryData& a_item)
		{
			if (a_item.IsQuestObject()) {
				_flags.set(kQuestItem);
			}
		}

		inline void SetStolen(RE::InventoryEntryData& a_item)
		{
			auto player = RE::PlayerCharacter::GetSingleton();
			if (player) {
				_flags.set(kStolen, !a_item.IsOwnedBy(player));
			}
		}

		inline void SetValue(RE::InventoryEntryData& a_item)
		{
			auto obj = a_item.GetObject();
			if (obj) {
				_value = obj->GetGoldValue() * _count;
			}
		}

		std::string _displayName;
		std::bitset<kTotal> _flags;
		std::ptrdiff_t _count;
		std::ptrdiff_t _value;
		double _enchantmentCharge;
		RE::FormID _formID;
	};

	[[nodiscard]] inline bool operator==(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) == 0; }
	[[nodiscard]] inline bool operator!=(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) != 0; }
	[[nodiscard]] inline bool operator<(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) < 0; }
	[[nodiscard]] inline bool operator>(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) > 0; }
	[[nodiscard]] inline bool operator<=(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) <= 0; }
	[[nodiscard]] inline bool operator>=(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) >= 0; }
}
