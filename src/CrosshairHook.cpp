#include "CrosshairHook.h"

#include "skse64_common/BranchTrampoline.h"

#include "REL/Relocation.h"
#include "SKSE/API.h"


namespace Temporary
{
	bool CrosshairHook::LookupCrosshairRefByHandle(RE::RefHandle& a_handle, RE::TESObjectREFRPtr& a_refrOut)
	{
		auto result = RE::TESObjectREFR::LookupByHandle(a_handle, a_refrOut);

		_crosshairRef = a_refrOut;
		SKSE::CrosshairRefEvent event(a_refrOut);
		SKSE::GetCrosshairRefEventSource()->SendEvent(&event);

		return result;
	}


	RE::TESObjectREFR* CrosshairHook::GetCurrentCrosshairRef(RE::StaticFunctionTag*)
	{
		return _crosshairRef.get();
	}


	void CrosshairHook::InstallHooks()
	{
		REL::Offset<std::uintptr_t> target(0x006B0570 + 0x90);	// 1_5_80
		g_branchTrampoline.Write5Call(target.GetAddress(), unrestricted_cast<std::uintptr_t>(&LookupCrosshairRefByHandle));
		_DMESSAGE("[DEBUG] Installed fix for crosshair ref dispatch");
	}


	bool CrosshairHook::RegisterFuncs(RE::BSScript::Internal::VirtualMachine* a_vm)
	{
		a_vm->RegisterFunction("GetCurrentCrosshairRef", "Game", GetCurrentCrosshairRef);
		_DMESSAGE("[DEBUG] Hooked Game.GetCurrentCrosshairRef");
		return true;
	}


	decltype(CrosshairHook::_crosshairRef) CrosshairHook::_crosshairRef;
}
