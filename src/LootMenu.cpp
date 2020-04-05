#include "LootMenu.h"

#include <array>
#include <cassert>
#include <functional>
#include <memory>
#include <string_view>
#include <utility>

#include "RE/Skyrim.h"

#include "CLIK/Array.h"
#include "Loot.h"


namespace Scaleform
{
	void LootMenu::Register()
	{
		auto ui = RE::UI::GetSingleton();
		if (ui) {
			ui->Register(MENU_NAME, Creator);
			_MESSAGE("Registered %s", MENU_NAME.data());
		}
	}


	void LootMenu::ModSelectedIndex(double a_mod)
	{
		const auto maxIdx = static_cast<double>(_itemListImpl.size() - 1);
		if (maxIdx >= 0.0) {
			auto idx = _itemList.SelectedIndex();
			idx += a_mod;
			idx = std::clamp(idx, 0.0, maxIdx);
			_itemList.SelectedIndex(idx);
		}
	}


	void LootMenu::ProcessRef(RE::TESObjectREFRPtr a_ref)
	{
		assert(a_ref);
		_itemListImpl.clear();

		auto inv = a_ref->GetInventory();
		for (auto& item : inv) {
			auto& [count, entry] = item.second;
			if (count > 0) {
				_itemListImpl.emplace_back(std::move(entry), count, a_ref);
			}
		}

		CLIK::Array arr(_view);
		for (auto& elem : _itemListImpl) {
			auto obj = elem.Object();
			arr.Push(obj);
		}
		_itemList.DataProvider(arr);

		if (!_itemListImpl.empty()) {
			_itemList.SelectedIndex(0.0);
		}
	}


	LootMenu::LootMenu() :
		super(),
		_view(nullptr),
		_inputDisablers(),
		_inputListeners(),
		_itemList(),
		_itemListImpl()
	{
		using Context = RE::UserEvents::INPUT_CONTEXT_ID;
		using Flag = RE::UI_MENU_FLAGS;

		auto menu = static_cast<RE::IMenu*>(this);
		//menu->flags = Flag::kUpdateUsesCursor | Flag::kUsesCursor;
		//menu->context = Context::kGameplay;

		auto scaleformManager = RE::BSScaleformManager::GetSingleton();
		auto success = scaleformManager->LoadMovieEx(menu, FILE_NAME, [](RE::GFxMovieDef*) -> void {
			return;
		});

		assert(success);
		_view = menu->view;
		_view->SetMouseCursorCount(0);	// disable input, we'll handle it ourselves
		InitExtensions();
	}


	RE::IMenu* LootMenu::Creator()
	{
		return new LootMenu();
	}


	auto LootMenu::ProcessMessage(RE::UIMessage& a_message)
		-> UIResult
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


	void LootMenu::AdvanceMovie(float a_interval, UInt32 a_currentTime)
	{
		auto loot = Loot::GetSingleton();
		loot->Process(*this);
		super::AdvanceMovie(a_interval, a_currentTime);
	}


	void LootMenu::InitExtensions()
	{
		RE::GFxValue boolean(true);
		bool success;

		success = _view->SetVariable("_global.gfxExtensions", boolean);
		assert(success);
		success = _view->SetVariable("_global.noInvisibleAdvance", boolean);
		assert(success);
	}


	void LootMenu::OnOpen()
	{
		std::array<std::pair<std::reference_wrapper<CLIK::Object>, std::string_view>, 1> objects = {
			std::make_pair(std::ref(_itemList), "_root.itemList")
		};

		for (auto& [object, path] : objects) {
			auto& instance = object.get().GetInstance();
			auto success = _view->GetVariable(std::addressof(instance), path.data());
			assert(success && instance.IsObject());
		}

		auto loot = Loot::GetSingleton();
		loot->Process(*this);
	}


	void LootMenu::OnClose()
	{
		return;
	}
}
