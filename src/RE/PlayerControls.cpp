#include "RE/PlayerControls.h"

#include "skse64/GameInput.h"  // PlayerControls


namespace RE
{
	PlayerControls* PlayerControls::GetSingleton()
	{
		return reinterpret_cast<PlayerControls*>(::PlayerControls::GetSingleton());
	}


	PlayerControls* PlayerControls::ctor_Hook()
	{
		return reinterpret_cast<PlayerControls*>(reinterpret_cast<::PlayerControls*>(this)->ctor_Hook());
	}


	PlayerControls* PlayerControls::ctor()
	{
		return reinterpret_cast<PlayerControls*>(CALL_MEMBER_FN(reinterpret_cast<::PlayerControls*>(this), ctor)());
	}
}
