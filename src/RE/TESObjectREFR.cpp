#include "RE/TESObjectREFR.h"

#include "skse64/GameExtraData.h"  // ExtraOwnership, ExtraTextDisplayData
#include "skse64/GameFormComponents.h"  // TESContainer, TESFullName
#include "skse64/GameObjects.h"  // TESObjectCONT
#include "skse64/GameReferences.h"  // TESObjectREFR
#include "skse64/GameRTTI.h"  // DYNAMIC_CAST
#include "skse64/GameTypes.h"  // BSFixedString

#include <exception>  // exception

#include "RE/ExtraLock.h"  // ExtraLock
#include "RE/TESActorBase.h"  // TESActorBase
#include "RE/TESFaction.h"  // TESFaction
#include "RE/TESNPC.h"  // TESNPC


namespace RE
{
	float TESObjectREFR::GetBaseScale()
	{
		return CALL_MEMBER_FN(reinterpret_cast<::TESObjectREFR*>(this), GetBaseScale)();
	}


	bool TESObjectREFR::IsOffLimits()
	{
		return CALL_MEMBER_FN(reinterpret_cast<::TESObjectREFR*>(this), IsOffLimits)();
	}


	float TESObjectREFR::GetWeight()
	{
		return CALL_MEMBER_FN(reinterpret_cast<::TESObjectREFR*>(this), GetWeight)();
	}


	const char* TESObjectREFR::GetReferenceName()
	{
		return CALL_MEMBER_FN(reinterpret_cast<::TESObjectREFR*>(this), GetReferenceName)();
	}


	TESWorldSpace* TESObjectREFR::GetWorldspace()
	{
		return CALL_MEMBER_FN(reinterpret_cast<::TESObjectREFR*>(this), GetWorldspace)();
	}


	UInt32 TESObjectREFR::CreateRefHandle()
	{
		return reinterpret_cast<::TESObjectREFR*>(this)->CreateRefHandle();
	}


	TESNPC* TESObjectREFR::GetActorOwner()
	{
		ExtraOwnership* exOwnership = static_cast<ExtraOwnership*>(extraData.GetByType(kExtraData_Ownership));
		if (exOwnership && exOwnership->owner && exOwnership->owner->formType == kFormType_Character) {
			return static_cast<TESNPC*>(exOwnership->owner);
		} else {
			return 0;
		}
	}


	TESForm* TESObjectREFR::GetBaseObject()
	{
		return baseForm;
	}


	TESContainer* TESObjectREFR::GetContainer()
	{
		TESContainer* container = 0;
		if (baseForm) {
			switch (baseForm->formType) {
			case kFormType_Container:
			{
				TESObjectCONT* cont = static_cast<TESObjectCONT*>(baseForm);
				container = cont ? &cont->container : 0;
				break;
			}
			case kFormType_NPC:
			{
				TESActorBase* actorBase = static_cast<TESActorBase*>(baseForm);
				container = actorBase;
				break;
			}
			}
		}
		return container;
	}


	const char* TESObjectREFR::GetFullName()
	{
		const char* result = 0;
		if (baseForm) {
			TESFullName* fullName = 0;
			try {
				fullName = DYNAMIC_CAST(baseForm, TESForm, TESFullName);
				if (fullName) {
					result = fullName->GetName();
				}
			} catch (std::exception& e) {
				_ERROR(e.what());
			}
		}
		return result;
	}


	TESFaction* TESObjectREFR::GetFactionOwner()
	{
		ExtraOwnership* exOwnership = static_cast<ExtraOwnership*>(extraData.GetByType(kExtraData_Ownership));
		if (exOwnership && exOwnership->owner && exOwnership->owner->formType == kFormType_Faction) {
			return static_cast<TESFaction*>(exOwnership->owner);
		} else {
			return 0;
		}
	}


	TESForm* TESObjectREFR::GetOwner()
	{
		return _GetOwner_Impl(this);
	}


	TESObjectCELL* TESObjectREFR::GetParentCell()
	{
		return parentCell;
	}


	float TESObjectREFR::GetPositionX()
	{
		return pos.x;
	}


	float TESObjectREFR::GetPositionY()
	{
		return pos.y;
	}


