#include "RE/PlayerCharacter.h"

#include "skse64/GameReferences.h"  // PlayerCharacter

#include "RE/TESObjectREFR.h"  // TESObjectREFR


namespace RE
{
	TintMask* PlayerCharacter::GetOverlayTintMask(TintMask* original)
	{
		return reinterpret_cast<::PlayerCharacter*>(this)->GetOverlayTintMask(original);
	}


	tArray<TintMask*>* PlayerCharacter::GetTintList()
	{
		return reinterpret_cast<::PlayerCharacter*>(this)->GetTintList();
	}


	UInt32 PlayerCharacter::GetNumTints(UInt32 tintType)
	{
		return CALL_MEMBER_FN(reinterpret_cast<::PlayerCharacter*>(this), GetNumTints)(tintType);
	}


	TintMask* PlayerCharacter::GetTintMask(UInt32 tintType, UInt32 index)
	{
		return CALL_MEMBER_FN(reinterpret_cast<::PlayerCharacter*>(this), GetTintMask)(tintType, index);
	}


	float PlayerCharacter::GetDamage(InventoryEntryData* pForm)
	{
		return CALL_MEMBER_FN(reinterpret_cast<::PlayerCharacter*>(this), GetDamage)(reinterpret_cast<::InventoryEntryData*>(pForm));
	}


	float PlayerCharacter::GetArmorValue(InventoryEntryData* pForm)
	{
		return CALL_MEMBER_FN(reinterpret_cast<::PlayerCharacter*>(this), GetArmorValue)(reinterpret_cast<::InventoryEntryData*>(pForm));
	}


	Actor* PlayerCharacter::GetActorInFavorState()
	{
		return CALL_MEMBER_FN(this, GetActorInFavorState)();
	}


	TESObjectREFR* PlayerCharacter::GetGrabbedRef()
	{
		::TESObjectREFR* refPtr = 0;
		UInt32 handle = playerGrabbedHandle;
		if (handle != *g_invalidRefHandle) {
			(*LookupREFRByHandle)(&handle, &refPtr);
		}
		return reinterpret_cast<TESObjectREFR*>(refPtr);
	}


	void PlayerCharacter::PlayPickupEvent(TESForm* item, TESForm* containerOwner, TESObjectREFR* containerRef, EventType eventType)
	{
		CALL_MEMBER_FN(this, PlayPickupEvent)(item, containerOwner, containerRef, eventType);
	}


	void PlayerCharacter::StartActivation()
	{
		CALL_MEMBER_FN(this, StartActivation)();
	}


	bool PlayerCharacter::TryToPickPocket(Actor* target, InventoryEntryData* pEntry, UInt32 numItems, bool unk4)
	{
		return CALL_MEMBER_FN(this, TryToPickPocket)(target, pEntry, numItems, unk4);
	}
}
