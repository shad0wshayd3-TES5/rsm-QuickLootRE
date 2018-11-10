#include "RE/TESActorBaseData.h"


namespace RE
{
	bool TESActorBaseData::IsFemale()
	{
		return (flags & kFlag_Female) != 0;
	}


	bool TESActorBaseData::IsEssential()
	{
		return (flags & kFlag_Essential) != 0;
	}


	bool TESActorBaseData::Respawns()
	{
		return (flags & kFlag_Respawn) != 0;
	}


	bool TESActorBaseData::UsesAutoCalc()
	{
		return (flags & kFlag_AutoCalc) != 0;
	}


	bool TESActorBaseData::IsUnique()
	{
		return (flags & kFlag_Unique) != 0;
	}


	bool TESActorBaseData::AffectsStealth()
	{
		return (flags & kFlag_DontAffectStealth) == 0;
	}


	bool TESActorBaseData::HasPCLevelMult()
	{
		return (flags & kFlag_PCLevelMult) != 0;
	}


	bool TESActorBaseData::Protects()
	{
		return (flags & kFlag_Protect) != 0;
	}


	bool TESActorBaseData::IsSummonable()
	{
		return (flags & kFlag_Summonable) != 0;
	}


	bool TESActorBaseData::Bleeds()
	{
		return (flags & kFlag_DoesntBleed) == 0;
	}


	bool TESActorBaseData::HasBleedoutOverride()
	{
		return (flags & kFlag_BleedoutOverride) != 0;
	}


	bool TESActorBaseData::UsesOppositeGenderAnims()
	{
		return (flags & kFlag_OppositeGenderanims) != 0;
	}


	bool TESActorBaseData::IsSimpleActor()
	{
		return (flags & kFlag_SimpleActor) != 0;
	}


	bool TESActorBaseData::IsGhost()
	{
		return (flags & kFlag_Ghost) != 0;
	}


	bool TESActorBaseData::IsInvulnerable()
	{
		return (flags & kFlag_Invulnerable) != 0;
	}
}
