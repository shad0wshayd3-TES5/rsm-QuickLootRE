#include "RE/NiNode.h"

#include "skse64/NiObjects.h"  // NiAVObject


namespace RE
{
	NiNode* NiNode::Create(UInt32 arrBufLen)
	{
		return reinterpret_cast<NiNode*>(::NiNode::Create(arrBufLen));
	}
}
