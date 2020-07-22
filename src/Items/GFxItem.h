#pragma once

namespace Items
{
	class GFxItem
	{
	public:
		inline GFxItem(std::ptrdiff_t a_count, bool a_stealing, observer<RE::InventoryEntryData*> a_item) :
			_src(a_item),
			_count(a_count),
			_stealing(a_stealing)
		{
			assert(a_item != nullptr);

			_displayName = safe_string(a_item->GetDisplayName());
			_enchantmentCharge = a_item->GetEnchantmentCharge();
		}

		inline GFxItem(std::ptrdiff_t a_count, bool a_stealing, stl::span<const RE::ObjectRefHandle> a_items) :
			_src(a_items),
			_count(a_count),
			_stealing(a_stealing)
		{
			std::vector<RE::TESObjectREFRPtr> items;
			items.reserve(a_items.size());
			for (auto& item : a_items) {
				auto ptr = item.get();
				if (ptr) {
					items.push_back(std::move(ptr));
				}
			}

			for (auto& item : items) {
				_displayName = safe_string(item->GetDisplayFullName());
				if (!_displayName.empty()) {
					break;
				}
			}

			for (auto& item : items) {
				_enchantmentCharge = item->GetEnchantmentCharge();
				if (_enchantmentCharge) {
					break;
				}
			}
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
			if (IsQuestItem() != a_rhs.IsQuestItem()) {
				return IsQuestItem() ? -1 : 1;
			} else if (IsKey() != a_rhs.IsKey()) {
				return IsKey() ? -1 : 1;
			} else if (IsNote() != a_rhs.IsNote()) {
				return IsNote() ? -1 : 1;
			} else if (IsBook() != a_rhs.IsBook()) {
				return IsBook() ? -1 : 1;
			} else if (IsGold() != a_rhs.IsGold()) {
				return IsGold() ? -1 : 1;
			} else if (IsAmmo() != a_rhs.IsAmmo()) {
				return IsAmmo() ? -1 : 1;
			} else if (IsLockpick() != a_rhs.IsLockpick()) {
				return IsLockpick() ? -1 : 1;
			} else if (GetValue() != a_rhs.GetValue()) {
				return GetValue() > a_rhs.GetValue() ? -1 : 1;
			} else if (const auto alphabetical = _stricmp(_displayName.c_str(), a_rhs._displayName.c_str());
					   alphabetical != 0) {
				return alphabetical < 0 ? -1 : 1;
			} else if (GetFormID() != a_rhs.GetFormID()) {
				return GetFormID() < a_rhs.GetFormID() ? -1 : 1;
			} else {
				return 0;
			}
		}

		[[nodiscard]] inline RE::FormID GetFormID() const
		{
			if (_formID) {
				return *_formID;
			}

			_formID = std::numeric_limits<RE::FormID>::max();
			switch (_src.index()) {
			case kInventory:
				{
					auto obj = std::get<kInventory>(_src)->GetObject();
					if (obj) {
						_formID = obj->GetFormID();
					}
				}
				break;
			case kGround:
				for (auto& handle : std::get<kGround>(_src)) {
					auto item = handle.get();
					auto obj = item ? item->GetObjectReference() : nullptr;
					if (obj) {
						_value = obj->GetFormID();
						break;
					}
				}
				break;
			default:
				assert(false);
				break;
			}

			return *_formID;
		}

		[[nodiscard]] inline std::ptrdiff_t GetValue() const
		{
			if (_value) {
				return *_value;
			}

			_value = std::numeric_limits<std::ptrdiff_t>::min();
			switch (_src.index()) {
			case kInventory:
				{
					auto obj = std::get<kInventory>(_src)->GetObject();
					if (obj) {
						_value = obj->GetGoldValue() * _count;
					}
				}
				break;
			case kGround:
				for (auto& handle : std::get<kGround>(_src)) {
					auto item = handle.get();
					auto obj = item ? item->GetObjectReference() : nullptr;
					if (obj) {
						_value = obj->GetGoldValue() * _count;
						break;
					}
				}
				break;
			default:
				assert(false);
				break;
			}

			return *_value;
		}

		[[nodiscard]] inline bool IsAmmo() const
		{
			if (_cached[kAmmo]) {
				return _flags[kAmmo];
			}

			bool result = false;
			switch (_src.index()) {
			case kInventory:
				{
					auto obj = std::get<kInventory>(_src)->GetObject();
					if (obj) {
						result = obj->IsAmmo();
					}
				}
				break;
			case kGround:
				for (auto& handle : std::get<kGround>(_src)) {
					auto item = handle.get();
					auto obj = item ? item->GetObjectReference() : nullptr;
					if (obj) {
						result = obj->IsAmmo();
						break;
					}
				}
				break;
			default:
				assert(false);
				break;
			}

			_cached.set(kAmmo);
			_flags.set(kAmmo, result);
			return result;
		}

		[[nodiscard]] inline bool IsBook() const
		{
			if (_cached[kBook]) {
				return _flags[kBook];
			}

			bool result = false;
			switch (_src.index()) {
			case kInventory:
				{
					auto obj = std::get<kInventory>(_src)->GetObject();
					if (obj) {
						result = obj->IsObject();
					}
				}
				break;
			case kGround:
				for (auto& handle : std::get<kGround>(_src)) {
					auto item = handle.get();
					auto obj = item ? item->GetObjectReference() : nullptr;
					if (obj) {
						result = obj->IsBook();
						break;
					}
				}
				break;
			default:
				assert(false);
				break;
			}

			_cached.set(kBook);
			_flags.set(kBook, result);
			return result;
		}

