#include "RE/Actor.h"

#include "skse64/GameForms.h"  // TESRace

#include "RE/TESNPC.h"  // TESNPC


namespace RE
{
	bool Actor::VisitFactions(::Actor::FactionVisitor& a_visitor)
	{
		return reinterpret_cast<::Actor*>(this)->VisitFactions(a_visitor);
	}


	TESForm* Actor::GetEquippedObject(bool a_abLeftHand)
	{
		return reinterpret_cast<::Actor*>(this)->GetEquippedObject(a_abLeftHand);
	}


	void Actor::UpdateSkinColor()
	{
		reinterpret_cast<::Actor*>(this)->UpdateSkinColor();
	}


	void Actor::UpdateHairColor()
	{
		reinterpret_cast<::Actor*>(this)->UpdateHairColor();
	}


	void Actor::QueueNiNodeUpdate(bool a_updateWeight)
	{
		CALL_MEMBER_FN(reinterpret_cast<::Actor*>(this), QueueNiNodeUpdate)(a_updateWeight);
	}


	bool Actor::HasPerk(BGSPerk* a_perk)
	{
		return CALL_MEMBER_FN(reinterpret_cast<::Actor*>(this), HasPerk)(a_perk);
	}


	UInt16 Actor::GetLevel()
	{
		return CALL_MEMBER_FN(reinterpret_cast<::Actor*>(this), GetLevel)();
	}


	void Actor::SetRace(TESRace* a_race, bool a_isPlayer)
	{
		CALL_MEMBER_FN(reinterpret_cast<::Actor*>(this), SetRace)(a_race, a_isPlayer);
	}


	void Actor::UpdateWeaponAbility(TESForm* a_weapon, BaseExtraList* a_extraData, bool a_bLeftHand)
	{
		CALL_MEMBER_FN(reinterpret_cast<::Actor*>(this), UpdateWeaponAbility)(a_weapon, reinterpret_cast<::BaseExtraList*>(a_extraData), a_bLeftHand);
	}


	void Actor::UpdateArmorAbility(TESForm* a_armor, BaseExtraList* a_extraData)
	{
		CALL_MEMBER_FN(reinterpret_cast<::Actor*>(this), UpdateArmorAbility)(a_armor, reinterpret_cast<::BaseExtraList*>(a_extraData));
	}


	bool Actor::IsHostileToActor(::Actor* a_actor)
	{
		return CALL_MEMBER_FN(reinterpret_cast<::Actor*>(this), IsHostileToActor)(a_actor);
	}


	void Actor::ResetAI(UInt32 a_unk1, UInt32 a_unk2)
	{
		CALL_MEMBER_FN(reinterpret_cast<::Actor*>(this), ResetAI)(a_unk1, a_unk2);
	}


	TESNPC* Actor::GetActorBase()
	{
		return static_cast<TESNPC*>(baseForm);
	}


	TESRace* Actor::GetRace()
	{
		TESRace* outRace = 0;

		TESNPC* actorBase = GetActorBase();
		if (actorBase) {
			outRace = actorBase->race;
		}

		return outRace;
	}


	bool Actor::IsBeingRidden()
	{
		return ((flags2 & kFlags2_HasInteraction) != 0) && extraData.HasType(kExtraData_Interaction);
	}


	bool Actor::IsCommandedActor() const
	{
		return (flags2 & kFlags2_CommandedActor) != 0;
	}


	bool Actor::IsEssential() const
	{
		return (flags2 & kFlags2_Essential) != 0;
	}


	bool Actor::IsGuard() const
	{
		return (flags1 & kFlags1_Guard) != 0;
	}


	bool Actor::IsInKillMove() const
	{
		return (flags2 & kFlags2_KillMove) != 0;
	}


	bool Actor::IsAIEnabled() const
	{
		return (flags1 & kFlags1_AIEnabled) != 0;
	}


	bool Actor::IsOnMount()
	{
		return ((flags2 & kFlags2_HasInteraction) != 0) && extraData.HasType(kExtraData_Interaction);
	}


	bool Actor::IsPlayerTeammate() const
	{
		return (flags1 & kFlags1_PlayerTeammate) != 0;
	}


	bool Actor::IsSneaking()
	{
		if (!ActorState::IsSneaking()) {
			return false;
		}

		if (ActorState::IsSwimming()) {
			return false;
		}

		if (IsOnMount()) {
			return false;
		}

		return true;
	}


	bool Actor::IsTrespassing() const
	{
		return (flags2 & kFlags2_Trespassing) != 0;
	}


	void Actor::DispelWornItemEnchantments()
	{
		CALL_MEMBER_FN(this, DispelWornItemEnchantments)();
	}


	void Actor::SendStealAlarm(TESObjectREFR* a_refItemOrContainer, TESForm* a_stolenItem, UInt32 a_numItems, UInt32 a_value, TESForm* a_owner, bool a_unk)
	{
		CALL_MEMBER_FN(this, SendStealAlarm)(a_refItemOrContainer, a_stolenItem, a_numItems, a_value, a_owner, a_unk);
	}


	SInt32 Actor::CalcEntryValue(InventoryEntryData* a_entryData, UInt32 a_numItems, bool a_unk)
	{
		return CALL_MEMBER_FN(this, CalcEntryValue)(a_entryData, a_numItems, a_unk);
	}


	SInt32 Actor::GetDetectionLevel(Actor* a_target, UInt32 a_flag)
	{
		return CALL_MEMBER_FN(this, GetDetectionLevel)(a_target, a_flag);
	}


	bool Actor::IsGhost()
	{
		return CALL_MEMBER_FN(this, IsGhost)();
	}


	bool Actor::IsSummoned()
	{
		TESNPC* npc = GetActorBase();
		return npc ? npc->IsSummonable() : false;
	}
}
