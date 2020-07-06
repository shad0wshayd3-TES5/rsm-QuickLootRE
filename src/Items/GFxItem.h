#pragma once

namespace Items
{
	class GFxItem
	{
	public:
		inline GFxItem(observer<RE::InventoryEntryData*> a_item) :
			_displayName(),
			_flags(),
			_enchantmentCharge(100.0)
		{
			auto player = RE::PlayerCharacter::GetSingleton();
			assert(a_item != nullptr);
			assert(player != nullptr);

			_displayName = safe_string(a_item->GetDisplayName());

			auto obj = a_item->GetObject();
			if (obj) {
				_flags.set(kGold, obj->IsGold());
			}

			_flags.set(kEnchanted, a_item->IsEnchanted());
			_flags.set(kStolen, !a_item->IsOwnedBy(player));

			auto charge = a_item->GetEnchantmentCharge();
			if (charge) {
				_enchantmentCharge = *charge;
			}
		}

		inline GFxItem(const std::vector<RE::ObjectRefHandle>& a_items) :
			_displayName(),
			_flags(),
			_enchantmentCharge(100.0)
		{
			auto player = RE::PlayerCharacter::GetSingleton();
			assert(player != nullptr);

			std::vector<RE::TESObjectREFRPtr> items;
			items.reserve(a_items.size());
			for (auto& item : a_items) {
				items.push_back(item.get());
			}

			for (auto& item : items) {
				_displayName = safe_string(item->GetDisplayFullName());
				if (!_displayName.empty()) {
					break;
				}
			}

			if (!items.empty()) {
				auto obj = items.front()->GetObjectReference();
				if (obj && obj->IsGold()) {
					_flags.set(kGold);
				}
			}

			for (auto& item : items) {
				auto ench = item->IsEnchanted();
				if (ench) {
					_flags.set(kEnchanted);
					break;
				}
			}

			for (auto& item : items) {
				auto stolen = player->WouldBeStealing(item.get());
				if (stolen) {
					_flags.set(kStolen);
					break;
				}
			}

			for (auto& item : items) {
				auto charge = item->GetEnchantmentCharge();
				if (charge) {
					_enchantmentCharge = *charge;
					break;
				}
			}
		}

		[[nodiscard]] inline int compare(const GFxItem& a_rhs) const
		{
			// Quest Item
			// Key
			// Note
			// Book
			// Gold
			// Ammo
			// Healing
			// Lockpick
			// Value
			// Alphabetical
			// FormID

			if (const auto gold = _flags[kGold] != a_rhs._flags[kGold]; gold) {
				return _flags[kGold] ? -1 : 1;
			} else if (const auto alpha = _stricmp(_displayName.c_str(), a_rhs._displayName.c_str()); alpha) {
				return alpha < 0 ? -1 : 1;
			} else {
				return 0;
			}
		}

		[[nodiscard]] inline RE::GFxValue Value() const { return { _displayName }; }

	private:
		enum : std::size_t
		{
			kGold,
			kEnchanted,
			kStolen,
			kTotal
		};

		std::string _displayName;
		std::bitset<kTotal> _flags;
		double _enchantmentCharge;
	};

	[[nodiscard]] inline bool operator==(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.compare(a_rhs) == 0; }
	[[nodiscard]] inline bool operator!=(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.compare(a_rhs) != 0; }
	[[nodiscard]] inline bool operator<(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.compare(a_rhs) < 0; }
	[[nodiscard]] inline bool operator>(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.compare(a_rhs) > 0; }
	[[nodiscard]] inline bool operator<=(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.compare(a_rhs) <= 0; }
	[[nodiscard]] inline bool operator>=(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.compare(a_rhs) >= 0; }
}
