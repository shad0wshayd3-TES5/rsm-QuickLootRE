#pragma once

namespace Events
{
	class CrosshairRefManager :
		public RE::BSTEventSink<SKSE::CrosshairRefEvent>,
		public RE::BSTEventSink<RE::TESLockChangedEvent>
	{
	public:
		static inline CrosshairRefManager* GetSingleton()
		{
			static CrosshairRefManager singleton;
			return std::addressof(singleton);
		}

		static inline void Register()
		{
			auto crosshair = SKSE::GetCrosshairRefEventSource();
			if (crosshair) {
				crosshair->AddEventSink(GetSingleton());
				logger::info("Registered {}"sv, typeid(SKSE::CrosshairRefEvent).name());
			}

			auto scripts = RE::ScriptEventSourceHolder::GetSingleton();
			if (scripts) {
				scripts->AddEventSink<RE::TESLockChangedEvent>(GetSingleton());
				logger::info("Registered {}"sv, typeid(RE::TESLockChangedEvent).name());
			}
		}

	protected:
		using EventResult = RE::BSEventNotifyControl;

		inline EventResult ProcessEvent(const SKSE::CrosshairRefEvent* a_event, RE::BSTEventSource<SKSE::CrosshairRefEvent>* a_eventSource) override
		{
			auto crosshairRef =
				a_event && a_event->crosshairRef ?
					a_event->crosshairRef->CreateRefHandle() :
					RE::ObjectRefHandle();
			if (_cachedRef == crosshairRef) {
				return EventResult::kContinue;
			}

			_cachedRef = crosshairRef;
			_cachedAshPile.reset();
			Evaluate(a_event->crosshairRef);

			return EventResult::kContinue;
		}

		inline EventResult ProcessEvent(const RE::TESLockChangedEvent* a_event, RE::BSTEventSource<RE::TESLockChangedEvent>*) override
		{
			if (a_event->lockedObject &&
				a_event->lockedObject->GetHandle() == _cachedRef) {
				Evaluate(a_event->lockedObject);
			}

			return EventResult::kContinue;
		}

	private:
		CrosshairRefManager() = default;
		CrosshairRefManager(const CrosshairRefManager&) = delete;
		CrosshairRefManager(CrosshairRefManager&&) = delete;

		~CrosshairRefManager() = default;

		CrosshairRefManager& operator=(const CrosshairRefManager&) = delete;
		CrosshairRefManager& operator=(CrosshairRefManager&&) = delete;

		[[nodiscard]] inline bool CanOpen(RE::TESObjectREFRPtr a_ref)
		{
			auto obj = a_ref ? a_ref->GetObjectReference() : nullptr;
			if (!a_ref || !obj) {
				return false;
			}

			if (obj->Is(RE::FormType::Activator)) {
				_cachedAshPile = a_ref->extraList.GetAshPileRef();
				return CanOpen(_cachedAshPile.get());
			}

			if (auto actor = a_ref->As<RE::Actor>(); actor) {
				if (!actor->IsDead() ||
					actor->IsSummoned()) {
					return false;
				}
			}

			return a_ref->HasContainer() &&
				   !a_ref->IsLocked() &&
				   !a_ref->IsActivationBlocked();
		}

		void Evaluate(RE::TESObjectREFRPtr a_ref);

		RE::ObjectRefHandle _cachedRef;
		RE::ObjectRefHandle _cachedAshPile;
	};

	inline void Register()
	{
		CrosshairRefManager::Register();
		logger::info("Registered all event handlers"sv);
	}
}
