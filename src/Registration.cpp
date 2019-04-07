#include "Registration.h"

#include "SKSE/Interface.h"
#include "RE/Skyrim.h"


OnContainerOpenAnim* OnContainerOpenAnim::GetSingleton()
{
	static OnContainerOpenAnim singleton;
	return &singleton;
}


OnContainerOpenAnim::OnContainerOpenAnim() :
	Base("OnContainerOpenAnim")
{}


OnContainerCloseAnim* OnContainerCloseAnim::GetSingleton()
{
	static OnContainerCloseAnim singleton;
	return &singleton;
}


OnContainerCloseAnim::OnContainerCloseAnim() :
	Base("OnContainerCloseAnim")
{}


namespace QuickLoot
{
	void RegisterForContainerOpenAnimEvent(RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			_ERROR("[ERROR] a_form is a NONE form!\n");
			return;
		}

		if (!OnContainerOpenAnim::GetSingleton()->Register(a_form)) {
			_ERROR("[ERROR] Failed to register for OnContainerOpenAnim event!\n");
		}
	}


	void UnregisterForContainerOpenAnimEvent(RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			_ERROR("[ERROR] a_form is a NONE form!\n");
			return;
		}

		if (!OnContainerOpenAnim::GetSingleton()->Unregister(a_form)) {
			_ERROR("[ERROR] Failed to unregister for OnContainerOpenAnim event!\n");
		}
	}


	void RegisterForContainerCloseAnimEvent(RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			_ERROR("[ERROR] a_form is a NONE form!\n");
			return;
		}

		if (!OnContainerCloseAnim::GetSingleton()->Register(a_form)) {
			_ERROR("[ERROR] Failed to register for OnContainerOpenAnim event!\n");
		}
	}


	void UnregisterForContainerCloseAnimEvent(RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			_ERROR("[ERROR] a_form is a NONE form!\n");
			return;
		}

		if (!OnContainerCloseAnim::GetSingleton()->Unregister(a_form)) {
			_ERROR("[ERROR] Failed to unregister for OnContainerOpenAnim event!\n");
		}
	}


	bool RegisterFuncs(RE::BSScript::Internal::VirtualMachine* a_vm)
	{
		a_vm->RegisterFunction("RegisterForContainerOpenAnimEvent", "QuickLoot", RegisterForContainerOpenAnimEvent);
		a_vm->RegisterFunction("UnregisterForContainerOpenAnimEvent", "QuickLoot", UnregisterForContainerOpenAnimEvent);
		a_vm->RegisterFunction("RegisterForContainerCloseAnimEvent", "QuickLoot", RegisterForContainerCloseAnimEvent);
		a_vm->RegisterFunction("UnregisterForContainerCloseAnimEvent", "QuickLoot", UnregisterForContainerCloseAnimEvent);

		return true;
	}
}
