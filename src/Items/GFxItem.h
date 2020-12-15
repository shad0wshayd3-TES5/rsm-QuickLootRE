#pragma once

namespace Items
{
	class GFxItem
	{
	public:
		GFxItem(std::ptrdiff_t a_count, bool a_stealing, observer<RE::InventoryEntryData*> a_item) :
			_src(a_item),
			_count(a_count),
			_stealing(a_stealing)
		{
			assert(a_item != nullptr);
		}

		GFxItem(std::ptrdiff_t a_count, bool a_stealing, stl::span<const RE::ObjectRefHandle> a_items) :
			_src(a_items),
			_count(a_count),
			_stealing(a_stealing)
		{}

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
		[[nodiscard]] int Compare(const GFxItem& a_rhs) const
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
			} else if (const auto alphabetical = _stricmp(GetDisplayName().c_str(), a_rhs.GetDisplayName().c_str());
					   alphabetical != 0) {
				return alphabetical < 0 ? -1 : 1;
			} else if (GetFormID() != a_rhs.GetFormID()) {
				return GetFormID() < a_rhs.GetFormID() ? -1 : 1;
			} else {
				return 0;
			}
		}

		[[nodiscard]] const std::string& GetDisplayName() const
		{
			if (_cache[kDisplayName]) {
				return _cache.DisplayName();
			}

			std::string result;
			switch (_src.index()) {
			case kInventory:
				result = safe_string(
					std::get<kInventory>(_src)->GetDisplayName());
				break;
			case kGround:
				for (const auto& handle : std::get<kGround>(_src)) {
					const auto item = handle.get();
					result = item ? safe_string(item->GetDisplayFullName()) : ""sv;
					if (!result.empty()) {
						break;
					}
				}
				break;
			default:
				assert(false);
				break;
			}

			_cache.DisplayName(std::move(result));
			return _cache.DisplayName();
		}

		[[nodiscard]] double GetEnchantmentCharge() const
		{
			if (_cache[kEnchantmentCharge]) {
				return _cache.EnchantmentCharge();
			}

			double result = -1.0;
			switch (_src.index()) {
			case kInventory:
				result =
					std::get<kInventory>(_src)
						->GetEnchantmentCharge()
						.value_or(-1.0);
				break;
			case kGround:
				for (const auto& handle : std::get<kGround>(_src)) {
					const auto item = handle.get();
					if (item) {
						const auto charge = item->GetEnchantmentCharge();
						if (charge) {
							result = *charge;
							break;
						}
					}
				}
				break;
			default:
				assert(false);
				break;
			}

			_cache.EnchantmentCharge(result);
			return result;
		}

		[[nodiscard]] RE::FormID GetFormID() const
		{
			if (_cache[kFormID]) {
				return _cache.FormID();
			}

			auto result = std::numeric_limits<RE::FormID>::max();
			switch (_src.index()) {
			case kInventory:
				{
					const auto obj = std::get<kInventory>(_src)->GetObject();
					if (obj) {
						result = obj->GetFormID();
					}
				}
				break;
			case kGround:
				for (const auto& handle : std::get<kGround>(_src)) {
					const auto item = handle.get();
					const auto obj = item ? item->GetObjectReference() : nullptr;
					if (obj) {
						result = obj->GetFormID();
						break;
					}
				}
				break;
			default:
				assert(false);
				break;
			}

			_cache.FormID(result);
			return result;
		}

		[[nodiscard]] std::ptrdiff_t GetValue() const
		{
			if (_cache[kValue]) {
				return _cache.Value();
			}

			auto result = std::numeric_limits<std::ptrdiff_t>::min();
			switch (_src.index()) {
			case kInventory:
				{
					const auto obj = std::get<kInventory>(_src)->GetObject();
					if (obj) {
						result = obj->GetGoldValue() * _count;
					}
				}
				break;
			case kGround:
				for (const auto& handle : std::get<kGround>(_src)) {
					const auto item = handle.get();
					const auto obj = item ? item->GetObjectReference() : nullptr;
					if (obj) {
						result = obj->GetGoldValue() * _count;
						break;
					}
				}
				break;
			default:
				assert(false);
				break;
			}

			_cache.Value(result);
			return result;
		}

		[[nodiscard]] double GetWeight() const
		{
			if (_cache[kWeight]) {
				return _cache.Weight();
			}

			double result = 0.0;
			switch (_src.index()) {
			case kInventory:
				{
					const auto obj = std::get<kInventory>(_src)->GetObject();
					if (obj) {
						result = obj->GetWeight();
					}
				}
				break;
			case kGround:
				for (const auto& handle : std::get<kGround>(_src)) {
					const auto item = handle.get();
					const auto obj = item ? item->GetObjectReference() : nullptr;
					if (obj) {
						result = obj->GetWeight();
						break;
					}
				}
				break;
			default:
				assert(false);
				break;
			}

			_cache.Weight(result);
			return result;
		}

		[[nodiscard]] bool IsAmmo() const
		{
			if (_cache[kAmmo]) {
				return _cache.Ammo();
			}

			bool result = false;
			switch (_src.index()) {
			case kInventory:
				{
					const auto obj = std::get<kInventory>(_src)->GetObject();
					if (obj) {
						result = obj->IsAmmo();
					}
				}
				break;
			case kGround:
				for (const auto& handle : std::get<kGround>(_src)) {
					const auto item = handle.get();
					const auto obj = item ? item->GetObjectReference() : nullptr;
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

			_cache.Ammo(result);
			return result;
		}

		[[nodiscard]] bool IsBook() const
		{
			if (_cache[kBook]) {
				return _cache.Book();
			}

			bool result = false;
			switch (_src.index()) {
			case kInventory:
				{
					const auto obj = std::get<kInventory>(_src)->GetObject();
					if (obj) {
						result = obj->IsObject();
					}
				}
				break;
			case kGround:
				for (const auto& handle : std::get<kGround>(_src)) {
					const auto item = handle.get();
					const auto obj = item ? item->GetObjectReference() : nullptr;
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

			_cache.Book(result);
			return result;
		}

		[[nodiscard]] bool IsGold() const
		{
			if (_cache[kGold]) {
				return _cache.Gold();
			}

			bool result = false;
			switch (_src.index()) {
			case kInventory:
				{
					const auto obj = std::get<kInventory>(_src)->GetObject();
					if (obj) {
						result = obj->IsGold();
					}
				}
				break;
			case kGround:
				for (const auto& handle : std::get<kGround>(_src)) {
					const auto item = handle.get();
					const auto obj = item ? item->GetObjectReference() : nullptr;
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

			_cache.Gold(result);
			return result;
		}

		[[nodiscard]] bool IsKey() const
		{
			if (_cache[kKey]) {
				return _cache.Key();
			}

			bool result = false;
			switch (_src.index()) {
			case kInventory:
				{
					const auto obj = std::get<kInventory>(_src)->GetObject();
					if (obj) {
						result = obj->IsKey();
					}
				}
				break;
			case kGround:
				for (const auto& handle : std::get<kGround>(_src)) {
					const auto item = handle.get();
					const auto obj = item ? item->GetObjectReference() : nullptr;
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

			_cache.Key(result);
			return result;
		}

		[[nodiscard]] bool IsLockpick() const
		{
			if (_cache[kLockpick]) {
				return _cache.Lockpick();
			}

			bool result = false;
			switch (_src.index()) {
			case kInventory:
				{
					const auto obj = std::get<kInventory>(_src)->GetObject();
					if (obj) {
						result = obj->IsLockpick();
					}
				}
				break;
			case kGround:
				for (const auto& handle : std::get<kGround>(_src)) {
					const auto item = handle.get();
					const auto obj = item ? item->GetObjectReference() : nullptr;
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

			_cache.Lockpick(result);
			return result;
		}

		[[nodiscard]] bool IsNote() const
		{
			if (_cache[kNote]) {
				return _cache.Note();
			}

			bool result = false;
			switch (_src.index()) {
			case kInventory:
				{
					const auto obj = std::get<kInventory>(_src)->GetObject();
					if (obj) {
						result = obj->IsNote();
					}
				}
				break;
			case kGround:
				for (const auto& handle : std::get<kGround>(_src)) {
					const auto item = handle.get();
					const auto obj = item ? item->GetObjectReference() : nullptr;
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

			_cache.Note(result);
			return result;
		}

		[[nodiscard]] bool IsQuestItem() const
		{
			if (_cache[kQuestItem]) {
				return _cache.QuestItem();
			}

			bool result = false;
			switch (_src.index()) {
			case kInventory:
				result = std::get<kInventory>(_src)->IsQuestObject();
				break;
			case kGround:
				for (const auto& handle : std::get<kGround>(_src)) {
					const auto item = handle.get();
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

			_cache.QuestItem(result);
			return result;
		}

		[[nodiscard]] bool IsStolen() const
		{
			if (_cache[kStolen]) {
				return _cache.Stolen();
			}

			bool result = false;
			auto player = RE::PlayerCharacter::GetSingleton();
			if (player) {
				switch (_src.index()) {
				case kInventory:
					result = !std::get<kInventory>(_src)->IsOwnedBy(player, !_stealing);
					break;
				case kGround:
					for (const auto& handle : std::get<kGround>(_src)) {
						const auto item = handle.get();
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

			_cache.Stolen(result);
			return result;
		}

		[[nodiscard]] RE::GFxValue GFxValue(RE::GFxMovieView& a_view) const
		{
			RE::GFxValue value;
			a_view.CreateObject(std::addressof(value));
			value.SetMember("displayName", { static_cast<std::string_view>(GetDisplayName()) });
			value.SetMember("count", { _count });
			value.SetMember("stolen", { IsStolen() });
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
			kTotalFlags,

			kDisplayName = kTotalFlags,
			kEnchantmentCharge,
			kWeight,
			kValue,
			kFormID,
			kTotalCachedFlags
		};

		enum : std::size_t
		{
			kInventory,
			kGround
		};

		class Cache
		{
		public:
			[[nodiscard]] bool operator[](std::size_t a_flag) const { return _cached.test(a_flag); }

			[[nodiscard]] bool QuestItem() const { return _flags.test(kQuestItem); }
			[[nodiscard]] void QuestItem(bool a_value) { CacheFlag(kQuestItem, a_value); }

			[[nodiscard]] bool Key() const { return _flags.test(kKey); }
			[[nodiscard]] void Key(bool a_value) { CacheFlag(kKey, a_value); }

			[[nodiscard]] bool Note() const { return _flags.test(kNote); }
			[[nodiscard]] void Note(bool a_value) { CacheFlag(kNote, a_value); }

			[[nodiscard]] bool Book() const { return _flags.test(kBook); }
			[[nodiscard]] void Book(bool a_value) { CacheFlag(kBook, a_value); }

			[[nodiscard]] bool Gold() const { return _flags.test(kGold); }
			[[nodiscard]] void Gold(bool a_value) { CacheFlag(kGold, a_value); }

			[[nodiscard]] bool Ammo() const { return _flags.test(kAmmo); }
			[[nodiscard]] void Ammo(bool a_value) { CacheFlag(kAmmo, a_value); }

			[[nodiscard]] bool Lockpick() const { return _flags.test(kLockpick); }
			[[nodiscard]] void Lockpick(bool a_value) { CacheFlag(kLockpick, a_value); }

			[[nodiscard]] bool Stolen() const { return _flags.test(kStolen); }
			[[nodiscard]] void Stolen(bool a_value) { CacheFlag(kStolen, a_value); }

			[[nodiscard]] constexpr const std::string& DisplayName() const noexcept { return _displayName; }
			[[nodiscard]] void DisplayName(std::string a_value)
			{
				_cached.set(kDisplayName);
				_displayName = std::move(a_value);
			}

			[[nodiscard]] constexpr double EnchantmentCharge() const noexcept { return _enchantmentCharge; }
			[[nodiscard]] void EnchantmentCharge(double a_value)
			{
				_cached.set(kEnchantmentCharge);
				_enchantmentCharge = a_value;
			}

			[[nodiscard]] constexpr double Weight() const noexcept { return _weight; }
			[[nodiscard]] void Weight(double a_value)
			{
				_cached.set(kWeight);
				_weight = a_value;
			}

			[[nodiscard]] constexpr std::ptrdiff_t Value() const noexcept { return _value; }
			[[nodiscard]] void Value(std::ptrdiff_t a_value)
			{
				_cached.set(kValue);
				_value = a_value;
			}

			[[nodiscard]] constexpr RE::FormID FormID() const noexcept { return _formID; }
			[[nodiscard]] void FormID(RE::FormID a_value)
			{
				_cached.set(kFormID);
				_formID = a_value;
			}

		private:
			void CacheFlag(std::size_t a_flag, bool a_value)
			{
				_cached.set(a_flag);
				_flags.set(a_flag, a_value);
			}

			std::string _displayName;
			double _enchantmentCharge{ -1.0 };
			double _weight{ 0.0 };
			std::ptrdiff_t _value{ 0 };
			RE::FormID _formID{ 0 };
			std::bitset<kTotalFlags> _flags;
			std::bitset<kTotalCachedFlags> _cached;
		};

		using inventory_t = RE::InventoryEntryData*;
		using ground_t = stl::span<const RE::ObjectRefHandle>;

		std::variant<inventory_t, ground_t> _src;
		std::ptrdiff_t _count;
		mutable Cache _cache;
		bool _stealing;
	};

	[[nodiscard]] inline bool operator==(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) == 0; }
	[[nodiscard]] inline bool operator!=(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) != 0; }
	[[nodiscard]] inline bool operator<(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) < 0; }
	[[nodiscard]] inline bool operator>(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) > 0; }
	[[nodiscard]] inline bool operator<=(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) <= 0; }
	[[nodiscard]] inline bool operator>=(const GFxItem& a_lhs, const GFxItem& a_rhs) { return a_lhs.Compare(a_rhs) >= 0; }
}
