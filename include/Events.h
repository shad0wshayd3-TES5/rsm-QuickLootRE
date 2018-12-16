#pragma once

#include "skse64/GameEvents.h"  // EventResult, EventDispatcher, TESContainerChangedEvent
#include "skse64/PapyrusEvents.h"  // SKSECrosshairRefEvent

#include "RE/BSTEvent.h"  // BSTEventSink, EventResult, BSTEventSource
#include "RE/MenuOpenCloseEvent.h"  // MenuOpenCloseEvent


namespace RE
{
	class InputEvent;
}


namespace QuickLootRE
{
	class CrosshairRefEventHandler : public BSTEventSink<SKSECrosshairRefEvent>
	{
	public:
		virtual	EventResult ReceiveEvent(SKSECrosshairRefEvent* a_event, EventDispatcher<SKSECrosshairRefEvent>* a_dispatcher) override;
	};


	class InputEventHandler : public RE::BSTEventSink<RE::InputEvent*>
	{
	public:
		virtual RE::EventResult ReceiveEvent(RE::InputEvent** a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override;
	};


	class MenuOpenCloseEventHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{
	public:
		virtual RE::EventResult ReceiveEvent(RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;
	};


	class TESCombatEventHandler : public RE::BSTEventSink<TESCombatEvent>
	{
	public:
		virtual RE::EventResult ReceiveEvent(TESCombatEvent* a_event, RE::BSTEventSource<TESCombatEvent>* a_eventSource) override;
	};


	class TESContainerChangedEventHandler : public RE::BSTEventSink<TESContainerChangedEvent>
	{
	public:
		virtual RE::EventResult ReceiveEvent(TESContainerChangedEvent* a_event, RE::BSTEventSource<TESContainerChangedEvent>* a_eventSource) override;
	};


	extern CrosshairRefEventHandler g_crosshairRefEventHandler;
	extern InputEventHandler g_inputEventHandler;
	extern MenuOpenCloseEventHandler g_menuOpenCloseEventHandler;
	extern TESCombatEventHandler g_combatEventHandler;
	extern TESContainerChangedEventHandler g_containerChangedEventHandler;
}
