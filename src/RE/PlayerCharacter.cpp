#include "RE/PlayerCharacter.h"

#include "skse64/GameReferences.h"  // PlayerCharacter

#include "RE/TESObjectREFR.h"  // TESObjectREFR


namespace RE
{
	TintMask* PlayerCharacter::GetOverlayTintMask(TintMask* a_original)
	{
		return reinterpret_cast<::PlayerCharacter*>(this)->GetOverlayTintMask(a_original);
	}


	tArray<TintMask*>* PlayerCharacter::GetTintList()
	{
		return reinterpret_cast<::PlayerCharacter*>(this)->GetTintList();
	}


	UInt32 PlayerCharacter::GetNumTints(UInt32 a_tintType)
	{
		return CALL_MEMBER_FN(reinterpret_cast<::PlayerCharacter*>(this), GetNumTints)(a_tintType);
	}


	TintMask* PlayerCharacter::GetTintMask(UInt32 a_tintType, UInt32 a_index)
	{
		return CALL_MEMBER_FN(reinterpret_cast<::PlayerCharacter*>(this), GetTintMask)(a_tintType, a_index);
	}


	float PlayerCharacter::GetDamage(InventoryEntryData* a_pForm)
	{
		return CALL_MEMBER_FN(reinterpret_cast<::PlayerCharacter*>(this), GetDamage)(reinterpret_cast<::InventoryEntryData*>(a_pForm));
	}


	float PlayerCharacter::GetArmorValue(InventoryEntryData* a_pForm)
	{
		return CALL_MEMBER_FN(reinterpret_cast<::PlayerCharacter*>(this), GetArmorValue)(reinterpret_cast<::InventoryEntryData*>(a_pForm));
	}


	Actor* PlayerCharacter::GetActorInFavorState()
	{
		return _GetActorInFavorState(this);
	}


	TESObjectREFR* PlayerCharacter::GetGrabbedRef()
	{
		TESObjectREFRPtr refPtr;
		UInt32 handle = playerGrabbedHandle;
		if (handle != *g_invalidRefHandle) {
			TESObjectREFR::LookupByHandle(handle, refPtr);
		}
		return refPtr;
	}


	void PlayerCharacter::PlayPickupEvent(TESForm* a_item, TESForm* a_containerOwner, TESObjectREFR* a_containerRef, EventType a_eventType)
	{
		_PlayPickupEvent(this, a_item, a_containerOwner, a_containerRef, a_eventType);
	}


	void PlayerCharacter::StartActivation()
	{
		_StartActivation(this);
	}


	bool PlayerCharacter::TryToPickPocket(Actor* a_target, InventoryEntryData* a_pEntry, UInt32 a_numItems, bool a_unk4)
	{
		return _TryToPickPocket(this, a_target, a_pEntry, a_numItems, a_unk4);
	}


	RelocAddr<PlayerCharacter::_GetActorInFavorState_t*> PlayerCharacter::_GetActorInFavorState(PLAYER_CHARACTER_GET_ACTOR_IN_FAVOR_STATE);
	RelocAddr<PlayerCharacter::_PlayPickupEvent_t*> PlayerCharacter::_PlayPickupEvent(PLAYER_CHARACTER_PLAY_PICKUP_EVENT);
	RelocAddr<PlayerCharacter::_StartActivation_t*> PlayerCharacter::_StartActivation(PLAYER_CHARACTER_START_ACTIVATION);
	RelocAddr<PlayerCharacter::_TryToPickPocket_t*> PlayerCharacter::_TryToPickPocket(PLAYER_CHARACTER_TRY_TO_PICK_POCKET);
}
