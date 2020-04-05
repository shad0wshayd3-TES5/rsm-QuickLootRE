#pragma once

#include <string_view>

#include "RE/Skyrim.h"

#include "CLIK/ScrollingList.h"
#include "InputDisablers.h"
#include "InputListeners.h"
#include "InventoryItem.h"


namespace Scaleform
{
	class LootMenu : public RE::IMenu
	{
	public:
		static constexpr std::string_view MenuName() { return MENU_NAME; }
		static void Register();

		void ModSelectedIndex(double a_mod);
		void ProcessRef(RE::TESObjectREFRPtr a_ref);

	protected:
		using UIResult = RE::UI_MESSAGE_RESULTS;

		LootMenu();
		virtual ~LootMenu() = default;

		static RE::IMenu* Creator();

		// IMenu
		virtual UIResult ProcessMessage(RE::UIMessage& a_message) override;
		virtual void AdvanceMovie(float a_interval, UInt32 a_currentTime) override;

	private:
		using super = RE::IMenu;

		void InitExtensions();
		void OnOpen();
		void OnClose();

		static constexpr std::string_view FILE_NAME = "LootMenu";
		static constexpr std::string_view MENU_NAME = "LootMenu";

		RE::GPtr<RE::GFxMovieView> _view;
		Input::Disablers _inputDisablers;
		Input::Listeners _inputListeners;
		CLIK::GFx::Controls::ScrollingList _itemList;
		std::vector<InventoryItem> _itemListImpl;
	};
}
