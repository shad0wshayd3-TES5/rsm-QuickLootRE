#include "AnimationManager.h"

#include "Loot.h"

bool AnimationManager::AnimHandler::ExecuteHandler(RE::Actor& a_handler, const RE::BSFixedString& a_parameter)
{
	if (a_handler.IsPlayerRef()) {
		auto loot = Loot::GetSingleton();
		loot->Close();
	}
	return _original ? (*_original)(a_handler, a_parameter) : true;
}
