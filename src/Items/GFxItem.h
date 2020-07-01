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
			_flags.set(kEnchanted, a_item->IsEnchanted());
			_flags.set(kStolen, !a_item->IsOwnedBy(player));
			auto charge = a_item->GetEnchantmentCharge();
			if (charge) {
				_enchantmentCharge = *charge;
			}
		}

		inline GFxItem(const std::vector<RE::NiPointer<RE::TESObjectREFR>>& a_items) :
			_displayName(),
			_flags(),
			_enchantmentCharge(100.0)
		{
			auto player = RE::PlayerCharacter::GetSingleton();
			assert(player != nullptr);

			for (auto& item : a_items) {
				_displayName = safe_string(item->GetDisplayFullName());
				if (!_displayName.empty()) {
					break;
				}
			}

			for (auto& item : a_items) {
				auto ench = item->IsEnchanted();
				if (ench) {
					_flags.set(kEnchanted);
					break;
				}
			}

			for (auto& item : a_items) {
				auto stolen = player->WouldBeStealing(item.get());
				if (stolen) {
					_flags.set(kStolen);
					break;
				}
			}

			for (auto& item : a_items) {
				auto charge = item->GetEnchantmentCharge();
				if (charge) {
					_enchantmentCharge = *charge;
					break;
				}
			}
		}

		[[nodiscard]] inline RE::GFxValue Value() const { return { _displayName }; }

	private:
		enum : std::size_t
		{
			kEnchanted,
			kStolen,
			kTotal
		};

		std::string _displayName;
		std::bitset<kTotal> _flags;
		double _enchantmentCharge;
	};
}
