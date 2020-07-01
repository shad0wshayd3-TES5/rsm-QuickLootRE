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

		[[nodiscard]] inline bool CanOpen() const
		{
			if (!_cachedRef) {
				return false;
			}

			switch (_cachedRef->GetFormType()) {
			case RE::FormType::Reference:
				return _cachedRef->GetContainer();
			case RE::FormType::ActorCharacter:
				return _cachedRef->IsDead();
			default:
				return false;
			}
		}

		RE::TESObjectREFRPtr _cachedRef;
	};

	inline void Register()
	{
		CrosshairRefManager::Register();
		_MESSAGE("Registered all event handlers");
	}
}
