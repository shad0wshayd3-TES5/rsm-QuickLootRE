#include "RE/Actor.h"

#include "skse64/GameForms.h"  // TESRace
#include "skse64/GameRTTI.h"  // DYNAMIC_CAST

#include "RE/ExtraFactionChanges.h"  // ExtraFactionChanges
#include "RE/TESActorBaseData.h"  // TESActorBaseData
#include "RE/TESFaction.h"  // TESFaction
#include "RE/TESNPC.h"  // TESNPC


namespace RE
{
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


	bool Actor::VisitFactions(FactionVisitor& visitor)
	{
		TESNPC* npc = GetActorBase();
		if (npc) {
			for (UInt32 i = 0; i < npc->factions.count; i++) {
				TESActorBaseData::FactionInfo info;
				npc->factions.GetNthItem(i, info);
				if (visitor.Accept(info.faction, info.rank)) {
					return true;
				}
			}

			ExtraFactionChanges* pFactionChanges = static_cast<ExtraFactionChanges*>(extraData.GetByType(kExtraData_FactionChanges));
			if (pFactionChanges) {
				for (UInt32 i = 0; i < pFactionChanges->factions.count; i++) {
					ExtraFactionChanges::FactionInfo info;
					pFactionChanges->factions.GetNthItem(i, info);
					if (visitor.Accept(info.faction, info.rank)) {
						return true;
					}
				}
			}
		}

		return false;
	}


	bool Actor::IsInFaction(TESFaction* a_faction)
	{
		if (!a_faction) {
			return false;
		} else {
			FactionVisitorUtil visitor(a_faction);
			return VisitFactions(visitor);
		}
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
		return (flags2 & kFlags2_IsCommandedActor) != 0;
	}


	bool Actor::IsEssential() const
	{
		return (flags2 & kFlags2_IsEssential) != 0;
	}


	bool Actor::IsGuard() const
	{
		return (flags1 & kFlags1_IsGuard) != 0;
	}


	bool Actor::IsInKillMove() const
	{
		return (flags2 & kFlags2_IsInKillMove) != 0;
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
		return (flags1 & kFlags1_IsPlayerTeammate) != 0;
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
		return (flags2 & kFlags2_IsTrespassing) != 0;
	}


	void Actor::DispelWornItemEnchantments()
	{
		_DispelWornItemEnchantments(this);
	}


	void Actor::SendStealAlarm(TESObjectREFR* a_refItemOrContainer, TESForm* a_stolenItem, UInt32 a_numItems, UInt32 a_value, TESForm* a_owner, bool a_unk)
	{
		_SendStealAlarm(this, a_refItemOrContainer, a_stolenItem, a_numItems, a_value, a_owner, a_unk);
	}


	SInt32 Actor::CalcEntryValue(InventoryEntryData* a_entryData, UInt32 a_numItems, bool a_unk)
	{
		return _CalcEntryValue(this, a_entryData, a_numItems, a_unk);
	}


	SInt32 Actor::GetDetectionLevel(Actor* a_target, UInt32 a_flag)
	{
		return _GetDetectionLevel(this, a_target, a_flag);
	}


	bool Actor::IsGhost()
	{
		return _IsGhost(this);
	}


	bool Actor::IsSummoned()
	{
		TESNPC* npc = GetActorBase();
		return npc ? npc->IsSummonable() : false;
	}


	RelocAddr<Actor::_DispelWornItemEnchantments_t*> Actor::_DispelWornItemEnchantments(ACTOR_DISPEL_WORN_ITEM_ENCHANTMENTS);
	RelocAddr<Actor::_SendStealAlarm_t*> Actor::_SendStealAlarm(ACTOR_SEND_STEAL_ALARM);
	RelocAddr<Actor::_CalcEntryValue_t*> Actor::_CalcEntryValue(ACTOR_CALC_ENTRY_VALUE);
	RelocAddr<Actor::_GetDetectionLevel_t*> Actor::_GetDetectionLevel(ACTOR_GET_DETECTION_LEVEL);
	RelocAddr<Actor::_IsGhost_t*> Actor::_IsGhost(ACTOR_IS_GHOST);


	Actor::FactionVisitorUtil::FactionVisitorUtil(TESFaction* a_faction) :
		_faction(a_faction)
	{}


	Actor::FactionVisitorUtil::~FactionVisitorUtil()
	{}


	bool Actor::FactionVisitorUtil::Accept(TESFaction* a_faction, SInt8 a_rank)
	{
		return (a_faction && a_faction->formID == _faction->formID);
	}
}
