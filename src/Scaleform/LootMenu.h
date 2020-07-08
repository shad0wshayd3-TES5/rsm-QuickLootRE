#pragma once

#include "CLIK/Array.h"
#include "CLIK/GFx/Controls/ButtonBar.h"
#include "CLIK/GFx/Controls/ScrollingList.h"
#include "CLIK/TextField.h"
#include "ContainerHandler.h"
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
		static constexpr std::string_view MenuName() noexcept { return MENU_NAME; }
		static constexpr SInt8 SortPriority() noexcept { return SORT_PRIORITY; }

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
			const auto maxIdx = static_cast<double>(_itemListImpl.size()) - 1.0;
			if (maxIdx >= 0.0) {
				auto idx = _itemList.SelectedIndex();
				idx += a_mod;
				idx = std::clamp(idx, 0.0, maxIdx);
				_itemList.SelectedIndex(idx);
			}
		}

		inline void SetContainer(RE::ObjectRefHandle a_ref)
		{
			assert(a_ref);
			_src = a_ref;
			_containerHandler.emplace(_src);
		}

		inline void RefreshInventory()
		{
			const auto idx = static_cast<std::ptrdiff_t>(_itemList.SelectedIndex());

			_itemListImpl.clear();
			auto src = _src.get();
			if (!src) {
				_itemListProvider.ClearElements();
				_itemList.Invalidate();
				_itemList.SelectedIndex(-1.0);
				return;
			}

			auto inv = src->GetInventory();
			for (auto& [obj, data] : inv) {
				auto& [count, entry] = data;
				if (CanDisplay(obj) && count > 0 && entry) {
					_itemListImpl.push_back(
						std::make_unique<Items::InventoryItem>(
							count, std::move(entry), _src));
				}
			}

			auto dropped = src->GetDroppedInventory();
			for (auto& [obj, data] : dropped) {
				auto& [count, items] = data;
				if (CanDisplay(obj) && count > 0 && !items.empty()) {
					_itemListImpl.push_back(
						std::make_unique<Items::GroundItems>(
							count, std::move(items)));
				}
			}

			Sort();
			_itemListProvider.ClearElements();
			for (const auto& elem : _itemListImpl) {
				_itemListProvider.PushBack(elem->Value(*_view));
			}
			_itemList.Invalidate();

			RestoreIndex(idx);
			UpdateWeight();
		}

		inline void TakeStack()
		{
			auto pos = static_cast<std::ptrdiff_t>(_itemList.SelectedIndex());
			if (0 <= pos && pos < stl::ssize(_itemListImpl)) {
				auto dst = _dst.get();
				if (dst) {
					_itemListImpl[static_cast<std::size_t>(pos)]->TakeAll(dst.get());
				}
			}

			QueueInventoryRefresh();
		}

	protected:
		using UIResult = RE::UI_MESSAGE_RESULTS;

		inline LootMenu() :
			super(),
			_view(),
			_dst(RE::PlayerCharacter::GetSingleton()),
			_src(),
			_viewHandler(),
			_containerHandler(),
			_rootObj(),
			_weight(),
			_buttonBar(),
			_itemList(),
			_itemListProvider(),
			_itemListImpl()
		{
			using Context = RE::UserEvents::INPUT_CONTEXT_ID;
			using Flag = RE::UI_MENU_FLAGS;

			auto menu = static_cast<super*>(this);
			auto scaleformManager = RE::BSScaleformManager::GetSingleton();
			[[maybe_unused]] const auto success =
				scaleformManager->LoadMovieEx(menu, FILE_NAME, [](RE::GFxMovieDef* a_def) -> void {
					a_def->SetState(
						RE::GFxState::StateType::kLog,
						RE::make_gptr<Logger>().get());
				});

			assert(success);
			_viewHandler.emplace(menu);
			_view = menu->uiMovie;
			_view->SetMouseCursorCount(0);	// disable input, we'll handle it ourselves
			InitExtensions();
		}

		LootMenu(const LootMenu&) = default;
		LootMenu(LootMenu&&) = default;

		~LootMenu() = default;

		LootMenu& operator=(const LootMenu&) = default;
		LootMenu& operator=(LootMenu&&) = default;

		static inline owner<RE::IMenu*> Creator() { return new LootMenu(); }

		// IMenu
		inline void PostCreate() override { OnOpen(); }

		inline UIResult ProcessMessage(RE::UIMessage& a_message) override
		{
			using Type = RE::UI_MESSAGE_TYPE;

			switch (a_message.type) {
			case Type::kHide:
				OnClose();
				return UIResult::kHandled;
			default:
				return super::ProcessMessage(a_message);
			}
		}

		inline void AdvanceMovie(float a_interval, UInt32 a_currentTime) override
		{
			ProcessDelegate();
			super::AdvanceMovie(a_interval, a_currentTime);
		}

	private:
		class Logger :
			public RE::GFxLog
		{
		public:
			inline void LogMessageVarg(LogMessageType, const char* a_fmt, std::va_list a_argList) override
			{
				std::string fmt(a_fmt ? a_fmt : "");
				while (!fmt.empty() && fmt.back() == '\n') {
					fmt.pop_back();
				}

				std::va_list args;
				va_copy(args, a_argList);
				std::vector<char> buf(std::vsnprintf(0, 0, fmt.c_str(), a_argList) + 1);
				std::vsnprintf(buf.data(), buf.size(), fmt.c_str(), args);
				va_end(args);

				_MESSAGE("%s: %s", LootMenu::MenuName().data(), buf.data());
			}
		};

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

		inline void AdjustPosition()
		{
			auto def = _view->GetMovieDef();
			if (def) {
				_rootObj.X(
					_rootObj.X() + def->GetWidth() / 5);
			}
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

		inline void OnClose() { return; }

		inline void OnOpen()
		{
			using element_t = std::pair<std::reference_wrapper<CLIK::Object>, std::string_view>;
			std::array objects{
				element_t{ std::ref(_rootObj), "_root.rootObj" },
				element_t{ std::ref(_itemList), "_root.rootObj.itemList" },
				element_t{ std::ref(_weight), "_root.rootObj.weight" },
				element_t{ std::ref(_buttonBar), "_root.rootObj.buttonBar" }
			};

			for (const auto& [object, path] : objects) {
				auto& instance = object.get().GetInstance();
				[[maybe_unused]] const auto success =
					_view->GetVariable(std::addressof(instance), path.data());
				assert(success && instance.IsObject());
			}

			AdjustPosition();

			_view->CreateArray(std::addressof(_itemListProvider));
			assert(_itemListProvider.IsArray());
			_itemList.DataProvider(CLIK::Array{ _itemListProvider });

			_weight.AutoSize(CLIK::Object{ "left" });

			SetupButtonBar();
			ProcessDelegate();
		}

		void ProcessDelegate();
		void QueueInventoryRefresh();

		inline void RestoreIndex(std::ptrdiff_t a_oldIdx)
		{
			if (const auto ssize = stl::ssize(_itemListImpl); 0 <= a_oldIdx && a_oldIdx < ssize) {
				_itemList.SelectedIndex(static_cast<double>(a_oldIdx));
			} else if (!_itemListImpl.empty()) {
				if (a_oldIdx >= ssize) {
					_itemList.SelectedIndex(static_cast<double>(ssize) - 1.0);
				} else {
					_itemList.SelectedIndex(0.0);
				}
			} else {
				_itemList.SelectedIndex(-1.0);
			}
		}

		inline void SetupButtonBar()
		{
			using namespace std::string_view_literals;

			// TODO: dynamically acquire index
			constexpr std::array data{
				std::make_pair("Take"sv, 18),
				std::make_pair("Transfer"sv, 19)
			};

			RE::GFxValue buttonBarProvider;
			_view->CreateArray(std::addressof(buttonBarProvider));
			for (const auto& elem : data) {
				RE::GFxValue obj;
				_view->CreateObject(std::addressof(obj));
				obj.SetMember("label", { elem.first });
				obj.SetMember("index", { elem.second });
				buttonBarProvider.PushBack(obj);
			}

			_buttonBar.DataProvider(CLIK::Array{ std::move(buttonBarProvider) });
		}

		inline void Sort()
		{
			std::stable_sort(
				_itemListImpl.begin(),
				_itemListImpl.end(),
				[&](auto&& a_lhs, auto&& a_rhs) {
					return *a_lhs < *a_rhs;
				});
		}

		inline void UpdateWeight()
		{
			auto dst = _dst.get();
			if (dst) {
				auto inventoryWeight =
					static_cast<std::ptrdiff_t>(dst->GetWeightInContainer());
				auto carryWeight =
					static_cast<std::ptrdiff_t>(dst->GetActorValue(RE::ActorValue::kCarryWeight));
				auto text = std::to_string(inventoryWeight);
				text += " / ";
				text += std::to_string(carryWeight);
				_weight.HTMLText(text);
			}
		}

		static constexpr std::string_view FILE_NAME{ "LootMenu" };
		static constexpr std::string_view MENU_NAME{ "LootMenu" };
		static constexpr SInt8 SORT_PRIORITY{ 3 };

		RE::GPtr<RE::GFxMovieView> _view;
		RE::ActorHandle _dst;
		RE::ObjectRefHandle _src;
		std::optional<ViewHandler> _viewHandler;
		std::optional<ContainerHandler> _containerHandler;
		CLIK::MovieClip _rootObj;
		CLIK::TextField _weight;
		CLIK::GFx::Controls::ButtonBar _buttonBar;
		CLIK::GFx::Controls::ScrollingList _itemList;
		RE::GFxValue _itemListProvider;
		std::vector<std::unique_ptr<Items::Item>> _itemListImpl;
	};
}
