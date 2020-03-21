#include "LootMenu.h"

#include <array>
#include <functional>
#include <memory>
#include <string_view>
#include <utility>

#include "RE/Skyrim.h"

#include "CLIK/Array.h"


namespace Scaleform
{
	void LootMenu::Close()
	{
		auto msgQ = RE::UIMessageQueue::GetSingleton();
		msgQ->AddMessage(MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
	}


	void LootMenu::Open()
	{
		auto msgQ = RE::UIMessageQueue::GetSingleton();
		msgQ->AddMessage(MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
	}


	void LootMenu::Register()
	{
		auto ui = RE::UI::GetSingleton();
		ui->Register(MENU_NAME, Creator);
		_MESSAGE("Registered %s", MENU_NAME.data());
	}


	LootMenu::LootMenu() :
		_view(nullptr),
		_itemList()
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

		CLIK::Array arr(_view);
		CLIK::Object elem;
		for (std::size_t i = 0; i < 10; ++i) {
			elem = "Foobar";
			arr.Push(elem);
		}
		_itemList.DataProvider(arr);
	}


	void LootMenu::OnClose()
	{
		return;
	}
}
