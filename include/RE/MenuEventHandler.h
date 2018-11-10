#pragma once

#include "skse64/GameTypes.h"  // BSIntrusiveRefCounted

class InputEvent;
class KinectEvent;
class MouseMoveEvent;
class ThumbstickEvent;

namespace RE
{
	class ButtonEvent;
}


namespace RE
{
	class MenuEventHandler : public BSIntrusiveRefCounted
	{
	public:
		MenuEventHandler();
		virtual ~MenuEventHandler() {}

		virtual bool	CanProcess(InputEvent* a_event) = 0;
		virtual bool	ProcessKinect(KinectEvent* a_event);
		virtual bool	ProcessThumbstick(ThumbstickEvent* a_event);
		virtual bool	ProcessMouseMove(MouseMoveEvent* a_event);
		virtual bool	ProcessButton(RE::ButtonEvent* a_event);


		UInt8	unk08;		// 08
		UInt8	unk09;		// 09
		UInt8	pad0A[2];	// 0A
	};
}