		[[nodiscard]] inline bool IsGold() const
		{
			if (_cached[kGold]) {
				return _flags[kGold];
			}

			bool result = false;
			switch (_src.index()) {
			case kInventory:
				{
					auto obj = std::get<kInventory>(_src)->GetObject();
					if (obj) {
						result = obj->IsGold();
					}
				}
				break;
			case kGround:
				for (auto& handle : std::get<kGround>(_src)) {
					auto item = handle.get();
					auto obj = item ? item->GetObjectReference() : nullptr;
					if (obj) {
						result = obj->IsGold();
						break;
					}
				}
				break;
			default:
				assert(false);
				break;
			}

			_cached.set(kGold);
			_flags.set(kGold, result);
			return result;
		}

		[[nodiscard]] inline bool IsKey() const
		{
			if (_cached[kKey]) {
				return _flags[kKey];
			}

			bool result = false;
			switch (_src.index()) {
			case kInventory:
				{
					auto obj = std::get<kInventory>(_src)->GetObject();
					if (obj) {
						result = obj->IsKey();
					}
				}
				break;
			case kGround:
				for (auto& handle : std::get<kGround>(_src)) {
					auto item = handle.get();
					auto obj = item ? item->GetObjectReference() : nullptr;
					if (obj) {
						result = obj->IsKey();
						break;
					}
				}
				break;
			default:
				assert(false);
				break;
			}

			_cached.set(kKey);
			_flags.set(kKey, result);
			return result;
		}

		[[nodiscard]] inline bool IsLockpick() const
		{
			if (_cached[kLockpick]) {
				return _flags[kLockpick];
			}

			bool result = false;
			switch (_src.index()) {
			case kInventory:
				{
					auto obj = std::get<kInventory>(_src)->GetObject();
					if (obj) {
						result = obj->IsLockpick();
					}
				}
				break;
			case kGround:
				for (auto& handle : std::get<kGround>(_src)) {
					auto item = handle.get();
					auto obj = item ? item->GetObjectReference() : nullptr;
					if (obj) {
						result = obj->IsLockpick();
						break;
					}
				}
				break;
			default:
				assert(false);
				break;
			}

			_cached.set(kLockpick);
			_flags.set(kLockpick, result);
			return result;
		}

		[[nodiscard]] inline bool IsNote() const
		{
			if (_cached[kNote]) {
				return _flags[kNote];
			}

			bool result = false;
			switch (_src.index()) {
			case kInventory:
				{
					auto obj = std::get<kInventory>(_src)->GetObject();
					if (obj) {
						result = obj->IsNote();
					}
				}
				break;
			case kGround:
				for (auto& handle : std::get<kGround>(_src)) {
					auto item = handle.get();
					auto obj = item ? item->GetObjectReference() : nullptr;
					if (obj) {
						result = obj->IsNote();
						break;
					}
				}
				break;
			default:
				assert(false);
				break;
			}

			_cached.set(kNote);
			_flags.set(kNote, result);
			return result;
		}

		[[nodiscard]] inline bool IsQuestItem() const
		{
			if (_cached[kQuestItem]) {
				return _flags[kQuestItem];
			}

			bool result = false;
			switch (_src.index()) {
			case kInventory:
				result = std::get<kInventory>(_src)->IsQuestObject();
				break;
			case kGround:
				for (auto& handle : std::get<kGround>(_src)) {
					auto item = handle.get();
					if (item && item->HasQuestObject()) {
						result = true;
						break;
					}
				}
				break;
			default:
				assert(false);
				break;
			}

			_cached.set(kQuestItem);
			_flags.set(kQuestItem, result);
			return result;
		}

		[[nodiscard]] inline bool IsStolen() const
		{
			if (_cached[kStolen]) {
				return _flags[kStolen];
			}

			bool result = false;
			auto player = RE::PlayerCharacter::GetSingleton();
			if (player) {
				switch (_src.index()) {
				case kInventory:
					result = !std::get<kInventory>(_src)->IsOwnedBy(player, !_stealing);
					break;
				case kGround:
					for (auto& handle : std::get<kGround>(_src)) {
						auto item = handle.get();
						if (item && item->IsCrimeToActivate()) {
							result = true;
							break;
						}
					}
					break;
				default:
					assert(false);
					break;
				}
			}

			_cached.set(kStolen);
			_flags.set(kStolen, result);
			return result;
		}

		[[nodiscard]] inline RE::GFxValue GFxValue(RE::GFxMovieView& a_view) const
		{
			RE::GFxValue value;
			a_view.CreateObject(std::addressof(value));
			value.SetMember("displayName", { static_cast<std::string_view>(_displayName) });
			value.SetMember("count", { _count });
			value.SetMember("doColor", { IsStolen() });
			if (_enchantmentCharge) {
				value.SetMember("enchantmentCharge", { *_enchantmentCharge });
			}
			return value;
		}

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
			kStolen,
			kTotal
		};

		enum : std::size_t
		{
			kInventory,
			kGround
		};

		using inventory_t = RE::InventoryEntryData*;
		using ground_t = stl::span<const RE::ObjectRefHandle>;

		std::variant<inventory_t, ground_t> _src;
		std::string _displayName;
		std::ptrdiff_t _count;
		std::optional<double> _enchantmentCharge;
		mutable std::optional<std::ptrdiff_t> _value;
		mutable std::optional<RE::FormID> _formID;
		mutable std::bitset<kTotal> _flags;
		mutable std::bitset<kTotal> _cached;
		bool _stealing;
	};

	[[nodiscard]] inline bool operator==(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) == 0; }
	[[nodiscard]] inline bool operator!=(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) != 0; }
	[[nodiscard]] inline bool operator<(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) < 0; }
	[[nodiscard]] inline bool operator>(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) > 0; }
	[[nodiscard]] inline bool operator<=(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) <= 0; }
	[[nodiscard]] inline bool operator>=(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) >= 0; }
}
