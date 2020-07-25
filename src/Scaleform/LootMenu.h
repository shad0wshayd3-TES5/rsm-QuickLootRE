#pragma once

#include "CLIK/Array.h"
#include "CLIK/GFx/Controls/ButtonBar.h"
#include "CLIK/GFx/Controls/ScrollingList.h"
#include "CLIK/TextField.h"
#include "ContainerChangedHandler.h"
#include "Items/GroundItems.h"
#include "Items/InventoryItem.h"
#include "Items/Item.h"
#include "OpenCloseHandler.h"
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
		static constexpr std::int8_t SortPriority() noexcept { return SORT_PRIORITY; }

		static inline void Register()
		{
			auto ui = RE::UI::GetSingleton();
			if (ui) {
				ui->Register(MENU_NAME, Creator);
				logger::info("Registered {}", MENU_NAME);
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
				UpdateInfoBar();
			}
		}

		inline void ModSelectedPage(double a_mod)
		{
			auto& inst = _itemList.GetInstance();
			std::array<RE::GFxValue, 1> args;
			args[0] = a_mod;
			[[maybe_unused]] auto success =
				inst.Invoke("modSelectedPage", args);
			assert(success);
			UpdateInfoBar();
		}

		inline void SetContainer(RE::ObjectRefHandle a_ref)
		{
			assert(a_ref);
			_src = a_ref;
			_containerChangedHandler.SetContainer(a_ref);
			_openCloseHandler.SetSource(a_ref);
			UpdateTitle();
			UpdateButtonBar();
			QueueInventoryRefresh();
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

			const auto stealing = WouldBeStealing();
			auto inv = src->GetInventory();
			for (auto& [obj, data] : inv) {
				auto& [count, entry] = data;
				if (CanDisplay(obj) && count > 0 && entry) {
					_itemListImpl.push_back(
						std::make_unique<Items::InventoryItem>(
							count, stealing, std::move(entry), _src));
				}
			}

			auto dropped = src->GetDroppedInventory();
			for (auto& [obj, data] : dropped) {
				auto& [count, items] = data;
				if (CanDisplay(obj) && count > 0 && !items.empty()) {
					_itemListImpl.push_back(
						std::make_unique<Items::GroundItems>(
							count, stealing, std::move(items)));
				}
			}

			Sort();
			_itemListProvider.ClearElements();
			for (const auto& elem : _itemListImpl) {
				_itemListProvider.PushBack(elem->GFxValue(*_view));
			}
			_itemList.InvalidateData();

			RestoreIndex(idx);
			UpdateWeight();
			UpdateInfoBar();
		}

		inline void TakeStack()
		{
			auto pos = static_cast<std::ptrdiff_t>(_itemList.SelectedIndex());
			if (0 <= pos && pos < stl::ssize(_itemListImpl)) {
				auto dst = _dst.get();
				if (dst) {
					_itemListImpl[static_cast<std::size_t>(pos)]->TakeAll(*dst);
					_openCloseHandler.Open();
#if 0  // TODO: handle activating containers
					auto src = _src.get();
					if (src && dst) {
						auto events = RE::ScriptEventSourceHolder::GetSingleton();
						events->SendActivateEvent(src, dst);
						events->SendOpenCloseEvent(src, dst, true);
						events->SendOpenCloseEvent(src, dst, false);
					}
#endif
				}
			}

			QueueInventoryRefresh();
		}

	protected:
		using UIResult = RE::UI_MESSAGE_RESULTS;

		inline LootMenu()
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
			_viewHandler.emplace(menu, _dst);
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

			switch (*a_message.type) {
			case Type::kHide:
				OnClose();
				return UIResult::kHandled;
			default:
				return super::ProcessMessage(a_message);
			}
		}

		inline void AdvanceMovie(float a_interval, std::uint32_t a_currentTime) override
		{
			auto src = _src.get();
			if (!src || src->IsActivationBlocked()) {
				Close();
			}

			ProcessDelegate();
			super::AdvanceMovie(a_interval, a_currentTime);
		}

		inline void RefreshPlatform() override
		{
			UpdateButtonBar();
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
				std::vector<char> buf(static_cast<std::size_t>(std::vsnprintf(0, 0, fmt.c_str(), a_argList) + 1));
				std::vsnprintf(buf.data(), buf.size(), fmt.c_str(), args);
				va_end(args);

				logger::info("{}: {}", LootMenu::MenuName(), buf.data());
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

		void Close();

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
				element_t{ std::ref(_rootObj), "_root.rootObj"sv },
				element_t{ std::ref(_title), "_root.rootObj.title"sv },
				element_t{ std::ref(_weight), "_root.rootObj.weightContainer.textField"sv },
				element_t{ std::ref(_itemList), "_root.rootObj.itemList"sv },
				element_t{ std::ref(_infoBar), "_root.rootObj.infoBar"sv },
				element_t{ std::ref(_buttonBar), "_root.rootObj.buttonBar"sv }
			};

			for (const auto& [object, path] : objects) {
				auto& instance = object.get().GetInstance();
				[[maybe_unused]] const auto success =
					_view->GetVariable(std::addressof(instance), path.data());
				assert(success && instance.IsObject());
			}

			AdjustPosition();

			_title.AutoSize(CLIK::Object{ "left" });
			_title.Visible(false);
			_weight.AutoSize(CLIK::Object{ "left" });
			_weight.Visible(false);

			_view->CreateArray(std::addressof(_itemListProvider));
			_itemList.DataProvider(CLIK::Array{ _itemListProvider });

			_view->CreateArray(std::addressof(_infoBarProvider));
			_infoBar.DataProvider(CLIK::Array{ _infoBarProvider });

			_view->CreateArray(std::addressof(_buttonBarProvider));
			_buttonBar.DataProvider(CLIK::Array{ _buttonBarProvider });

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

		inline void Sort()
		{
			std::stable_sort(
				_itemListImpl.begin(),
				_itemListImpl.end(),
				[&](auto&& a_lhs, auto&& a_rhs) {
					return *a_lhs < *a_rhs;
				});
		}

		inline void UpdateButtonBar()
		{
			if (!_view) {
				return;
			}

			const bool stealing = WouldBeStealing();
			const std::array mappings{
				std::make_tuple(stealing ? "sSteal"sv : "sTake"sv, "Activate"sv, stealing),
				std::make_tuple("sSearch"sv, "Ready Weapon"sv, stealing)
			};

			_buttonBarProvider.ClearElements();
			auto gmst = RE::GameSettingCollection::GetSingleton();
			const boost::regex pattern("<.*>(.*)<.*>", boost::regex_constants::ECMAScript);
			for (std::size_t i = 0; i < mappings.size(); ++i) {
				const auto& mapping = mappings[i];

				auto setting = gmst->GetSetting(std::get<0>(mapping).data());
				std::string label = setting ? setting->GetString() : "<undefined>"s;
				boost::smatch matches;
				if (boost::regex_match(label, matches, pattern)) {
					if (matches.size() >= 2) {
						assert(matches.size() == 2);
						label = matches[1].str();
					}
				}

				const auto index =
					static_cast<std::ptrdiff_t>(
						Input::ControlMap()(std::get<1>(mapping)));
				const auto doColor = std::get<2>(mapping);

				RE::GFxValue obj;
				_view->CreateObject(std::addressof(obj));
				obj.SetMember("label", { static_cast<std::string_view>(label) });
				obj.SetMember("index", { index });
				obj.SetMember("doColor", { doColor });
				_buttonBarProvider.PushBack(obj);
			}
			_buttonBar.InvalidateData();
		}

		inline void UpdateInfoBar()
		{
			_infoBarProvider.ClearElements();
			const auto idx = static_cast<std::ptrdiff_t>(_itemList.SelectedIndex());
			if (0 <= idx && idx < stl::ssize(_itemListImpl)) {
				const std::array functors{
					std::function{ [](const Items::Item& a_val) { return fmt::format(FMT_STRING("{:.1f}"), a_val.Weight()); } },
					std::function{ [](const Items::Item& a_val) { return fmt::format(FMT_STRING("{}"), a_val.Value()); } },
				};

				const auto& item = _itemListImpl[static_cast<std::size_t>(idx)];
				std::string str;
				RE::GFxValue obj;
				for (const auto& functor : functors) {
					str = functor(*item);
					obj.SetString(str);
					_infoBarProvider.PushBack(obj);
				}

				const auto ench = item->EnchantmentCharge();
				if (ench >= 0.0) {
					str = fmt::format(FMT_STRING("{:.1f}%"), ench);
					obj.SetString(str);
					_infoBarProvider.PushBack(obj);
				}
			}

			_infoBar.InvalidateData();
		}

		inline void UpdateTitle()
		{
			auto src = _src.get();
			if (src) {
				_title.HTMLText(
					safe_string(
						src->GetDisplayFullName()));
				_title.Visible(true);
			}
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
				_weight.Visible(true);
			}
		}

		[[nodiscard]] inline bool WouldBeStealing() const
		{
			auto dst = _dst.get();
			auto src = _src.get();
			return dst && src && dst->WouldBeStealing(src.get());
		}

		static constexpr std::string_view FILE_NAME{ "LootMenu" };
		static constexpr std::string_view MENU_NAME{ "LootMenu" };
		static constexpr std::int8_t SORT_PRIORITY{ 3 };

		RE::GPtr<RE::GFxMovieView> _view;
		RE::ActorHandle _dst{ RE::PlayerCharacter::GetSingleton() };
		RE::ObjectRefHandle _src;

		std::optional<ViewHandler> _viewHandler;
		ContainerChangedHandler _containerChangedHandler;
		OpenCloseHandler _openCloseHandler{ _dst };

		CLIK::MovieClip _rootObj;
		CLIK::TextField _title;
		CLIK::TextField _weight;

		CLIK::GFx::Controls::ScrollingList _itemList;
		RE::GFxValue _itemListProvider;
		std::vector<std::unique_ptr<Items::Item>> _itemListImpl;

		CLIK::GFx::Controls::ButtonBar _infoBar;
		RE::GFxValue _infoBarProvider;

		CLIK::GFx::Controls::ButtonBar _buttonBar;
		RE::GFxValue _buttonBarProvider;
	};
}
