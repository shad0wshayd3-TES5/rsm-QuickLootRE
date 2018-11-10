#include "RE/MenuControls.h"

#include "skse64/GameInput.h"  // MenuControls


namespace RE
{
	MenuControls* MenuControls::GetSingleton()
	{
		return reinterpret_cast<MenuControls*>(::MenuControls::GetSingleton());
	}


	void MenuControls::RegisterHandler(MenuEventHandler* a_handler)
	{
		_RegisterHandler_Impl(this, a_handler);
	}


	void MenuControls::RemoveHandler(MenuEventHandler* a_handler)
	{
		_RemoveHandler_Impl(this, a_handler);
	}


	RelocAddr<MenuControls::_RegisterHandler_Impl_t*> MenuControls::_RegisterHandler_Impl(MENU_CONTROLS_REGISTER_HANDLER_IMPL);
	RelocAddr<MenuControls::_RemoveHandler_Impl_t*> MenuControls::_RemoveHandler_Impl(MENU_CONTROLS_REMOVE_HANDLER_IMPL);
}
