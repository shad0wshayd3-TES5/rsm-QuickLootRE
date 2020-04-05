#pragma once

#include <optional>
#include <vector>

#include "RE/Skyrim.h"


class InputDisabler
{
public:
	InputDisabler();
	InputDisabler(const InputDisabler&) = default;
	InputDisabler(InputDisabler&&) = default;

	~InputDisabler();

	InputDisabler& operator=(const InputDisabler&) = default;
	InputDisabler& operator=(InputDisabler&&) = default;

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
