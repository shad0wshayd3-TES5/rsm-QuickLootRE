#pragma once

namespace Input
{
	class Disablers
	{
	public:
		inline Disablers() :
			_disablers()
		{
			_disablers.emplace_back(UEFlag::kActivate);
			_disablers.emplace_back(UEFlag::kPOVSwitch);

			Enable();
		}

		Disablers(const Disablers&) = default;
		Disablers(Disablers&&) = default;

		inline ~Disablers() { Disable(); }

		Disablers& operator=(const Disablers&) = default;
		Disablers& operator=(Disablers&&) = default;

	private:
		using UEFlag = RE::UserEvents::USER_EVENT_FLAG;

		class Disabler
		{
		public:
			Disabler() = delete;
			Disabler(const Disabler&) = default;
			Disabler(Disabler&&) = default;

			inline Disabler(UEFlag a_flag) :
				_originalState(std::nullopt),
				_flag(a_flag)
			{}

			~Disabler() = default;

			Disabler& operator=(const Disabler&) = default;
			Disabler& operator=(Disabler&&) = default;

			inline void Enable()
			{
				auto controlMap = RE::ControlMap::GetSingleton();
				if (controlMap) {
					_originalState = controlMap->AreControlsEnabled(_flag);
					controlMap->ToggleControls(_flag, false);
				}
			}

			inline void Disable()
			{
				if (_originalState) {
					auto controlMap = RE::ControlMap::GetSingleton();
					if (controlMap) {
						controlMap->ToggleControls(_flag, *_originalState);
					}
					_originalState = std::nullopt;
				}
			}

		private:
			std::optional<bool> _originalState;
			UEFlag _flag;
		};

		inline void Enable()
		{
			for (auto& disabler : _disablers) {
				disabler.Enable();
			}
		}

		inline void Disable()
		{
			for (auto& disabler : _disablers) {
				disabler.Disable();
			}
		}

		std::vector<Disabler> _disablers;
	};
}
