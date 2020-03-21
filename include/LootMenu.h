#pragma once

#include <string_view>

#include "RE/Skyrim.h"

#include "CLIK/ScrollingList.h"


namespace Scaleform
{
	class LootMenu : public RE::IMenu
	{
	public:
		static void Close();
		static void Open();
		static void Register();

	protected:
		using UIResult = RE::UI_MESSAGE_RESULTS;


		LootMenu();
		virtual ~LootMenu() = default;


		static RE::IMenu* Creator();

		// IMenu
		virtual UIResult ProcessMessage(RE::UIMessage& a_message) override;

	private:
		void InitExtensions();
		void OnOpen();
		void OnClose();


		static constexpr std::string_view FILE_NAME = "LootMenu";
		static constexpr std::string_view MENU_NAME = "LootMenu";

		RE::GPtr<RE::GFxMovieView> _view;
		CLIK::GFx::Controls::ScrollingList _itemList;
	};
}
