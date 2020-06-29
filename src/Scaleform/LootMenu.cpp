#include "Scaleform/LootMenu.h"

#include "Loot.h"

namespace Scaleform
{
	void LootMenu::AdvanceMovie(float a_interval, UInt32 a_currentTime)
	{
		auto loot = Loot::GetSingleton();
		loot->Process(*this);
		super::AdvanceMovie(a_interval, a_currentTime);
	}

	void LootMenu::OnOpen()
	{
		using element_t = std::pair<std::reference_wrapper<CLIK::Object>, std::string_view>;
		std::array objects{
			element_t{ std::ref(_itemList), "_root.itemList" }
		};

		for (const auto& [object, path] : objects) {
			auto& instance = object.get().GetInstance();
			[[maybe_unused]] const auto success =
				_view->GetVariable(std::addressof(instance), path.data());
			assert(success && instance.IsObject());
		}

		_view->CreateArray(std::addressof(_itemListProvider));
		assert(_itemListProvider.IsArray());
		CLIK::Array arr{ _itemListProvider };
		_itemList.DataProvider(arr);

		auto loot = Loot::GetSingleton();
		loot->Process(*this);
	}
}
