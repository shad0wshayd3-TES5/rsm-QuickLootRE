#pragma once

namespace Input
{
	class IHandler
	{
	public:
		virtual ~IHandler() = default;

		inline void operator()(RE::InputEvent* const& a_event) { DoHandle(a_event); }

	protected:
		virtual void DoHandle(RE::InputEvent* const& a_event) = 0;
	};

	class ScrollHandler :
		public IHandler
	{
	public:
		ScrollHandler();

	protected:
		inline void DoHandle(RE::InputEvent* const& a_event) override
		{
			using Device = RE::INPUT_DEVICE;

			for (auto iter = a_event; iter; iter = iter->next) {
				auto event = iter->AsButtonEvent();
				if (event && CanProcess(*event) && ProcessInput(*event)) {
					return;
				}
			}
		}

	private:
		using mapping_type = std::map<std::uint32_t, std::function<void()>>;

		class ScrollTimer
		{
		public:
			using value_type = double;

			constexpr void advance() noexcept
			{
				if (_doDelay) {
					_timer += _delay;
					_doDelay = false;
				} else {
					_timer += _speed;
				}
			}

			[[nodiscard]] constexpr value_type get() const noexcept { return _timer; }

			constexpr void reset() noexcept
			{
				_timer = 0.0;
				_doDelay = true;
			}

		private:
			value_type _timer{ 0.0 };
			value_type _delay{ 0.5 };
			value_type _speed{ 0.05 };
			bool _doDelay{ true };
		};

		[[nodiscard]] inline bool CanProcess(const RE::ButtonEvent& a_event)
		{
			using Device = RE::INPUT_DEVICE;
			switch (a_event.GetDevice()) {
			case Device::kMouse:
				return true;
			default:
				if (a_event.IsPressed() && a_event.HeldDuration() < _scrollTimer.get()) {
					return false;
				} else if (a_event.IsUp()) {
					_scrollTimer.reset();
					return false;
				} else {
					_scrollTimer.advance();
					return true;
				}
			}
		}

		[[nodiscard]] inline bool ProcessInput(const RE::ButtonEvent& a_event)
		{
			const auto device = a_event.GetDevice();
			if (0 <= device && device < _mappings.size()) {
				const auto& mappings = _mappings[device];
				const auto it = mappings.find(a_event.GetIDCode());
				if (it != mappings.end()) {
					it->second();
					return true;
				}
			} else {
				assert(false);
			}

			return false;
		}

		std::array<mapping_type, RE::INPUT_DEVICES::kTotal> _mappings;
		ScrollTimer _scrollTimer;
	};

	class TakeHandler :
		public IHandler
	{
	protected:
		inline void DoHandle(RE::InputEvent* const& a_event) override
		{
			for (auto iter = a_event; iter; iter = iter->next) {
				auto event = iter->AsButtonEvent();
				if (!event) {
					continue;
				}

				auto controlMap = RE::ControlMap::GetSingleton();
				const auto idCode =
					controlMap ?
                        controlMap->GetMappedKey("Activate", event->GetDevice()) :
                        RE::ControlMap::kInvalid;

				if (event->GetIDCode() == idCode) {
					if (event->IsHeld() && event->HeldDuration() > GetGrabDelay()) {
						TryGrab();
						return;
					} else if (event->IsUp()) {
						TakeStack();
						return;
					}
				}
			}
		}

	private:
		inline float GetGrabDelay() const
		{
			if (_grabDelay) {
				return _grabDelay->GetFloat();
			} else {
				assert(false);
				return std::numeric_limits<float>::max();
			}
		}

		void TakeStack();
		void TryGrab();

		observer<RE::Setting*> _grabDelay{ RE::GetINISetting("fZKeyDelay:Controls") };
	};

	class TransferHandler :
		public IHandler
	{
	protected:
		void DoHandle(RE::InputEvent* const& a_event) override;
	};

	class Listeners :
		public RE::BSTEventSink<RE::InputEvent*>
	{
	public:
		inline Listeners()
		{
			_callbacks.push_back(std::make_unique<TakeHandler>());
			_callbacks.push_back(std::make_unique<ScrollHandler>());
			_callbacks.push_back(std::make_unique<TransferHandler>());
		}

		Listeners(const Listeners&) = default;
		Listeners(Listeners&&) = default;

		inline ~Listeners() { Disable(); }

		Listeners& operator=(const Listeners&) = default;
		Listeners& operator=(Listeners&&) = default;

		inline void Enable()
		{
			auto input = RE::BSInputDeviceManager::GetSingleton();
			if (input) {
				input->AddEventSink(this);
			}
		}

		inline void Disable()
		{
			auto input = RE::BSInputDeviceManager::GetSingleton();
			if (input) {
				input->RemoveEventSink(this);
			}
		}

	private:
		using EventResult = RE::BSEventNotifyControl;

		inline EventResult ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) override
		{
			if (a_event) {
				for (auto& callback : _callbacks) {
					(*callback)(*a_event);
				}
			}

			return EventResult::kContinue;
		}

		std::vector<std::unique_ptr<IHandler>> _callbacks{};
	};
}
