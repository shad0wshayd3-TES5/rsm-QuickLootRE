#include "RE/ExtraContainerChanges.h"

#include "skse64/GameExtraData.h"  // ExtraContainerChanges, ExtraContainerChanges::Data


namespace RE
{
	::ExtraContainerChanges::FoundEquipData ExtraContainerChanges::FindEquipped(FormMatcher& a_matcher, bool a_isWorn, bool a_isWornLeft)
	{
		return reinterpret_cast<::ExtraContainerChanges*>(this)->FindEquipped(a_matcher, a_isWorn, a_isWornLeft);
	}


	::ExtraContainerChanges::FoundHotkeyData ExtraContainerChanges::FindHotkey(SInt32 a_hotkey)
	{
		return reinterpret_cast<::ExtraContainerChanges*>(this)->FindHotkey(a_hotkey);
	}


	::ExtraContainerChanges::FoundHotkeyData ExtraContainerChanges::FindHotkey(TESForm* a_form)
	{
		return reinterpret_cast<::ExtraContainerChanges*>(this)->FindHotkey(a_form);
	}
}
