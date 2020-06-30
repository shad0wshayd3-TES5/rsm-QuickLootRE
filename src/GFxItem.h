#pragma once

class GFxItem
{
public:
	inline GFxItem(observer<RE::InventoryEntryData*> a_item) :
		_displayName(),
		_flags()
	{
		assert(a_item != nullptr);

		_displayName = safe_string(a_item->GetDisplayName());
		_flags.set(kEnchanted, a_item->IsEnchanted());
	}

	inline GFxItem(const std::vector<RE::NiPointer<RE::TESObjectREFR>>& a_items) :
		_displayName(),
		_flags()
	{
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
	}

	[[nodiscard]] inline RE::GFxValue Value() const { return { _displayName }; }

private:
	enum : std::size_t
	{
		kEnchanted,
		kTotal
	};

	std::string _displayName;
	std::bitset<kTotal> _flags;
};
