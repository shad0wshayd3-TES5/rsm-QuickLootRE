#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "HookShare.h"

#include "RE/Skyrim.h"


namespace
{
	enum class ControlID
	{
		kNone,
		kActivate,
		kReadyWeapon,
		kTogglePOV,
		kJump,
		kSprint,
		kSneak,
		kShout,
		kToggleRun,
		kAutoMove,
		kFavorites
	};


	bool g_activateHandlerHooked = false;
	bool g_cameraStateHandlerHooked = false;


	template <class Op>
	class SettingMap : public std::unordered_map<std::string, std::function<void(HookShare::RegisterForCanProcess_t*, llvm::function_ref<void(const char*)>)>>
	{
	public:
		SettingMap()
		{
			using HookShare::Hook;

			std::string key;

			key = "activate";
			insert(std::make_pair(key, [](auto a_register, auto a_set)
			{
				auto strHolder = InputStringHolder::GetSingleton();
				a_register(Hook::kActivate, &ActivateHandlerEx<Op>::Hook_CanProcess);
				a_set(strHolder->activate.c_str());
			}));

			key = "readyWeapon";
			insert(std::make_pair(key, [](auto a_register, auto a_set)
			{
				auto strHolder = InputStringHolder::GetSingleton();
				a_register(Hook::kReadyWeapon, &ReadyWeaponHandlerEx<Op>::Hook_CanProcess);
				a_set(strHolder->readyWeapon.c_str());
			}));

			key = "togglePOV";
			insert(std::make_pair(key, [](auto a_register, auto a_set)
			{
				auto strHolder = InputStringHolder::GetSingleton();
				a_register(Hook::kFirstPersonState, &FirstPersonStateHandlerEx<Op>::Hook_CanProcess);
				a_register(Hook::kThirdPersonState, &ThirdPersonStateHandlerEx<Op>::Hook_CanProcess);
				a_set(strHolder->togglePOV.c_str());
				g_cameraStateHandlerHooked = true;
			}));

			key = "jump";
			insert(std::make_pair(key, [](auto a_register, auto a_set)
			{
				auto strHolder = InputStringHolder::GetSingleton();
				a_register(Hook::kJump, &JumpHandlerEx<Op>::Hook_CanProcess);
				a_set(strHolder->jump.c_str());
			}));

			key = "sprint";
			insert(std::make_pair(key, [](auto a_register, auto a_set)
			{
				auto strHolder = InputStringHolder::GetSingleton();
				a_register(Hook::kSprint, &SprintHandlerEx<Op>::Hook_CanProcess);
				a_set(strHolder->sprint.c_str());
			}));

			key = "sneak";
			insert(std::make_pair(key, [](auto a_register, auto a_set)
			{
				auto strHolder = InputStringHolder::GetSingleton();
				a_register(Hook::kSneak, &SneakHandlerEx<Op>::Hook_CanProcess);
				a_set(strHolder->sneak.c_str());
			}));

			key = "shout";
			insert(std::make_pair(key, [](auto a_register, auto a_set)
			{
				auto strHolder = InputStringHolder::GetSingleton();
				a_register(Hook::kShout, &ShoutHandlerEx<Op>::Hook_CanProcess);
				a_set(strHolder->shout.c_str());
			}));

			key = "toggleRun";
			insert(std::make_pair(key, [](auto a_register, auto a_set)
			{
				auto strHolder = InputStringHolder::GetSingleton();
				a_register(Hook::kToggleRun, &ToggleRunHandlerEx<Op>::Hook_CanProcess);
				a_set(strHolder->toggleRun.c_str());
			}));

			key = "autoMove";
			insert(std::make_pair(key, [](auto a_register, auto a_set)
			{
				auto strHolder = InputStringHolder::GetSingleton();
				a_register(Hook::kAutoMove, &AutoMoveHandlerEx<Op>::Hook_CanProcess);
				a_set(strHolder->autoMove.c_str());
			}));
		}
	};
}


namespace Hooks
{
	void InstallHooks(HookShare::RegisterForCanProcess_t* a_register);
}
