#pragma once

#include "RE/PlayerControls.h"  // PlayerControls::Data024
#include "RE/PlayerInputHandler.h"  // PlayerInputHandler


namespace RE
{
	class ReadyWeaponHandler : public PlayerInputHandler
	{
	public:
		virtual	bool	CanProcess(InputEvent* a_event) override;
		virtual	void	ProcessButton(ButtonEvent* a_event, PlayerControls::Data024* a_arg2) override;
	};
}