	float TESObjectREFR::GetPositionZ()
	{
		return pos.z;
	}


	bool TESObjectREFR::Is3DLoaded()
	{
		return GetNiNode() != 0;
	}


	bool TESObjectREFR::IsMarkedForDeletion()
	{
		return (flags & kTESFormFlag_MarkedForDeletion) != 0;
	}


	bool TESObjectREFR::IsDisabled()
	{
		return (flags & kTESFormFlag_Disabled) != 0;
	}


	bool TESObjectREFR::IsIgnoringFriendlyHits()
	{
		return (flags & kTESFormFlag_IgnoreFriendlyHits) != 0;
	}


	bool TESObjectREFR::SetDisplayName(const BSFixedString& name, bool force)
	{
		bool renamed = false;

		ExtraTextDisplayData* xTextData = reinterpret_cast<ExtraTextDisplayData*>(extraData.GetByType(kExtraData_TextDisplayData));
		if (xTextData) {
			bool inUse = (xTextData->message || xTextData->owner);
			if (inUse && force) {
				xTextData->message = nullptr;
				xTextData->owner = nullptr;
			}
			renamed = (!inUse || force);
			CALL_MEMBER_FN(xTextData, SetName_Internal)(name.c_str());
		} else {
			ExtraTextDisplayData* newTextData = ExtraTextDisplayData::Create();
			CALL_MEMBER_FN(newTextData, SetName_Internal)(name.c_str());
			extraData.Add(kExtraData_TextDisplayData, newTextData);
			renamed = true;
		}

		return renamed;
	}


	bool TESObjectREFR::LookupByHandle(UInt32& a_refHandle, TESObjectREFRPtr& a_refrOut)
	{
		typedef bool _Lookup(const UInt32& a_refHandle, TESObjectREFRPtr& a_refrOut);
		static _Lookup* Lookup = reinterpret_cast<_Lookup*>(::LookupREFRObjectByHandle.GetUIntPtr());

		return Lookup(a_refHandle, a_refrOut);
	}


	bool TESObjectREFR::LookupByHandle(UInt32& a_refHandle, TESObjectREFR*& a_refrOut)
	{
		typedef bool _Lookup(const UInt32& a_refHandle, TESObjectREFR*& a_refrOut);
		static _Lookup* Lookup = reinterpret_cast<_Lookup*>(::LookupREFRByHandle.GetUIntPtr());

		return Lookup(a_refHandle, a_refrOut);
	}


	bool TESObjectREFR::IsLocked()
	{
		LockState* state = _GetLockState_Impl(this);
		return (state && state->isLocked);
	}


	UInt32 TESObjectREFR::GetNumItems(bool a_unk1, bool a_unk2)
	{
		return _GetNumItems(this, a_unk1, a_unk2);
	}


	UInt32 TESObjectREFR::ActivateRefChildren(TESObjectREFR* a_activator)
	{
		return _ActivateRefChildren(this, a_activator);
	}


	void TESObjectREFR::PlayAnimation(RE::NiControllerManager* a_manager, RE::NiControllerSequence* a_toSeq, RE::NiControllerSequence* a_fromSeq, bool a_unk)
	{
		_PlayAnimation(this, a_manager, a_toSeq, a_fromSeq, a_unk);
	}


	RelocAddr<TESObjectREFR::_GetOwner_Impl_t*> TESObjectREFR::_GetOwner_Impl(TES_OBJECT_REFR_GET_OWNER_IMPL);
	RelocAddr<TESObjectREFR::_GetLockState_Impl_t*> TESObjectREFR::_GetLockState_Impl(TES_OBJECT_REFR_GET_LOCK_STATE_IMPL);
	RelocAddr<TESObjectREFR::_GetNumItems_t*> TESObjectREFR::_GetNumItems(TES_OBJECT_REFR_GET_NUM_ITEMS);
	RelocAddr<TESObjectREFR::_ActivateRefChildren_t*> TESObjectREFR::_ActivateRefChildren(TES_OBJECT_REFR_ACTIVATE_CHILDREN);
	RelocAddr<TESObjectREFR::_PlayAnimation_t*> TESObjectREFR::_PlayAnimation(TES_OBJECT_REFR_PLAY_ANIMATION);
}
