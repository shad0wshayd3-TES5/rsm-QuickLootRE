#pragma once

#include "RE/PlayerControls.h"  // PlayerControls::Data024
#include "RE/PlayerInputHandler.h"

class InputEvent;
class ThumbstickEvent;



namespace RE
{
	class ButtonEvent;


	class MovementHandler : public PlayerInputHandler
	{
		virtual	bool	CanProcess(InputEvent* a_event) override;
		virtual	void	ProcessThumbstick(ThumbstickEvent* a_event, PlayerControls::Data024* a_arg2) override;
		virtual	void	ProcessButton(ButtonEvent* a_event, PlayerControls::Data024* a_arg2) override;
	};
}
