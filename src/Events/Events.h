#pragma once

namespace Events
{
	class CrosshairRefManager :
		public RE::BSTEventSink<SKSE::CrosshairRefEvent>
	{
	public:
		static inline CrosshairRefManager* GetSingleton()
		{
			static CrosshairRefManager singleton;
			return std::addressof(singleton);
		}

		static inline void Register()
		{
			auto source = SKSE::GetCrosshairRefEventSource();
			if (source) {
				source->AddEventSink(GetSingleton());
				_MESSAGE("Registered %s", typeid(CrosshairRefManager).name());
			}
		}

	protected:
		using EventResult = RE::BSEventNotifyControl;

		EventResult ProcessEvent(const SKSE::CrosshairRefEvent* a_event, RE::BSTEventSource<SKSE::CrosshairRefEvent>* a_eventSource) override;

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

			switch (a_ref->GetFormType()) {
			case RE::FormType::Reference:
				switch (obj->GetFormType()) {
				case RE::FormType::Activator:
					if (auto ashPile = a_ref->extraList.GetAshPileRef(); ashPile) {
						_cachedAshPile = ashPile;
						return CanOpen(_cachedAshPile.get());
					}
				default:
					return a_ref->GetContainer();
				}
			case RE::FormType::ActorCharacter:
				return a_ref->IsDead();
			default:
				return false;
			}
		}

		RE::ObjectRefHandle _cachedRef;
		RE::ObjectRefHandle _cachedAshPile;
	};

	inline void Register()
	{
		CrosshairRefManager::Register();
		_MESSAGE("Registered all event handlers");
	}
}
