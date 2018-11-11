#pragma once

#include "skse64/GameEvents.h"  // EventResult, EventDispatcher, MenuOpenCloseEvent, TESContainerChangedEvent
#include "skse64/GameFormComponents.h"  // TESContainer::Entry
#include "skse64/GameInput.h"  // InputEvent
#include "skse64/PapyrusEvents.h"  // SKSECrosshairRefEvent

#include <utility>  // pair
#include <map>  // map
#include <vector>  // vector

class InputEventDispatcher;
class InventoryEntryData;
class TESContainer;
class TESForm;


namespace RE
{
	class BaseExtraList;
}


namespace QuickLootRE
{
	class CrosshairRefEventHandler : public BSTEventSink<SKSECrosshairRefEvent>
	{
	public:
		virtual	EventResult ReceiveEvent(SKSECrosshairRefEvent* a_event, EventDispatcher<SKSECrosshairRefEvent>* a_dispatcher) override;
	};


	class InputEventHandler : public BSTEventSink<InputEvent>
	{
	public:
		virtual EventResult ReceiveEvent(InputEvent** a_event, InputEventDispatcher* a_dispatcher) override;
	};


	class MenuOpenCloseEventHandler : public BSTEventSink<MenuOpenCloseEvent>
	{
	public:
		virtual EventResult ReceiveEvent(MenuOpenCloseEvent* a_event, EventDispatcher<MenuOpenCloseEvent>* a_dispatcher) override;
	};


	class TESContainerChangedEventHandler : public BSTEventSink<TESContainerChangedEvent>
	{
	public:
		virtual EventResult ReceiveEvent(TESContainerChangedEvent* a_event, EventDispatcher<TESContainerChangedEvent>* a_dispatcher) override;
	};


	extern CrosshairRefEventHandler g_crosshairRefEventHandler;
	extern InputEventHandler g_inputEventHandler;
	extern MenuOpenCloseEventHandler g_menuOpenCloseEventHandler;
	extern TESContainerChangedEventHandler g_containerChangedEventHandler;
}
