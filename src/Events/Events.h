#pragma once

namespace Events
{
	class CrosshairRefHandler :
		public RE::BSTEventSink<SKSE::CrosshairRefEvent>
	{
	public:
		static inline CrosshairRefHandler* GetSingleton()
		{
			static CrosshairRefHandler singleton;
			return std::addressof(singleton);
		}

		static inline void Register()
		{
			auto source = SKSE::GetCrosshairRefEventSource();
			if (source) {
				source->AddEventSink(GetSingleton());
				_MESSAGE("Registered %s", typeid(CrosshairRefHandler).name());
			}
		}

	protected:
		using EventResult = RE::BSEventNotifyControl;

		EventResult ProcessEvent(const SKSE::CrosshairRefEvent* a_event, RE::BSTEventSource<SKSE::CrosshairRefEvent>* a_eventSource) override;

	private:
		CrosshairRefHandler() = default;
		CrosshairRefHandler(const CrosshairRefHandler&) = delete;
		CrosshairRefHandler(CrosshairRefHandler&&) = delete;

		~CrosshairRefHandler() = default;

		CrosshairRefHandler& operator=(const CrosshairRefHandler&) = delete;
		CrosshairRefHandler& operator=(CrosshairRefHandler&&) = delete;

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
		CrosshairRefHandler::Register();
		_MESSAGE("Registered all event handlers");
	}
}
