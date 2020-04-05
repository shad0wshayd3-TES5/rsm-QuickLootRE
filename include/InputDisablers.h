#pragma once

#include <optional>
#include <vector>

#include "RE/Skyrim.h"


namespace Input
{
	class Disablers
	{
	public:
		Disablers();
		Disablers(const Disablers&) = default;
		Disablers(Disablers&&) = default;

		~Disablers();

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
			Disabler(UEFlag a_flag);

			~Disabler() = default;

			Disabler& operator=(const Disabler&) = default;
			Disabler& operator=(Disabler&&) = default;

			void Enable();
			void Disable();

		private:
			std::optional<bool> _originalState;
			UEFlag _flag;
		};

		void Enable();
		void Disable();

		std::vector<Disabler> _disablers;
	};
}
