#include "RE/IMenu.h"


namespace RE
{
	IMenu::IMenu() :
		view(0),
		menuDepth(3),
		flags(kFlag_None),
		context(InputManager::kContext_Invalid),
		unk18(0)
	{}


	IMenu::~IMenu()
	{
		CALL_MEMBER_FN(reinterpret_cast<::IMenu*>(this), dtor)();
	}


	void IMenu::Accept(CallbackProcessor* processor)
	{}


	void IMenu::OnOpen()
	{}


	void IMenu::Unk_03(void)
	{}


	UInt32 IMenu::ProcessMessage(UIMessage* message)
	{
		return CALL_MEMBER_FN(reinterpret_cast<::IMenu*>(this), ProcessMessage_internal)(message);
	}


	void IMenu::NextFrame(UInt32 arg0, UInt32 arg1)
	{
		CALL_MEMBER_FN(reinterpret_cast<::IMenu*>(this), NextFrame_internal)(arg0, arg1);
	}


	void IMenu::Render()
	{
		if (view) {
			view->Display();
		}
	}


	void IMenu::Unk_07(void)
	{}


	void IMenu::InitMovie()
	{
		CALL_MEMBER_FN(reinterpret_cast<::IMenu*>(this), InitMovie_internal)(reinterpret_cast<::GFxMovieView*>(view));
	}


	bool IMenu::PausesGame()
	{
		return (flags & kFlag_PauseGame) != 0;
	}


	bool IMenu::DeletesOnClose()
	{
		return (flags & kFlag_DoNotDeleteOnClose) == 0;
	}


	bool IMenu::ShowsCursor()
	{
		return (flags & kFlag_ShowCursor) != 0;
	}


	bool IMenu::HasFlag0008()
	{
		return (flags & kFlag_Unk0008) != 0;
	}


	bool IMenu::IsModal()
	{
		return (flags & kFlag_Modal) != 0;
	}


	bool IMenu::StopsDrawingWorld()
	{
		return (flags & kFlag_StopDrawingWorld) != 0;
	}


	bool IMenu::IsOpen()
	{
		return (flags & kFlag_Open) != 0;
	}


	bool IMenu::PreventsGameLoad()
	{
		return (flags & kFlag_PreventGameLoad) != 0;
	}


	bool IMenu::HasFlag0100()
	{
		return (flags & kFlag_Unk0100) != 0;
	}


	bool IMenu::HidesOtherMenus()
	{
		return (flags & kFlag_HideOther) != 0;
	}


	bool IMenu::HasFlag0400()
	{
		return (flags & kFlag_Unk0400) != 0;
	}


	bool IMenu::PreventsGameSave()
	{
		return (flags & kFlag_DoNotPreventGameSave) == 0;
	}


	bool IMenu::HasFlag1000()
	{
		return (flags & kFlag_Unk1000) != 0;
	}


	bool IMenu::IsItemMenu()
	{
		return (flags & kFlag_ItemMenu) != 0;
	}


	bool IMenu::StopsCrosshairUpdates()
	{
		return (flags & kFlag_StopCrosshairUpdate) != 0;
	}


	bool IMenu::HasFlag8000()
	{
		return (flags & kFlag_Unk8000) != 0;
	}


	bool IMenu::HasFlag10000()
	{
		return (flags & kFlag_Unk10000) != 0;
	}
}
