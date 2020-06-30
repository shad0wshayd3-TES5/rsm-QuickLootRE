#pragma once

#include "CLIK/Array.h"
#include "CLIK/GFx/Controls/ScrollingList.h"
#include "Items/GroundItems.h"
#include "Items/InventoryItem.h"
#include "Items/Item.h"
#include "ViewHandler.h"

namespace Scaleform
{
	class LootMenu :
		public RE::IMenu
	{
	private:
		using super = RE::IMenu;

	public:
		static constexpr std::string_view MenuName() { return MENU_NAME; }

		static inline void Register()
		{
			auto ui = RE::UI::GetSingleton();
			if (ui) {
				ui->Register(MENU_NAME, Creator);
				_MESSAGE("Registered %s", MENU_NAME.data());
			}
		}

		inline void ModSelectedIndex(double a_mod)
		{
			const auto maxIdx = static_cast<double>(_itemListImpl.size() - 1);
			if (maxIdx >= 0.0) {
				auto idx = _itemList.SelectedIndex();
				idx += a_mod;
				idx = std::clamp(idx, 0.0, maxIdx);
				_itemList.SelectedIndex(idx);
			}
		}

		inline void ProcessRef(RE::TESObjectREFRPtr a_ref)
		{
			assert(a_ref);
			_itemListImpl.clear();

			auto inv = a_ref->GetInventory();
			for (auto& [obj, data] : inv) {
				auto& [count, entry] = data;
				if (CanDisplay(obj) && count > 0 && entry) {
					_itemListImpl.push_back(
						std::make_unique<Items::InventoryItem>(
							std::move(entry), count, a_ref));
				}
			}

			auto dropped = a_ref->GetDroppedInventory();
			for (auto& [obj, data] : dropped) {
				auto& [count, items] = data;
				if (CanDisplay(obj) && count > 0 && !items.empty()) {
					_itemListImpl.push_back(
						std::make_unique<Items::GroundItems>(
							std::move(items), count));
				}
			}

			_itemListProvider.ClearElements();
			for (const auto& elem : _itemListImpl) {
				_itemListProvider.PushBack(elem->Value());
			}
			_itemList.Invalidate();

			if (!_itemListImpl.empty()) {
				_itemList.SelectedIndex(0.0);
			}
		}

		inline void TakeStack()
		{
			if (!_itemListImpl.empty()) {
				auto pos = static_cast<std::ptrdiff_t>(_itemList.SelectedIndex());
				if (0 <= pos && pos < stl::ssize(_itemListImpl)) {
					_itemListImpl[static_cast<std::size_t>(pos)]->TakeAll(_dest.get());
					_itemListImpl.erase(_itemListImpl.begin() + pos);
					_itemListProvider.RemoveElement(static_cast<UInt32>(pos));
					_itemList.Invalidate();
				}
			}
		}

	protected:
		using UIResult = RE::UI_MESSAGE_RESULTS;

		inline LootMenu() :
			super(),
			_view(),
			_dest(RE::PlayerCharacter::GetSingleton()),
			_viewHandler(std::nullopt),
			_itemList(),
			_itemListProvider(),
			_itemListImpl()
		{
			using Context = RE::UserEvents::INPUT_CONTEXT_ID;
			using Flag = RE::UI_MENU_FLAGS;

			auto menu = static_cast<super*>(this);
			menu->menuDepth = 0;
			//menu->flags = Flag::kUpdateUsesCursor | Flag::kUsesCursor;
			//menu->context = Context::kGameplay;

			auto scaleformManager = RE::BSScaleformManager::GetSingleton();
			[[maybe_unused]] const auto success =
				scaleformManager->LoadMovieEx(menu, FILE_NAME, [](RE::GFxMovieDef*) -> void {
					return;
				});

			assert(success);
			_viewHandler.emplace(menu->view);
			_view = menu->view;
			_view->SetMouseCursorCount(0);	// disable input, we'll handle it ourselves
			InitExtensions();

			if (!_dest->extraList.HasType<RE::ExtraContainerChanges>()) {
				_dest->InitInventoryIfRequired();
			}
		}

		LootMenu(const LootMenu&) = default;
		LootMenu(LootMenu&&) = default;

		inline ~LootMenu() {}

		LootMenu& operator=(const LootMenu&) = default;
		LootMenu& operator=(LootMenu&&) = default;

		static inline owner<RE::IMenu*> Creator() { return new LootMenu(); }

		// IMenu
		inline UIResult ProcessMessage(RE::UIMessage& a_message) override
		{
			using Type = RE::UI_MESSAGE_TYPE;

			switch (a_message.type) {
			case Type::kShow:
				OnOpen();
				return UIResult::kHandled;
			case Type::kHide:
				OnClose();
				return UIResult::kHandled;
			default:
				return RE::IMenu::ProcessMessage(a_message);
			}
		}

		inline void AdvanceMovie(float a_interval, UInt32 a_currentTime) override
		{
			ProcessDelegate();
			super::AdvanceMovie(a_interval, a_currentTime);
		}

	private:
		[[nodiscard]] static inline bool CanDisplay(RE::TESBoundObject* a_object)
		{
			if (!a_object) {
				return false;
			}

			switch (a_object->GetFormType()) {
			case RE::FormType::Scroll:
			case RE::FormType::Armor:
			case RE::FormType::Book:
			case RE::FormType::Ingredient:
			case RE::FormType::Misc:
			case RE::FormType::Weapon:
			case RE::FormType::Ammo:
			case RE::FormType::KeyMaster:
			case RE::FormType::AlchemyItem:
			case RE::FormType::Note:
			case RE::FormType::SoulGem:
				break;
			case RE::FormType::Light:
				{
					auto light = static_cast<RE::TESObjectLIGH*>(a_object);
					if (!light->CanBeCarried()) {
						return false;
					}
				}
				break;
			default:
				return false;
			}

			if (!a_object->GetPlayable()) {
				return false;
			}

			auto name = a_object->GetName();
			if (!name || name[0] == '\0') {
				return false;
			}

			return true;
		}

		inline void InitExtensions()
		{
			const RE::GFxValue boolean{ true };
			bool success;

			success = _view->SetVariable("_global.gfxExtensions", boolean);
			assert(success);
			success = _view->SetVariable("_global.noInvisibleAdvance", boolean);
			assert(success);
		}

		inline void OnOpen()
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
			_itemList.DataProvider(CLIK::Array{ _itemListProvider });

			ProcessDelegate();
		}

		inline void OnClose() { return; }

		void ProcessDelegate();

		static constexpr std::string_view FILE_NAME{ "LootMenu" };
		static constexpr std::string_view MENU_NAME{ "LootMenu" };

		RE::GPtr<RE::GFxMovieView> _view;
		RE::ActorPtr _dest;
		std::optional<ViewHandler> _viewHandler;
		CLIK::GFx::Controls::ScrollingList _itemList;
		RE::GFxValue _itemListProvider;
		std::vector<std::unique_ptr<Items::Item>> _itemListImpl;
	};
}
