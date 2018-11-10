#include "RE/ActorProcessManager.h"


namespace RE
{
	void ActorProcessManager::SetEquipFlag(Flag flag)
	{
		CALL_MEMBER_FN(reinterpret_cast<::ActorProcessManager*>(this), SetEquipFlag)(flag);
	}


	void ActorProcessManager::UpdateEquipment(Actor* actor)
	{
		CALL_MEMBER_FN(reinterpret_cast<::ActorProcessManager*>(this), UpdateEquipment)(reinterpret_cast<::Actor*>(actor));
	}


	void ActorProcessManager::SetDataFlag(float flag)
	{
		CALL_MEMBER_FN(reinterpret_cast<::ActorProcessManager*>(this), SetDataFlag)(flag);
	}


	void ActorProcessManager::UpdateEquipment_Hooked(Actor* actor)
	{
		reinterpret_cast<::ActorProcessManager*>(this)->UpdateEquipment_Hooked(reinterpret_cast<::Actor*>(actor));
	}
}
