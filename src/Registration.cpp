#include "Registration.h"

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
	void RegisterForContainerOpenAnimEvent(VM* a_vm, UInt32 a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("a_form is a NONE form!", a_stackID, Severity::kError);
			return;
		}

		auto dispatcher = OnContainerOpenAnim::GetSingleton();
		if (!dispatcher->Register(a_form)) {
			a_vm->TraceStack("Failed to register for OnContainerOpenAnim event!", a_stackID, Severity::kError);
		}
	}


	void UnregisterForContainerOpenAnimEvent(VM* a_vm, UInt32 a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("a_form is a NONE form!", a_stackID, Severity::kError);
			return;
		}

		auto dispatcher = OnContainerOpenAnim::GetSingleton();
		if (!dispatcher->Unregister(a_form)) {
			a_vm->TraceStack("Failed to unregister for OnContainerOpenAnim event!", a_stackID, Severity::kError);
		}
	}


	void RegisterForContainerCloseAnimEvent(VM* a_vm, UInt32 a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("a_form is a NONE form!", a_stackID, Severity::kError);
			return;
		}

		auto dispatcher = OnContainerCloseAnim::GetSingleton();
		if (!dispatcher->Register(a_form)) {
			a_vm->TraceStack("Failed to register for OnContainerOpenAnim event!", a_stackID, Severity::kError);
		}
	}


	void UnregisterForContainerCloseAnimEvent(VM* a_vm, UInt32 a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("a_form is a NONE form!", a_stackID, Severity::kError);
			return;
		}

		auto dispatcher = OnContainerCloseAnim::GetSingleton();
		if (!dispatcher->Unregister(a_form)) {
			a_vm->TraceStack("Failed to unregister for OnContainerOpenAnim event!", a_stackID, Severity::kError);
		}
	}


	bool RegisterFuncs(VM* a_vm)
	{
		a_vm->RegisterFunction("RegisterForContainerOpenAnimEvent", "QuickLoot", RegisterForContainerOpenAnimEvent);
		a_vm->RegisterFunction("UnregisterForContainerOpenAnimEvent", "QuickLoot", UnregisterForContainerOpenAnimEvent);
		a_vm->RegisterFunction("RegisterForContainerCloseAnimEvent", "QuickLoot", RegisterForContainerCloseAnimEvent);
		a_vm->RegisterFunction("UnregisterForContainerCloseAnimEvent", "QuickLoot", UnregisterForContainerCloseAnimEvent);

		return true;
	}
}
