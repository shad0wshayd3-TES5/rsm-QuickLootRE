#include "RE/NiAVObject.h"


namespace RE
{
	void NiAVObject::UpdateNode(ControllerUpdateContext* ctx)
	{
		CALL_MEMBER_FN(reinterpret_cast<::NiAVObject*>(this), UpdateNode)(reinterpret_cast<::NiAVObject::ControllerUpdateContext*>(ctx));
	}
}
