#include "RE/TESNPC.h"

#include "skse64/GameObjects.h"  // TESNPC

class TESRace;


namespace RE
{
	char TESNPC::GetSex()
	{
		return CALL_MEMBER_FN(reinterpret_cast<::TESNPC*>(this), GetSex)();
	}


	bool TESNPC::HasOverlays()
	{
		return CALL_MEMBER_FN(reinterpret_cast<::TESNPC*>(this), HasOverlays)();
	}


	void TESNPC::ChangeHeadPart(BGSHeadPart* a_target)
	{
		CALL_MEMBER_FN(reinterpret_cast<::TESNPC*>(this), ChangeHeadPart)(a_target);
	}


	void TESNPC::ApplyMorph(MorphAction* a_morphAction)
	{
		CALL_MEMBER_FN(reinterpret_cast<::TESNPC*>(this), ApplyMorph)(reinterpret_cast<::TESNPC::MorphAction*>(a_morphAction));
	}


	void TESNPC::UpdateNeck(BSFaceGenNiNode* a_faceNode)
	{
		CALL_MEMBER_FN(reinterpret_cast<::TESNPC*>(this), UpdateNeck)(a_faceNode);
	}


	void TESNPC::SetSkinFromTint(NiColorA* a_result, TintMask* a_tintMask, UInt32 a_compute, UInt32 a_unk1)
	{
		CALL_MEMBER_FN(reinterpret_cast<::TESNPC*>(this), SetSkinFromTint)(a_result, a_tintMask, a_compute, a_unk1);
	}


	void TESNPC::SetFaceTexture(BGSTextureSet* a_textureSet)
	{
		reinterpret_cast<::TESNPC*>(this)->SetFaceTexture(a_textureSet);
	}


	void TESNPC::SetHairColor(BGSColorForm* a_hairColor)
	{
		reinterpret_cast<::TESNPC*>(this)->SetHairColor(a_hairColor);
	}


	BGSHeadPart* TESNPC::GetHeadPartByType(UInt32 a_type)
	{
		return reinterpret_cast<::TESNPC*>(this)->GetCurrentHeadPartByType(a_type);
	}


	BGSHeadPart* TESNPC::GetHeadPartOverlayByType(UInt32 a_type)
	{
		return reinterpret_cast<::TESNPC*>(this)->GetHeadPartOverlayByType(a_type);
	}


	BGSHeadPart* TESNPC::GetCurrentHeadPartByType(UInt32 a_type)
	{
		return reinterpret_cast<::TESNPC*>(this)->GetCurrentHeadPartByType(a_type);
	}


	TESNPC* TESNPC::GetRootTemplate()
	{
		return reinterpret_cast<TESNPC*>(reinterpret_cast<::TESNPC*>(this)->GetRootTemplate());
	}


	TESRace* TESNPC::GetRace()
	{
		return race;
	}
}
