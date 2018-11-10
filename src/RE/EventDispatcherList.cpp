#include "RE/EventDispatcherList.h"

#include "skse64/GameEvents.h"  // GetEventDispatcherList()


namespace RE
{
	EventDispatcherList* EventDispatcherList::GetEventDispatcherList()
	{
		return reinterpret_cast<EventDispatcherList*>(::GetEventDispatcherList());
	}
}
