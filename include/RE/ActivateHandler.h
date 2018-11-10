#pragma once

#include "RE/HeldStateHandler.h"  // HeldStateHandler
#include "RE/PlayerControls.h"  // PlayerControls::Data024

class InputEvent;


namespace RE
{
	class ButtonEvent;


	class ActivateHandler : public HeldStateHandler
	{
	public:
		virtual	bool	CanProcess(InputEvent* a_event) override;
		virtual	void	ProcessButton(ButtonEvent* a_event, PlayerControls::Data028* a_arg2) override;
	};
}
