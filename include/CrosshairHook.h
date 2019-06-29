#pragma once

#include "RE/Skyrim.h"


namespace Temporary
{
	class CrosshairHook
	{
	public:
		static bool LookupCrosshairRefByHandle(RE::RefHandle& a_handle, RE::TESObjectREFRPtr& a_refrOut);
		static RE::TESObjectREFR* GetCurrentCrosshairRef(RE::StaticFunctionTag*);
		static void InstallHooks();
		static bool RegisterFuncs(RE::BSScript::Internal::VirtualMachine* a_vm);

	private:
		CrosshairHook() = delete;
		CrosshairHook(const CrosshairHook&) = delete;
		CrosshairHook(CrosshairHook&&) = delete;
		~CrosshairHook() = delete;

		CrosshairHook& operator=(const CrosshairHook&) = delete;
		CrosshairHook& operator=(CrosshairHook&&) = delete;


		static RE::TESObjectREFRPtr _crosshairRef;
	};
}
