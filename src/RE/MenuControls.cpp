#include "RE/MenuControls.h"

#include "skse64/GameInput.h"  // MenuControls


namespace RE
{
	MenuControls* MenuControls::GetSingleton()
	{
		return reinterpret_cast<MenuControls*>(::MenuControls::GetSingleton());
	}


	void MenuControls::RegisterHandler(MenuEventHandler* handler)
	{
		CALL_MEMBER_FN(this, RegisterHandler_Impl)(handler);
	}


	void MenuControls::RemoveHandler(MenuEventHandler* handler)
	{
		CALL_MEMBER_FN(this, RemoveHandler_Impl)(handler);
	}
}
