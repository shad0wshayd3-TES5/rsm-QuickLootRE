#include "RE/UIManager.h"


namespace RE
{
	UIManager* UIManager::GetSingleton()
	{
		return reinterpret_cast<UIManager*>(::UIManager::GetSingleton());
	}


	void UIManager::AddMessage(BSFixedString& a_menuName, UInt32 a_msgID, IUIMessageData* a_pData)
	{
		CALL_MEMBER_FN(reinterpret_cast<::UIManager*>(this), AddMessage)(&a_menuName, a_msgID, a_pData);
	}


	IUIMessageData* UIManager::CreateUIMessageData(const BSFixedString& a_name)
	{
		return CALL_MEMBER_FN(reinterpret_cast<::UIManager*>(this), CreateUIMessageData)(a_name);
	}


	void UIManager::ProcessCommands()
	{
		reinterpret_cast<::UIManager*>(this)->ProcessCommands();
	}


	void UIManager::QueueCommand(UIDelegate* a_cmd)
	{
		reinterpret_cast<::UIManager*>(this)->QueueCommand(a_cmd);
	}


	void UIManager::QueueCommand(UIDelegate_v1* a_cmd)
	{
		reinterpret_cast<::UIManager*>(this)->QueueCommand(a_cmd);
	}
}
