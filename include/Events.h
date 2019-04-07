#pragma once

#include "SKSE/Events.h"  // CrosshairRefEvent
#include "RE/Skyrim.h"


namespace Events
{
	class CrosshairRefEventHandler : public RE::BSTEventSink<SKSE::CrosshairRefEvent>
	{
	public:
		virtual	RE::EventResult ReceiveEvent(SKSE::CrosshairRefEvent* a_event, RE::BSTEventSource<SKSE::CrosshairRefEvent>* a_dispatcher) override;

		static CrosshairRefEventHandler* GetSingleton();

	protected:
		CrosshairRefEventHandler() = default;
		CrosshairRefEventHandler(const CrosshairRefEventHandler&) = delete;
		CrosshairRefEventHandler(CrosshairRefEventHandler&&) = delete;
		virtual ~CrosshairRefEventHandler() = default;

		CrosshairRefEventHandler& operator=(const CrosshairRefEventHandler&) = delete;
		CrosshairRefEventHandler& operator=(CrosshairRefEventHandler&&) = delete;
	};


	class InputEventHandler : public RE::BSTEventSink<RE::InputEvent*>
	{
	public:
		virtual RE::EventResult ReceiveEvent(RE::InputEvent** a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override;

		static InputEventHandler* GetSingleton();

	protected:
		InputEventHandler() = default;
		InputEventHandler(const InputEventHandler&) = delete;
		InputEventHandler(InputEventHandler&&) = delete;
		virtual ~InputEventHandler() = default;

		InputEventHandler& operator=(const InputEventHandler&) = delete;
		InputEventHandler& operator=(InputEventHandler&&) = delete;
	};


	class MenuOpenCloseEventHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{
	public:
		virtual RE::EventResult ReceiveEvent(RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

		static MenuOpenCloseEventHandler* GetSingleton();

	protected:
		MenuOpenCloseEventHandler() = default;
		MenuOpenCloseEventHandler(const MenuOpenCloseEventHandler&) = delete;
		MenuOpenCloseEventHandler(MenuOpenCloseEventHandler&&) = delete;
		virtual ~MenuOpenCloseEventHandler() = default;

		MenuOpenCloseEventHandler& operator=(const MenuOpenCloseEventHandler&) = delete;
		MenuOpenCloseEventHandler& operator=(MenuOpenCloseEventHandler&&) = delete;
	};


	class TESCombatEventHandler : public RE::BSTEventSink<RE::TESCombatEvent>
	{
	public:
		virtual RE::EventResult ReceiveEvent(RE::TESCombatEvent* a_event, RE::BSTEventSource<RE::TESCombatEvent>* a_eventSource) override;

		static TESCombatEventHandler* GetSingleton();

	protected:
		TESCombatEventHandler() = default;
		TESCombatEventHandler(const TESCombatEventHandler&) = delete;
		TESCombatEventHandler(TESCombatEventHandler&&) = delete;
		virtual ~TESCombatEventHandler() = default;

		TESCombatEventHandler& operator=(const TESCombatEventHandler&) = delete;
		TESCombatEventHandler& operator=(TESCombatEventHandler&&) = delete;
	};


	class TESContainerChangedEventHandler : public RE::BSTEventSink<RE::TESContainerChangedEvent>
	{
	public:
		virtual RE::EventResult ReceiveEvent(RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource) override;

		static TESContainerChangedEventHandler* GetSingleton();

	protected:
		TESContainerChangedEventHandler() = default;
		TESContainerChangedEventHandler(const TESContainerChangedEventHandler&) = delete;
		TESContainerChangedEventHandler(TESContainerChangedEventHandler&&) = delete;
		virtual ~TESContainerChangedEventHandler() = default;

		TESContainerChangedEventHandler& operator=(const TESContainerChangedEventHandler&) = delete;
		TESContainerChangedEventHandler& operator=(TESContainerChangedEventHandler&&) = delete;
	};
}
