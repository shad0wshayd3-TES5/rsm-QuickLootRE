#pragma once

#include "skse64/GameEvents.h"  // EventResult, EventDispatcher
#include "skse64/gamethreads.h"  // TaskDelegate
#include "skse64/PapyrusEvents.h"  // SKSECrosshairRefEvent

#include "RE/BSTEvent.h"  // BSTEventSink, EventResult, BSTEventSource
#include "RE/InputEvent.h"  // InputEvent
#include "RE/MenuOpenCloseEvent.h"  // MenuOpenCloseEvent
#include "RE/TESCombatEvent.h"  // TESCombatEvent
#include "RE/TESContainerChangedEvent.h"  // TESContainerChangedEvent


namespace Events
{
	class CrosshairRefEventHandler : public BSTEventSink<SKSECrosshairRefEvent>
	{
	public:
		virtual	EventResult ReceiveEvent(SKSECrosshairRefEvent* a_event, EventDispatcher<SKSECrosshairRefEvent>* a_dispatcher) override;

		static CrosshairRefEventHandler* GetSingleton();
		static void Free();

	protected:
		CrosshairRefEventHandler();
		CrosshairRefEventHandler(const CrosshairRefEventHandler&) = delete;
		CrosshairRefEventHandler(CrosshairRefEventHandler&&) = delete;
		virtual ~CrosshairRefEventHandler();

		CrosshairRefEventHandler& operator=(const CrosshairRefEventHandler&) = delete;
		CrosshairRefEventHandler& operator=(CrosshairRefEventHandler&&) = delete;


		static CrosshairRefEventHandler* _singleton;
	};


	class InputEventHandler : public RE::BSTEventSink<RE::InputEvent*>
	{
	public:
		virtual RE::EventResult ReceiveEvent(RE::InputEvent** a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override;

		static InputEventHandler* GetSingleton();
		static void Free();

	protected:
		InputEventHandler();
		InputEventHandler(const InputEventHandler&) = delete;
		InputEventHandler(InputEventHandler&&) = delete;
		virtual ~InputEventHandler();

		InputEventHandler& operator=(const InputEventHandler&) = delete;
		InputEventHandler& operator=(InputEventHandler&&) = delete;


		static InputEventHandler* _singleton;
	};


	class MenuOpenCloseEventHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{
	public:
		virtual RE::EventResult ReceiveEvent(RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

		static MenuOpenCloseEventHandler* GetSingleton();
		static void Free();

	protected:
		MenuOpenCloseEventHandler();
		MenuOpenCloseEventHandler(const MenuOpenCloseEventHandler&) = delete;
		MenuOpenCloseEventHandler(MenuOpenCloseEventHandler&&) = delete;
		virtual ~MenuOpenCloseEventHandler();

		MenuOpenCloseEventHandler& operator=(const MenuOpenCloseEventHandler&) = delete;
		MenuOpenCloseEventHandler& operator=(MenuOpenCloseEventHandler&&) = delete;


		static MenuOpenCloseEventHandler* _singleton;
	};


	class TESCombatEventHandler : public RE::BSTEventSink<RE::TESCombatEvent>
	{
	public:
		virtual RE::EventResult ReceiveEvent(RE::TESCombatEvent* a_event, RE::BSTEventSource<RE::TESCombatEvent>* a_eventSource) override;

		static TESCombatEventHandler* GetSingleton();
		static void Free();

	protected:
		TESCombatEventHandler();
		TESCombatEventHandler(const TESCombatEventHandler&) = delete;
		TESCombatEventHandler(TESCombatEventHandler&&) = delete;
		virtual ~TESCombatEventHandler();

		TESCombatEventHandler& operator=(const TESCombatEventHandler&) = delete;
		TESCombatEventHandler& operator=(TESCombatEventHandler&&) = delete;


		static TESCombatEventHandler* _singleton;
	};


	class TESContainerChangedEventHandler : public RE::BSTEventSink<RE::TESContainerChangedEvent>
	{
	public:
		virtual RE::EventResult ReceiveEvent(RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource) override;

		static TESContainerChangedEventHandler* GetSingleton();
		static void Free();

	protected:
		TESContainerChangedEventHandler();
		TESContainerChangedEventHandler(const TESContainerChangedEventHandler&) = delete;
		TESContainerChangedEventHandler(TESContainerChangedEventHandler&&) = delete;
		virtual ~TESContainerChangedEventHandler();

		TESContainerChangedEventHandler& operator=(const TESContainerChangedEventHandler&) = delete;
		TESContainerChangedEventHandler& operator=(TESContainerChangedEventHandler&&) = delete;


		static TESContainerChangedEventHandler* _singleton;
	};
}
