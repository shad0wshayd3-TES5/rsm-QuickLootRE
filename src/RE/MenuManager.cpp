#include "RE/MenuManager.h"


namespace RE
{
	MenuManager* MenuManager::GetSingleton(void)
	{
		return reinterpret_cast<MenuManager*>(::MenuManager::GetSingleton());
	}


	EventDispatcher<MenuOpenCloseEvent>* MenuManager::MenuOpenCloseEventDispatcher()
	{
		return reinterpret_cast<::MenuManager*>(this)->MenuOpenCloseEventDispatcher();
	}


	bool MenuManager::IsMenuOpen(BSFixedString* menuName)
	{
		return reinterpret_cast<::MenuManager*>(this)->IsMenuOpen(menuName);
	}


	IMenu* MenuManager::GetMenu(BSFixedString* menuName)
	{
		return reinterpret_cast<IMenu*>(reinterpret_cast<::MenuManager*>(this)->GetMenu(menuName));
	}


	GFxMovieView* MenuManager::GetMovieView(BSFixedString* menuName)
	{
		return reinterpret_cast<GFxMovieView*>(reinterpret_cast<::MenuManager*>(this)->GetMovieView(menuName));
	}


	void MenuManager::ShowMenus(bool show)
	{
		reinterpret_cast<::MenuManager*>(this)->ShowMenus(show);
	}


	bool MenuManager::IsShowingMenus()
	{
		return reinterpret_cast<::MenuManager*>(this)->IsShowingMenus();
	}


	void MenuManager::Register(const char* name, CreatorFunc creator)
	{
		reinterpret_cast<::MenuManager*>(this)->Register(name, reinterpret_cast<::MenuManager::CreatorFunc>(creator));
	}


	bool MenuManager::GameIsPaused()
	{
		return numPauseGame != 0;
	}
}
