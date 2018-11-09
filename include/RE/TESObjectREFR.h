#pragma once

#include "skse64/GameForms.h"  // TESForm
#include "skse64/GameReferences.h"  // TESObjectREFR
#include "skse64/GameRTTI.h"  // DYNAMIC_CAST
#include "skse64/NiTypes.h"  // NiPoint3

#include "Offsets.h"

#include "RE/BaseExtraList.h"  // BaseExtraList
#include "RE/ExtraLock.h"  // LockState
#include "RE/IAnimationGraphManagerHolder.h"  // IAnimationGraphManagerHolder

class ActorWeightModel;
class BaseExtraList;
class BSFaceGenAnimationData;
class BSFaceGenNiNode;
class TESFaction;
class TESNPC;
class TESObjectCELL;


namespace RE
{
	class NiControllerManager;
	class NiControllerSequence;
	class NiNode;


	MAKE_NI_POINTER(TESObjectREFR);


	class TESObjectREFR :
		public TESForm,
		public BSHandleRefObject
	{
	private:
		static inline bool LookupREFRByHandle(const UInt32& a_refHandle, TESObjectREFRPtr& a_refrOut)
		{
			typedef bool _Lookup(const UInt32& a_refHandle, TESObjectREFRPtr& a_refrOut);
			static _Lookup* Lookup = reinterpret_cast<_Lookup*>(LookupREFRObjectByHandle.GetUIntPtr());

			return Lookup(a_refHandle, a_refrOut);
		}

	public:
		// parents
		BSTEventSink <BSAnimationGraphEvent>	animGraphEventSink;	// 30
		IAnimationGraphManagerHolder			animGraphHolder;	// 38


		enum { kTypeID = kFormType_Reference };


		enum RemoveType : UInt32
		{
			kRemoveType_Remove,
			kRemoveType_Steal,
			kRemoveType_Trade,
			kRemoveType_Drop,
			kRemoveType_Take,
			kRemoveType_Unk05
		};


		enum Flag : UInt32
		{
			kFlag_Harvested = 1 << 13,
		};


		enum TESFormFlag : UInt32
		{
			kTESFormFlag_Deleted = 1 << 5,
			kTESFormFlag_Disabled = 1 << 11,
			kTESFormFlag_IgnoreFriendlyHits = 1 << 20
		};


		struct LoadedState
		{
			UInt8	todo[0x68];	// 00
			NiNode*	node;		// 68
			// ... probably more
		};


		virtual void					Unk_39(void);
		virtual void					Unk_3A(void);
		virtual void					Unk_3B(void);
		virtual void					Unk_3C(void);
		virtual void					Unk_3D(void);
		virtual void					Unk_3E(void);
		virtual void					Unk_3F(void);
		virtual void					Unk_40(void);
		virtual void					Unk_41(void);
		virtual void					Unk_42(void);
		virtual void					Unk_43(void);
		virtual void					Unk_44(void);
		virtual void					Unk_45(void);
		virtual void					Unk_46(void);
		virtual void					Unk_47(void);
		virtual void					Unk_48(void);
		virtual void					Unk_49(void);
		virtual void					Unk_4A(void);
		virtual void					Unk_4B(void);
		virtual void					Unk_4C(void);
		virtual void					Unk_4D(void);
		virtual void					Unk_4E(void);
		virtual void					Unk_4F(void);
		virtual void					Unk_50(void);
		virtual void					GetStartingPos(::NiPoint3* a_pos);
		virtual void					Unk_52(void);
		virtual void					Unk_53(void);
		virtual void					Unk_54(void);
		virtual void					Unk_55(void);
		virtual void					RemoveItem(UInt32* a_droppedItemHandle, TESForm* a_akItem, UInt32 a_aiCount, RemoveType a_mode, ::BaseExtraList* a_extraList, TESObjectREFR* a_moveToRef, UInt32 a_unk7, UInt32 a_unk8);
		virtual void					Unk_57(void);
		virtual void					Unk_58(void);
		virtual void					Unk_59(void);
		virtual void					Unk_5A(void);
		virtual void					GetMarkerPosition(::NiPoint3* a_pos);
		virtual void					Unk_5C(void);
		virtual void					Unk_5D(void);
		virtual bool					IsChild() const;
		virtual void					Unk_5F(void);
		virtual void					Unk_60(void);
		virtual BSFaceGenNiNode*		GetFaceGenNiNode();
		virtual void					Unk_62(void);
		virtual BSFaceGenAnimationData*	GetFaceGenAnimationData();
		virtual void					Unk_64(void);
		virtual void					Unk_65(void);
		virtual void					Unk_66(void);
		virtual void					Unk_67(void);
		virtual void					Unk_68(void);
		virtual void					Unk_69(void);
		virtual void					Unk_6A(void);
		virtual void					Unk_6B(void);
		virtual void					Unk_6C(void);
		virtual void					SetNiNode(NiNode* a_root, UInt32 a_unk1);  // NULL, 1?
		virtual void					Unk_6E(void);
		virtual NiNode*					GetNiRootNode(UInt32 a_firstPerson);
		virtual NiNode*					GetNiNode(void);  // Root of the skeleton (Get3D)
		virtual void					Unk_71(void);
		virtual void					Unk_72(void);
		virtual void					Unk_73(void);
		virtual void					Unk_74(void);
		virtual void					Unk_75(void);
		virtual void					Unk_76(void);
		virtual void					Unk_77(void);
		virtual void					Unk_78(void);
		virtual void					Unk_79(void);
		virtual void					Unk_7A(void);
		virtual void					Unk_7B(void);
		virtual void					Unk_7C(void);
		virtual void					Unk_7D(void);
		virtual ActorWeightModel*		GetWeightModel(UInt32 a_weightModel);  // 0 Small 1 Large
		virtual void					Unk_7F(void);
		virtual void					Unk_80(void);
		virtual void					Unk_81(void);
		virtual void					Unk_82(void);
		virtual void					Unk_83(void);
		virtual void					Unk_84(void);
		virtual void					Unk_85(void);
		virtual void					Unk_86(void);
		virtual void					Unk_87(void);
		virtual void					Unk_88(void);
		virtual void					Unk_89(void);
		virtual void					ResetInventory(bool a_unk);
		virtual void					Unk_8B(void);
		virtual void					Unk_8C(void);
		virtual void					Unk_8D(void);
		virtual void					Unk_8E(void);
		virtual void					Unk_8F(void);
		virtual void					Unk_90(void);
		virtual void					Unk_91(void);
		virtual void					Unk_92(void);
		virtual void					Unk_93(void);
		virtual void					Unk_94(void);
		virtual void					Unk_95(void);
		virtual void					Unk_96(void);
		virtual void					Unk_97(void);
		virtual void					Unk_98(void);
		virtual bool					IsDead(bool a_isActor);
		virtual void					Unk_9A(void);
		virtual void					Unk_9B(void);


		inline float			GetBaseScale()																															{ return CALL_MEMBER_FN(reinterpret_cast<::TESObjectREFR*>(this), GetBaseScale)(); }
		inline bool				IsOffLimits()																															{ return CALL_MEMBER_FN(reinterpret_cast<::TESObjectREFR*>(this), IsOffLimits)(); }
		inline float			GetWeight()																																{ return CALL_MEMBER_FN(reinterpret_cast<::TESObjectREFR*>(this), GetWeight)(); }
		inline const char*		GetReferenceName()																														{ return CALL_MEMBER_FN(reinterpret_cast<::TESObjectREFR*>(this), GetReferenceName)(); }
		inline TESWorldSpace*	GetWorldspace()																															{ return CALL_MEMBER_FN(reinterpret_cast<::TESObjectREFR*>(this), GetWorldspace)(); }
		inline UInt32			CreateRefHandle()																														{ return reinterpret_cast<::TESObjectREFR*>(this)->CreateRefHandle(); }

		TESNPC*					GetActorOwner();
		inline TESForm*			GetBaseObject()																															{ return baseForm; }
		TESContainer*			GetContainer();
		const char*				GetFullName();
		TESFaction*				GetFactionOwner();
		inline TESForm*			GetOwner()																																{ return CALL_MEMBER_FN(this, GetOwner_Impl)(); }
		inline TESObjectCELL*	GetParentCell()																															{ return parentCell; }
		inline float			GetPositionX()																															{ return pos.x; }
		inline float			GetPositionY()																															{ return pos.y; }
		inline float			GetPositionZ()																															{ return pos.z; }
		inline bool				Is3DLoaded()																															{ return GetNiNode() != 0; }
		inline bool				IsDeleted()																																{ return (flags & kTESFormFlag_Deleted) != 0; }
		inline bool				IsDisabled()																															{ return (flags & kTESFormFlag_Disabled) != 0; }
		inline bool				IsIgnoringFriendlyHits()																												{ return (flags & kTESFormFlag_IgnoreFriendlyHits) != 0; }
		bool					SetDisplayName(const BSFixedString& name, bool force);
		inline static bool		LookupByHandle(UInt32& a_refHandle, TESObjectREFRPtr& a_refrOut)																		{ return LookupREFRByHandle(a_refHandle, a_refrOut); }
		inline bool				IsLocked()																																{ LockState* state = CALL_MEMBER_FN(this, GetLockState_Impl)(); return (state && state->isLocked); }
		inline UInt32			GetNumItems(bool a_unk1, bool a_unk2)																									{ return CALL_MEMBER_FN(this, GetNumItems)(a_unk1, a_unk2); }
		inline UInt32			ActivateRefChildren(TESObjectREFR* a_activator)																							{ return CALL_MEMBER_FN(this, ActivateRefChildren)(a_activator); }
		inline void				PlayAnimation(RE::NiControllerManager* a_manager, RE::NiControllerSequence* a_toSeq, RE::NiControllerSequence* a_fromSeq, bool a_unk)	{ CALL_MEMBER_FN(this, PlayAnimation)(a_manager, a_toSeq, a_fromSeq, a_unk); }


		// members
		TESForm*		baseForm;		// 40
		NiPoint3		rot;			// 48
		NiPoint3		pos;			// 54

		TESObjectCELL*	parentCell;		// 60
		LoadedState*	loadedState;	// 68
		BaseExtraList	extraData;		// 70
		UInt64			unk88;			// 88 - New in SE
		UInt16			unk90;			// 90 - init'd to 100
		UInt8			unk92;			// 92
		UInt8			unk93;			// 93
		UInt32			pad94;			// 94

	private:
		MEMBER_FN_PREFIX(TESObjectREFR);
		DEFINE_MEMBER_FN(GetOwner_Impl, TESForm*, TES_OBJECT_REFR_GET_OWNER_IMPL);
		DEFINE_MEMBER_FN(GetLockState_Impl, LockState*, TES_OBJECT_REFR_GET_LOCK_STATE_IMPL);
		DEFINE_MEMBER_FN(GetNumItems, UInt32, TES_OBJECT_REFR_GET_NUM_ITEMS, bool a_unk1, bool a_unk2);
		DEFINE_MEMBER_FN(ActivateRefChildren, UInt32, TES_OBJECT_REFR_ACTIVATE_CHILDREN, TESObjectREFR* a_activator);
		DEFINE_MEMBER_FN(PlayAnimation, void, TES_OBJECT_REFR_PLAY_ANIMATION, RE::NiControllerManager* a_manager, RE::NiControllerSequence* a_toSeq, RE::NiControllerSequence* a_fromSeq, bool a_unk);
	};
};
