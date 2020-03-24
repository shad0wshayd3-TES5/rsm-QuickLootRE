#pragma once

#include "RE/Skyrim.h"
#include "SKSE/Events.h"


namespace Events
{
	class CrosshairRefHandler : public RE::BSTEventSink<SKSE::CrosshairRefEvent>
	{
	public:
		static CrosshairRefHandler* GetSingleton();

		static void Register();

	protected:
		using EventResult = RE::BSEventNotifyControl;


		virtual EventResult ProcessEvent(const SKSE::CrosshairRefEvent* a_event, RE::BSTEventSource<SKSE::CrosshairRefEvent>* a_eventSource) override;

	private:
		CrosshairRefHandler() = default;
		CrosshairRefHandler(const CrosshairRefHandler&) = delete;
		CrosshairRefHandler(CrosshairRefHandler&&) = delete;
		virtual ~CrosshairRefHandler() = default;

		CrosshairRefHandler& operator=(const CrosshairRefHandler&) = delete;
		CrosshairRefHandler& operator=(CrosshairRefHandler&&) = delete;


		RE::TESObjectREFRPtr _cachedRef;
	};


	void Register();
}
